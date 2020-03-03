#include <devlog.h>
#include <extevhan.h>
#include <devexec.h>
#include "objects/bacnet_object.h"
#include "objects/bacnet_cov_reporting_object.h"
#include "bacnet_server_controller.h"

CBacnetServerController::CBacnetServerController(CDeviceExecution& paDeviceExecution): CExternalEventHandler(paDeviceExecution), mObjectTable(NULL), mCOVReporters(NULL) {
  DEVLOG_DEBUG("[CBacnetServerController] CBacnetServerController(): created new server controller instance\n");
  memset(&m_stServerConfig, 0, sizeof(ServerConfig));
}

CBacnetServerController::~CBacnetServerController() {

}



bool CBacnetServerController::init(uint16_t paPort) {
 

  mObjectTable = new TObjectTable();
  mCOVReporters = new TCOVReporters();
  
  m_stServerConfig.Port = htons(paPort);
  DEVLOG_DEBUG("[CBacnetServerController] init(): Initializing controller on Port=%04X\n", htons(m_stServerConfig.Port));
  
  initNetworkAddresses();

  mBacnetSocket = openBacnetIPSocket();
  
  return true;
}

#define NETWORK_IFACE_NAME "eth0"
bool CBacnetServerController::initNetworkAddresses(){
  ifreq ifr = {};
  struct sockaddr_in ip_address;
  int rv, fd;

  fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

  strncpy(ifr.ifr_name, NETWORK_IFACE_NAME, sizeof(ifr.ifr_name));
  if(fd < 0){
    DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses(): Failed\n");
    return false;
  } else {
    // local address
    rv = ioctl(fd, SIOCGIFADDR, &ifr); // todo check if secced
    if(rv<0) {
      DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses(): Failed\n");
      return false;
    }
    ip_address = *((struct sockaddr_in *) &ifr.ifr_addr);
    m_stServerConfig.LocalAddr = ip_address.sin_addr;
    DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses(): Local Address=%s:%04X\n", inet_ntoa(m_stServerConfig.LocalAddr), htons(m_stServerConfig.Port));

    // netmask
    rv = ioctl(fd, SIOCGIFNETMASK, &ifr);
    if(rv<0) {
      DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses(): Failed\n");
      return false;
    }
    ip_address = *((struct sockaddr_in *) &ifr.ifr_addr);
    DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses(): Netmask=%s\n", inet_ntoa(ip_address.sin_addr));
    close(fd);
    // broadcast
    m_stServerConfig.BroadcastAddr.s_addr = m_stServerConfig.LocalAddr.s_addr | (~ip_address.sin_addr.s_addr);
    DEVLOG_DEBUG("[CBacnetServerController] initNetworkAddresses() Broadcast Address: %s\n", inet_ntoa(m_stServerConfig.BroadcastAddr));
    
  }
  return true;
}


CBacnetServerController::TSocketDescriptor CBacnetServerController::openBacnetIPSocket() {
  //open socket
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd <= 0) {
    DEVLOG_DEBUG("[CBacnetServerController] Failed to open socket\n");
    return -1;
  }
  // allow reuse addr:port combo
  int sockopt = 1;
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0) {
    DEVLOG_DEBUG("[CBacnetServerController] Setsockopt failed: reuse\n");
  }
  // allow broadcast msgs
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof(sockopt)) < 0) {
    DEVLOG_DEBUG("[CBacnetServerController] Setsockopt failed: broadcast\n");
  }
  

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = m_stServerConfig.Port;

  //bind socket
  if( bind(sockfd, (const sockaddr *)&address, sizeof(sockaddr_in)) < 0 ){
    DEVLOG_DEBUG("[CBacnetServerController] Failed to bind socket\n");
    return -1;
  } else {
    return sockfd;
  }
}

bool CBacnetServerController::addObjectTableEntry(CBacnetObject *paObject) {
  if(paObject == NULL)
    return false;

  //TODO check for duplicates
  mObjectTable->pushBack(paObject);

  DEVLOG_DEBUG("[CBacnetServerController] addObjectTableEntry(): updated object table with Object=%d, 0bjectID=%d\n", paObject->mObjectType, paObject->mObjectID);
  return true;
}

void CBacnetServerController::addCOVReportersEntry(CBacnetCOVReportingObject *paObject) {
//void CBacnetServerController::addCOVReportersEntry(CBacnetObject *paObject) {
  
  TSubscriptions *subs = new TSubscriptions();
  SCOVReporter *newEntry = new SCOVReporter(paObject, subs);

  mCOVReporters->pushBack(newEntry);

  DEVLOG_DEBUG("[CBacnetServerController] addCOVReportersEntry(): added COVReporters entry\n");
  
}

void CBacnetServerController::initDone() {
  start();
}

void CBacnetServerController::run() {
  while(isAlive()) {

    sockaddr_in src;
    if(receivePacket(100, &src) > 0) {
      handleReceivedPacket(src);
    }

    processPendingCOVReports();

    /*
    processPendingCOVReports();
    ---------------------------------------

    processPendingCOVReports:
      for each e in mCOVReporters:
        if e->obj->COVCondition():
          sendCOVReport(e)
          e->obj->clearCOVCondition();

    ---------------------------------------

    sendCOVReport(e):
      for each subscriber in e->subs:
        // build packet and send
    */

  }
}

ssize_t CBacnetServerController::receivePacket(uint16_t timeout, sockaddr_in *src) {
  struct timeval select_timeout;
  select_timeout.tv_sec = 0;
  select_timeout.tv_usec = 1000 * timeout; // 10 ms timeout on receive - non-blocking recv
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(mBacnetSocket, &read_fds);

  if(select(mBacnetSocket+1, &read_fds, NULL, NULL, &select_timeout) > 0){
    socklen_t srcLen = sizeof(sockaddr_in);
    ssize_t rcv_retval = recvfrom(mBacnetSocket, mReceiveBuffer, sizeof(mReceiveBuffer), 0, (sockaddr *)src, &srcLen);
    if(rcv_retval > 0) {
      DEVLOG_DEBUG("[CBacnetServerController] Received a BACnet packet of length %d\n", rcv_retval);
    } else  {
      DEVLOG_DEBUG("[CBacnetServerController] recvfrom() failed\n");
    }

    // msg from myself -- ignore
    if(src != NULL && src->sin_addr.s_addr == m_stServerConfig.LocalAddr.s_addr && src->sin_port == m_stServerConfig.Port) { 
      return 0;
    } else {
      // it's all good
      return rcv_retval;
    }
  }
  return -1;
}

void CBacnetServerController::handleReceivedPacket(sockaddr_in &src){
  uint8_t service_choice, apdu_type, invoke_id = 0;
  uint32_t apdu_offset, apdu_len, service_req_offset = 0;

  uint32_t response_len = 0;
  
  if(decodeNPDU(apdu_offset, apdu_len) && decodeAPDU(apdu_offset, invoke_id, service_choice, service_req_offset)) {
    switch (service_choice) {
    case SERVICE_CONFIRMED_READ_PROPERTY:
      handleReadPropertyRequest(service_req_offset, apdu_len-4, src, invoke_id); // 1st bit - service choice, 2d bit - flags, 3d - invoke id, 4th - service choice
      break;
    case SERVICE_CONFIRMED_WRITE_PROPERTY:
      handleWritePropertyRequest(service_req_offset, apdu_len-4, src, invoke_id);
      break;
    case SERVICE_UNCONFIRMED_WHO_IS:
      handleWhoIsRequest(service_req_offset, apdu_len-2, src);
      break;
    case SERVICE_CONFIRMED_SUBSCRIBE_COV:
      handleSubscribeCOVRequest(service_req_offset, apdu_len-4, src, invoke_id);
      break;
    default:
      break;
    }
  }
    //handleRequest(src, invoke_id, service_choice, service_request_offset)
  

  /*if(decodeNPDU(apdu_offset, apdu_len)) {

    DEVLOG_DEBUG("[CBacnetServerController] NPDU info: apdu_len=%d, apdu_offset=%d\n", apdu_len, apdu_offset);

    //handleAPDU(&mReceiveBuffer[apdu_offset+4], apdu_len, apdu_type, service_choice);
    decodeAPDU(apdu_offset, invoke_id, service_choice, service_req_offset);
  } else {
      DEVLOG_DEBUG("[CBacnetServerController] decodeNPDU() failed\n");
  }*/


}



bool CBacnetServerController::decodeNPDU(uint32_t &apdu_offset, uint32_t &apdu_len){
  if (mReceiveBuffer[0] == BVLL_TYPE_BACNET_IP) {
    /* Bacnet/IP signature check */
    uint16_t npdu_len;
    (void) decode_unsigned16(&mReceiveBuffer[2], &npdu_len);

    if ((npdu_len >= 4) && (npdu_len <= (MAX_MPDU-4))) { 
      /* Length of NPDU is OK (including BVLC length) */

      if(mReceiveBuffer[1] == BVLC_ORIGINAL_UNICAST_NPDU || mReceiveBuffer[1] == BVLC_ORIGINAL_BROADCAST_NPDU) {
        /* For now only handle unicast/broadcast BVLC types */

        if (mReceiveBuffer[4] == BACNET_PROTOCOL_VERSION) {
          /* Protocol version 0x01 */

          BACNET_NPDU_DATA npdu_data;
          BACNET_ADDRESS dest;
          apdu_offset = npdu_decode(&mReceiveBuffer[4], &dest, NULL, &npdu_data);

          if(!npdu_data.network_layer_message && (apdu_offset > 0) && (apdu_offset+4 <= npdu_len) 
              && ((dest.net == 0) || (dest.net == BACNET_BROADCAST_NETWORK))) {
                /* NOT network layer message, APDU offset is in bounds, NOT routing infromation */
                apdu_len = npdu_len-4-apdu_offset;
                apdu_offset += 4;
                return true;
                
                // apdu_type = (mReceiveBuffer[apdu_offset+4] & 0xF0);

                // if((dest.net != BACNET_BROADCAST_NETWORK) || (apdu_type != PDU_TYPE_CONFIRMED_SERVICE_REQUEST )) {

                //   /* NOT confirmed Broadcast (TODO read section 5.4.5.1)*/


                //   switch (apdu_type)
                //   {
                //     // case PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST:  
                //     //   /* I-Am + UnconfirmedCOVNotification*/
                //     //   service_choice = mReceiveBuffer[apdu_offset+4+1];
                //     //   if(service_choice < MAX_BACNET_UNCONFIRMED_SERVICE)
                //     //     return true;
                //     //   break;

                //     // case PDU_TYPE_SIMPLE_ACK:
                //     //   /* WriteProperty acknowledge */
                //     //   service_choice = mReceiveBuffer[apdu_offset+4+2];
                //     //   if(service_choice == SERVICE_CONFIRMED_WRITE_PROPERTY ||
                //     //       service_choice == SERVICE_CONFIRMED_SUBSCRIBE_COV)
                //     //     return true;
                //     //   break;

                //     // case PDU_TYPE_COMPLEX_ACK:
                //     //   /* ReadProperty acknowledge and result */
                //     //   if( !(mReceiveBuffer[apdu_offset+4] & 1<<3) ) {
                //     //     /* NOT segmented */
                //     //     service_choice = mReceiveBuffer[apdu_offset+4+2];
                //     //     if(service_choice == SERVICE_CONFIRMED_READ_PROPERTY)
                //     //       return true;
                //     //   }
                //     //   break;

                //     // default:
                //     //   return false;
                //     //   break;
                //   }
                  
                // }

          }
        }
      }
    }
  }

  return false;
}

bool CBacnetServerController::decodeAPDU(const uint32_t &apdu_offset, uint8_t &invoke_id, uint8_t &service_choice, uint32_t &service_req_offset) {
  BACNET_PDU_TYPE apdu_type = (BACNET_PDU_TYPE) (mReceiveBuffer[apdu_offset] & 0xF0);

  switch (apdu_type)
  {
  case PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST:
    /* WhoIs */

    service_choice = mReceiveBuffer[apdu_offset+1];

    if(service_choice == SERVICE_UNCONFIRMED_WHO_IS) {
      service_req_offset = apdu_offset+2;
      DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST, service_choice=%d service_req_offset=%d\n", service_choice, service_req_offset);
      return true;
    } else {
       DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST, service not supported\n");
    }
    
    break;
  
  case PDU_TYPE_CONFIRMED_SERVICE_REQUEST:
    /*SubscribeCOV, WriteProperty, ReadProperty*/
    
    service_choice = mReceiveBuffer[apdu_offset+3];
    
    if(service_choice == SERVICE_CONFIRMED_READ_PROPERTY || service_choice == SERVICE_CONFIRMED_WRITE_PROPERTY || service_choice ==  SERVICE_CONFIRMED_SUBSCRIBE_COV) {
      invoke_id = mReceiveBuffer[apdu_offset+2];
      service_req_offset = apdu_offset+4;
      DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_CONFIRMED_SERVICE_REQUEST, invoke_id=%d, service_choice=%d, service_req_offset=%d\n", invoke_id, service_choice, service_req_offset);
      return true;
    } else {
      DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_CONFIRMED_SERVICE_REQUEST service not supported\n");
    }
    
    break;
  default:
    break;
  }

  return false;
}

void CBacnetServerController::handleReadPropertyRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &paSrc, const uint8_t &invoke_id) {
  BACNET_READ_PROPERTY_DATA rpdata;
  int len = rp_decode_service_request(&mReceiveBuffer[service_req_offset], service_req_len, &rpdata);
  if(len > 0) {
    CBacnetObject *obj = getBacnetObject(rpdata.object_type, rpdata.object_instance);
    
    if(obj == NULL) {
       DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): target object not found\n");
       return;
    }

    DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): ObjectType=%d, ObjectInstance=%d, ObjectProperty=%d\n", rpdata.object_type, rpdata.object_instance, rpdata.object_property);

    /*
      dva varianta: 1) функция obj->readProperty() возвращает CIEC_ANY, потом CIEC_ANY передается в функцию, которая кодирует ответ
                    2) в функцию obj->readProperty() передается параметр (указатель), на место которого кодируется present value
    */

    BACNET_NPDU_DATA npdu_data;
    npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
    BACNET_ADDRESS dest = ipToBacnetAddress(paSrc.sin_addr, paSrc.sin_port, false);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.LocalAddr, m_stServerConfig.Port, false);
    
    int pdu_len = 4;
    pdu_len += npdu_encode_pdu(&mSendBuffer[pdu_len], &dest, &src, &npdu_data);
    pdu_len += rp_ack_encode_apdu_init(&mSendBuffer[pdu_len], invoke_id, &rpdata);
    //obj->encodeApplicationData(&mSendBuffer[...], rpdata.object_property);
    int app_data_len = obj->readProperty(&mSendBuffer[pdu_len], rpdata.object_property);

    if (app_data_len>0) {
      pdu_len += app_data_len;
      pdu_len += rp_ack_encode_apdu_object_property_end(&mSendBuffer[pdu_len]);
      mSendBuffer[0] = BVLL_TYPE_BACNET_IP;
      mSendBuffer[1] = BVLC_ORIGINAL_UNICAST_NPDU; 
      encode_unsigned16(&mSendBuffer[2], pdu_len);
      if(sendPacket(pdu_len, paSrc) > 0){
        DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): Sent ReadProperty Ack!\n");
      }
    } else {
      DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): obj->readProperty() failed\n");
    }
  }


}

void CBacnetServerController::handleWritePropertyRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &paSrc, const uint8_t &invoke_id) {
  BACNET_WRITE_PROPERTY_DATA wpdata;
  int len = wp_decode_service_request(&mReceiveBuffer[service_req_offset], service_req_len, &wpdata);

  if(len > 0) {
    CBacnetObject *obj = getBacnetObject(wpdata.object_type
    , wpdata.object_instance);
  
    if(obj == NULL) {
       DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): target object not found\n");
       return;
    }

    DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): ObjectType=%d, ObjectInstance=%d, ObjectProperty=%d\n", wpdata.object_type, wpdata.object_instance, wpdata.object_property);


    BACNET_APPLICATION_DATA_VALUE value;
    if(bacapp_decode_application_data(wpdata.application_data, wpdata.application_data_len, &value) > 0){
      
      if(obj->writeProperty(&value, wpdata.object_property)) {
        // update ConfigFB;
        startNewEventChain(obj->getConfigFB());
      
        // send acknowledge
        BACNET_NPDU_DATA npdu_data;
        npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
        BACNET_ADDRESS dest = ipToBacnetAddress(paSrc.sin_addr, paSrc.sin_port, false);
        BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.LocalAddr, m_stServerConfig.Port, false);
        int pdu_len = 4;
        pdu_len += npdu_encode_pdu(&mSendBuffer[pdu_len], &dest, &src, &npdu_data);
        pdu_len += encode_simple_ack(&mSendBuffer[pdu_len], invoke_id, SERVICE_CONFIRMED_WRITE_PROPERTY);

        mSendBuffer[0] = BVLL_TYPE_BACNET_IP;
        mSendBuffer[1] = BVLC_ORIGINAL_UNICAST_NPDU; 
        encode_unsigned16(&mSendBuffer[2], pdu_len);

        if(sendPacket(pdu_len, paSrc) > 0){
          DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): Sent WriteProperty Ack!\n");
        }
      } else {
         DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): obj->writeProeprty() failed\n");
      }
      
    } 
  }
}

void CBacnetServerController::handleWhoIsRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &src) {
  int32_t low, high = 0;
  int len = whois_decode_service_request(&mReceiveBuffer[service_req_offset], service_req_len, &low, &high);


  //CBacnetObject *obj = getBacnetObject(wpdata.object_type, wpdata.object_instance);
  const CBacnetObject *obj = (*mObjectTable->begin());

  if(len != BACNET_STATUS_ERROR && (len == 0 || (obj->mObjectID >= low && obj->mObjectID <= high))) {
    DEVLOG_DEBUG("[CBacnetServerController] handleWhoIsRequest(): Received WhoIs request, sending IAm\n");
    BACNET_NPDU_DATA npdu_data;
    npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
    BACNET_ADDRESS dest = ipToBacnetAddress(m_stServerConfig.BroadcastAddr, m_stServerConfig.Port, true);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.LocalAddr, m_stServerConfig.Port, false);
    
    int pdu_len = 4;
    pdu_len += npdu_encode_pdu(&mSendBuffer[pdu_len], &dest, &src, &npdu_data);

    pdu_len += iam_encode_apdu(&mSendBuffer[pdu_len], obj->mObjectID, MAX_APDU, SEGMENTATION_NONE, 260); // 260 - BACnet Stack at Sourceforge vendor ID


    mSendBuffer[0] = BVLL_TYPE_BACNET_IP;
    mSendBuffer[1] = BVLC_ORIGINAL_UNICAST_NPDU; 
    encode_unsigned16(&mSendBuffer[2], pdu_len);

    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr = m_stServerConfig.BroadcastAddr;
    destAddr.sin_port = m_stServerConfig.Port;
    memset(&destAddr.sin_zero, 0, sizeof(destAddr.sin_zero));

    if(sendPacket(pdu_len, destAddr) > 0){
      DEVLOG_DEBUG("Sent!\n");
    }
  }
}

void CBacnetServerController::handleSubscribeCOVRequest(const uint32_t &service_req_offset, uint16_t service_req_len, sockaddr_in &paSrc, const uint8_t &invoke_id) {
  BACNET_SUBSCRIBE_COV_DATA cov_data;
  int len = cov_subscribe_decode_service_request(&mReceiveBuffer[service_req_offset], service_req_offset, &cov_data);

  // not cancellation request and not subcov confirmed request
  if(len > 0 && !cov_data.cancellationRequest && !cov_data.issueConfirmedNotifications) {
    DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Subscribing to ObjectType=%d, ObjectInstance=%d\n", cov_data.monitoredObjectIdentifier.type, cov_data.monitoredObjectIdentifier.instance);

    // TODO check for duplicates
    SCOVReporter *cov_reporters_entry = getCOVReportersEntry((BACNET_OBJECT_TYPE) cov_data.monitoredObjectIdentifier.type, cov_data.monitoredObjectIdentifier.instance);
    
    if(cov_reporters_entry != NULL) {
      //cov_reporters_entry->covData = &cov_data; // TODO -> covData per each subscription -> rethink subs list

      /*
        updateCOVSubscriptionDataList(TSubscriptions *list, BACNET_SUBSCRIBE_COV_DATA *cov_data, in_addr *paAddr, uint16_t *paPort); 
      */

      if(updateCOVSubscriptionDataList(cov_reporters_entry->subs, &cov_data, &paSrc.sin_addr, &paSrc.sin_port)) {
        DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Subscribe OK: pushed addr %s:%04X\n", inet_ntoa(paSrc.sin_addr), htons(paSrc.sin_port));

        //cov_reporters_entry->obj->setCOVCondition(); //TODO remove
        
        BACNET_NPDU_DATA npdu_data;
        npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
        BACNET_ADDRESS dest = ipToBacnetAddress(paSrc.sin_addr, paSrc.sin_port, false);
        BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.LocalAddr, m_stServerConfig.Port, false);
        int pdu_len = 4;
        pdu_len += npdu_encode_pdu(&mSendBuffer[pdu_len], &dest, &src, &npdu_data);
        pdu_len += encode_simple_ack(&mSendBuffer[pdu_len], invoke_id, SERVICE_CONFIRMED_SUBSCRIBE_COV);
        mSendBuffer[0] = BVLL_TYPE_BACNET_IP;
        mSendBuffer[1] = BVLC_ORIGINAL_UNICAST_NPDU; 
        encode_unsigned16(&mSendBuffer[2], pdu_len);
        if(sendPacket(pdu_len, paSrc) > 0){
          DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Sent SubscribeCOV Ack!\n");
        }
      } else {
        DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Subscribe FAILED: duplicated subscription\n");
      }
    } else {
      DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Subscribe FAILED: target object not found\n");
    }

  }

}

void CBacnetServerController::processPendingCOVReports() {
  TCOVReporters::Iterator itEnd = mCOVReporters->end();
  for(TCOVReporters::Iterator it = mCOVReporters->begin(); it != itEnd; ++it){
    if((*it)->obj->COVCondition()) {
      sendCOVReports((*it));
      (*it)->obj->clearCOVCondition();
    }
  }
}

void CBacnetServerController::sendCOVReports(SCOVReporter *paCOVReporter) {

  BACNET_PROPERTY_VALUE value_list[2]; // FIXME: for now only present_value+statusflags
  value_list[0].next = &value_list[1];
  value_list[1].next = NULL;

  paCOVReporter->obj->encodeValueList(value_list);

  TSubscriptions::Iterator itEnd = paCOVReporter->subs->end();
  for(TSubscriptions::Iterator it = paCOVReporter->subs->begin(); it != itEnd; ++it){

    BACNET_NPDU_DATA npdu_data;
    npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
    BACNET_ADDRESS dest = ipToBacnetAddress((*it)->addr, (*it)->port, false);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.LocalAddr, m_stServerConfig.Port, false);
    int pdu_len = 4;
    pdu_len += npdu_encode_pdu(&mSendBuffer[pdu_len], &dest, &src, &npdu_data);
    
    BACNET_COV_DATA cov_data;
    cov_data.subscriberProcessIdentifier = (*it)->covData.subscriberProcessIdentifier;
    cov_data.initiatingDeviceIdentifier = (*mObjectTable->begin())->mObjectID;
    cov_data.monitoredObjectIdentifier.type = (*it)->covData.monitoredObjectIdentifier.type;
    cov_data.monitoredObjectIdentifier.instance = (*it)->covData.monitoredObjectIdentifier.instance;
    cov_data.timeRemaining = (*it)->covData.lifetime;
    cov_data.listOfValues = value_list;

    pdu_len += ucov_notify_encode_apdu(&mSendBuffer[pdu_len], &cov_data);

    mSendBuffer[0] = BVLL_TYPE_BACNET_IP;
    mSendBuffer[1] = BVLC_ORIGINAL_UNICAST_NPDU; 
    encode_unsigned16(&mSendBuffer[2], pdu_len);


    struct sockaddr_in destNetworkAddr;
    destNetworkAddr.sin_family = AF_INET;
    destNetworkAddr.sin_addr.s_addr = (*it)->addr.s_addr;
    destNetworkAddr.sin_port = (*it)->port;
    memset(destNetworkAddr.sin_zero, 0, 8);

    if(sendPacket(pdu_len, destNetworkAddr) > 0){
      DEVLOG_DEBUG("[CBacnetServerController] sendCOVReports(): Sent COV Report!\n");
    }

  }
}

CBacnetObject* CBacnetServerController::getBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectInstance) {
  TObjectTable::Iterator itEnd = mObjectTable->end();
  for(TObjectTable::Iterator it = mObjectTable->begin(); it != itEnd; ++it){ 
    if((*it)->mObjectType == paObjectType && (*it)->mObjectID == paObjectInstance) {
      return (*it);
    }
      
  }
  return NULL;
}

CBacnetServerController::SCOVReporter* CBacnetServerController::getCOVReportersEntry(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectInstance) {

  TCOVReporters::Iterator itEnd = mCOVReporters->end();
  for(TCOVReporters::Iterator it = mCOVReporters->begin(); it != itEnd; ++it){
    if((*it)->obj->mObjectType == paObjectType && (*it)->obj->mObjectID == paObjectInstance) {
      return (*it);
    }
  }
  return NULL;
}

bool CBacnetServerController::updateCOVSubscriptionDataList(TSubscriptions *list, BACNET_SUBSCRIBE_COV_DATA *cov_data, in_addr *paAddr, uint16_t *paPort) {
  TSubscriptions::Iterator itEnd = list->end();
  for(TSubscriptions::Iterator it = list->begin(); it != itEnd; ++it){ 
    if(cov_data->subscriberProcessIdentifier == (*it)->covData.subscriberProcessIdentifier && paAddr->s_addr == (*it)->addr.s_addr && (*paPort) == (*it)->port ) {
      return false;
    }
  }

  list->pushBack(new SSubscriptionData((*paAddr), (*paPort), (*cov_data)));
  return true;
}

BACNET_ADDRESS CBacnetServerController::ipToBacnetAddress(struct in_addr paDeviceAddr, uint16_t paPort, bool paBroadcastAddr){
  BACNET_ADDRESS ret_val;
  ret_val.mac_len = (uint8_t) 6;
  memcpy(&ret_val.mac[0], &paDeviceAddr, 4);
  memcpy(&ret_val.mac[4], &paPort, 2);
  ret_val.net = paBroadcastAddr ? BACNET_BROADCAST_NETWORK : 0;
  ret_val.len = 0; // no SLEN
  memset(ret_val.adr, 0, MAX_MAC_LEN); // no SADR

  return ret_val;
}

int CBacnetServerController::sendPacket(uint16_t len, const sockaddr_in &paDest) {
  // struct sockaddr_in bvlc_dest;
  // bvlc_dest.sin_family = AF_INET;
  // bvlc_dest.sin_addr.s_addr = dest_addr.s_addr;
  // bvlc_dest.sin_port = dest_port; //TODO remove htons
  // memset(bvlc_dest.sin_zero, 0, 8);

  //int send_len = sendto(mBacnetSocket, (char *) mSendBuffer, len, 0, (struct sockaddr *) &bvlc_dest, sizeof(struct sockaddr));
  int send_len = sendto(mBacnetSocket, (char *) mSendBuffer, len, 0, (struct sockaddr *) &paDest, sizeof(struct sockaddr));
  if(send_len > 0){
    DEVLOG_DEBUG("[CBacnetClientController] Sent a BACnet packet of length %d\n", send_len);
  } else {
    DEVLOG_DEBUG("[CBacnetClientController] sendto() failed\n");
  }
  return send_len;
}