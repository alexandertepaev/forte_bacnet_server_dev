/*******************************************************************************
 * Copyright (c) 2020 Alexander Tepaev github.com/alexandertepaev
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alexander Tepaev
 *******************************************************************************/
#include <devlog.h>
#include <extevhan.h>
#include <devexec.h>
#include "objects/bacnet_object.h"
#include "objects/bacnet_cov_reporting_object.h"
#include "bacnet_server_controller.h"



CBacnetServerController::CBacnetServerController(CDeviceExecution& paDeviceExecution, TForteUInt16 paID): CExternalEventHandler(paDeviceExecution), m_nID(paID), mObjectTable(NULL), mCOVReporters(NULL) {

}

CBacnetServerController::~CBacnetServerController() {
  deinit();
}

const char* CBacnetServerController::init(TForteUInt16 paPort) {
  DEVLOG_DEBUG("[CBacnetServerController] init(): Controller ID: %d\n", m_nID);

  // object table
  mObjectTable = new TObjectTable();
  
  // list of objects that can issue cov reports
  mCOVReporters = new TCOVReporters();
  
  // get network addresses
  m_stServerConfig.nPort = htons(paPort); // Store in HTONS order
  setNetworkAddresses(); // FIXME ERROR CHECKING
  DEVLOG_DEBUG("[CBacnetServerController] init(): Local address: %s:%04X\n", inet_ntoa(m_stServerConfig.stLocalAddr), htons(m_stServerConfig.nPort));
  DEVLOG_DEBUG("[CBacnetServerController] init(): Broadcast address: %s:%04X\n",inet_ntoa(m_stServerConfig.stBroadcastAddr), htons(m_stServerConfig.nPort));
  
  // open communication socket
  if(!initCommunicationSocket())
    return scmSocketInitFailed; // return error message
  
  return 0;
}

void CBacnetServerController::deinit() {
  if(mCommunicationSocket > 0){
    close(mCommunicationSocket);
  }
}

void CBacnetServerController::setNetworkAddresses() {
  ifreq ifr = {};
  struct sockaddr_in IPAddress;
  int rv, fd; // TODO: check return values
  fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  strncpy(ifr.ifr_name, scm_sNetwrokIfaceName, sizeof(ifr.ifr_name));
  if(fd >= 0) {
    // local address
    rv = ioctl(fd, SIOCGIFADDR, &ifr);
    IPAddress = *((struct sockaddr_in *) &ifr.ifr_addr);
    this->m_stServerConfig.stLocalAddr = IPAddress.sin_addr;

    // netmask
    rv = ioctl(fd, SIOCGIFNETMASK, &ifr);
    IPAddress = *((struct sockaddr_in *) &ifr.ifr_addr);
    close(fd);

    // broadcast address
    this->m_stServerConfig.stBroadcastAddr.s_addr = m_stServerConfig.stLocalAddr.s_addr | (~IPAddress.sin_addr.s_addr);
  }
}

bool CBacnetServerController::initCommunicationSocket() {
  //open socket
  mCommunicationSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(mCommunicationSocket <= 0) {
    DEVLOG_DEBUG("[CBacnetServerController] Failed to open socket\n");
    return false;
  }
  // allow broadcast msgs
  int sockopt = 1;
  if (setsockopt(mCommunicationSocket, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof(sockopt)) < 0) {
    DEVLOG_DEBUG("[CBacnetServerController] initCommunicationSocket(): setsockopt SO_BROADCAST failed\n");
    return false;
  }
  //bind socket
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = m_stServerConfig.nPort;
  if(bind(mCommunicationSocket, (const sockaddr *)&address, sizeof(sockaddr_in)) < 0){
    DEVLOG_DEBUG("[CBacnetServerController] initCommunicationSocket(): Failed to bind socket\n");
    return false;
  }
  return true;
}

void CBacnetServerController::initDone() {
  start();
}

bool CBacnetServerController::addObjectTableEntry(CBacnetObject *paObject) {
  if(paObject == NULL)
    return false;
  //TODO: check for duplicates
  mObjectTable->pushBack(paObject);
  DEVLOG_DEBUG("[CBacnetServerController] addObjectTableEntry(): Updated object table, Object=%d, 0bjectID=%d\n", paObject->mObjectType, paObject->mObjectID);
  return true;
}

void CBacnetServerController::addCOVReportersEntry(CBacnetCOVReportingObject *paObject) {
  TSubscriptions *subs = new TSubscriptions();
  SCOVReporter *newEntry = new SCOVReporter(paObject, subs);
  mCOVReporters->pushBack(newEntry);
  DEVLOG_DEBUG("[CBacnetServerController] addCOVReportersEntry(): Added COVReporters entry\n");
}

void CBacnetServerController::run() {
  while(isAlive()) {
    // try to receive a packet and handle it in case of success
    sockaddr_in src;
    if(receivePacket(scm_nReceivePacketSelectTimeoutMillis, &src) > 0) {
      handleReceivedPacket(src);
    }
    // check and issue pending COV notifications
    processPendingCOVReports();
  }
}

BACNET_ADDRESS CBacnetServerController::ipToBacnetAddress(const struct in_addr &paAddress, const TForteUInt16 &paPort, const bool &paIsBroadcastAddr){
  // see libbacnet's bip_get_broadcast_address and bip_get_my_address 
  BACNET_ADDRESS retVal;
  retVal.mac_len = (TForteUInt8) IP_ADDR_LEN + PORT_LEN;
  memcpy(&retVal.mac[0], &paAddress, IP_ADDR_LEN);
  memcpy(&retVal.mac[IP_ADDR_LEN], &paPort, PORT_LEN);
  retVal.net = paIsBroadcastAddr ? BACNET_BROADCAST_NETWORK : 0; // broadcast -> all 1s, all 0s otherwise
  retVal.len = 0; // no SLEN, local only, no routing
  memset(retVal.adr, 0, MAX_MAC_LEN); // no SADR -> local only, no routing
  return retVal;
}

int CBacnetServerController::sendPacket(const TForteUInt16 &len, const struct in_addr &destinationAddr, const TForteUInt16 &destinationPort) {
  struct sockaddr_in dest;
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = destinationAddr.s_addr;
  dest.sin_port = destinationPort;
  memset(dest.sin_zero, 0, 8);
  int send_len = (int) sendto(mCommunicationSocket, (char *)mSendBuffer, (size_t) len, 0, (struct sockaddr *)&dest, sizeof(struct sockaddr));
  return send_len;
}

int CBacnetServerController::receivePacket(const TForteUInt16 &paTimeout, sockaddr_in *paSourceAddress) {
  struct timeval selectTimeout;
  selectTimeout.tv_sec = 0;
  selectTimeout.tv_usec = scm_nMicrosInMillis * paTimeout;// convert milliseconds to micrseconds
  fd_set readFDs;
  FD_ZERO(&readFDs);
  FD_SET(mCommunicationSocket, &readFDs);
  if(select(mCommunicationSocket+1, &readFDs, NULL, NULL, &selectTimeout) > 0){
    socklen_t srcLen = sizeof(sockaddr_in);
    int rcv_retval = (int) recvfrom(mCommunicationSocket, mReceiveBuffer, sizeof(mReceiveBuffer), 0, (sockaddr *)paSourceAddress, &srcLen);
    if(paSourceAddress != NULL && paSourceAddress->sin_addr.s_addr ==  m_stServerConfig.stLocalAddr.s_addr && paSourceAddress->sin_port == m_stServerConfig.nPort) { 
      return 0; // msg from myself
    } else {
      return rcv_retval; // receive succeeded
    }
  }
  return -1; // nothing
}

void CBacnetServerController::handleReceivedPacket(sockaddr_in &paSourceAddress){
  TForteUInt8 serviceChoice = 0;
  TForteUInt8 invokeID = 0;
  TForteUInt16 APDUOffset = 0;
  TForteUInt16 APDULen = 0;
  TForteUInt16 ServiceRequestOffset = 0;
  
  //TODO apply the same logic in the client implementation? 
  if(decodeNPDU(APDUOffset, APDULen) && decodeAPDU(APDUOffset, invokeID, serviceChoice, ServiceRequestOffset)) {
    switch (serviceChoice) {
      case SERVICE_CONFIRMED_READ_PROPERTY:
        handleReadPropertyRequest(ServiceRequestOffset, (TForteUInt16)(APDULen-CONFIRMED_REQ_APP_TAGS_OFFSET), invokeID, paSourceAddress); // 1st bit - service choice, 2d bit - flags, 3d - invoke id, 4th - service choice
        break;
      case SERVICE_CONFIRMED_WRITE_PROPERTY:
        handleWritePropertyRequest(ServiceRequestOffset, (TForteUInt16) (APDULen-CONFIRMED_REQ_APP_TAGS_OFFSET), invokeID, paSourceAddress);
        break;
      case SERVICE_UNCONFIRMED_WHO_IS:
        handleWhoIsRequest(ServiceRequestOffset, (TForteUInt16)(APDULen-UNCONFIRMED_REQ_APP_TAGS_OFFSET));
        break;
      case SERVICE_CONFIRMED_SUBSCRIBE_COV:
        handleSubscribeCOVRequest(ServiceRequestOffset, invokeID, paSourceAddress);
        break;
      default:
        break;
    }
  }
}


bool CBacnetServerController::decodeNPDU(TForteUInt16 &paAPDUOffset, TForteUInt16 &paAPDULen){
  if (mReceiveBuffer[BVLC_TYPE_BYTE] == BVLL_TYPE_BACNET_IP) {
    /* Bacnet/IP signature check */
    TForteUInt16 NPDULen;
    (void) decode_unsigned16(&mReceiveBuffer[2], &NPDULen);
    if ((NPDULen >= BVLC_HEADER_LEN) && (NPDULen <= (MAX_MPDU-BVLC_HEADER_LEN))) {
      /* Length of NPDU is OK (including BVLC length) */
      if(mReceiveBuffer[BVLC_FUNCTION_BYTE] == BVLC_ORIGINAL_UNICAST_NPDU || mReceiveBuffer[BVLC_FUNCTION_BYTE] == BVLC_ORIGINAL_BROADCAST_NPDU) {
        /* For now only handle unicast/broadcast BVLC types */
        if (mReceiveBuffer[NPDU_OFFSET] == BACNET_PROTOCOL_VERSION) {
          /* Protocol version 0x01 */
          BACNET_NPDU_DATA NPDUData;
          BACNET_ADDRESS dest;
          paAPDUOffset = (TForteUInt16) npdu_decode(&mReceiveBuffer[NPDU_OFFSET], &dest, NULL, &NPDUData);
          if(!NPDUData.network_layer_message && (paAPDUOffset > 0) && (paAPDUOffset <= NPDULen - BVLC_HEADER_LEN) 
              && ((dest.net == 0) || (dest.net == BACNET_BROADCAST_NETWORK))) {
                /* NOT network layer message, APDU offset is in bounds, NOT routing infromation */
                paAPDUOffset = (TForteUInt16)(paAPDUOffset+BVLC_HEADER_LEN);
                paAPDULen = (TForteUInt16)(NPDULen-paAPDUOffset);
                return true;
          }
        }
      }
    }
  }
  return false;
}

bool CBacnetServerController::decodeAPDU(const TForteUInt16 &paAPDUOffset, TForteUInt8 &paInvokeID, TForteUInt8 &paServiceChoice, TForteUInt16 &paServiceRequestOffset) {
  
  TForteUInt8 APDUType = (BACNET_PDU_TYPE) (mReceiveBuffer[paAPDUOffset] & APDU_TYPE_MASK);
  
  switch (APDUType)
  {
    case PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST:
      /* WhoIs */
      paServiceChoice = mReceiveBuffer[paAPDUOffset+UNCONFIRMED_REQ_SERVICE_CHOICE_OFFSET];
      if(paServiceChoice == SERVICE_UNCONFIRMED_WHO_IS) {
        paServiceRequestOffset = (TForteUInt16)(paAPDUOffset+UNCONFIRMED_REQ_APP_TAGS_OFFSET); // beginning of the ServiceRequest data
        DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST, service_choice=%d service_req_offset=%d\n", paServiceChoice, paServiceRequestOffset);
        return true;
      } else {
        DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST, service not supported\n");
      }
      break;
    case PDU_TYPE_CONFIRMED_SERVICE_REQUEST:
      /*SubscribeCOV, WriteProperty, ReadProperty*/
      paServiceChoice = mReceiveBuffer[paAPDUOffset+CONFIRMED_REQ_SERVICE_CHOICE_OFFSET];
      if(paServiceChoice == SERVICE_CONFIRMED_READ_PROPERTY || paServiceChoice == SERVICE_CONFIRMED_WRITE_PROPERTY || paServiceChoice ==  SERVICE_CONFIRMED_SUBSCRIBE_COV) {
        paInvokeID = mReceiveBuffer[paAPDUOffset+CONFIRMED_REQ_INVOKE_ID_OFFSET];
        paServiceRequestOffset = (TForteUInt16)(paAPDUOffset+CONFIRMED_REQ_APP_TAGS_OFFSET); // beginning of the ServiceRequest data
        DEVLOG_DEBUG("[CBacnetServerController] decodeAPDU(): PDU_TYPE_CONFIRMED_SERVICE_REQUEST, invoke_id=%d, service_choice=%d, service_req_offset=%d\n", paInvokeID, paServiceChoice, paServiceRequestOffset);
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

void CBacnetServerController::handleReadPropertyRequest(const TForteUInt16 &paServiceRequestOffset, TForteUInt16 paServiceRequestLen, const TForteUInt8 &paInvokeID, const sockaddr_in &paSourceAddress) {
  // Decode ReadProperty service request data
  BACNET_READ_PROPERTY_DATA rpdata;
  int len = rp_decode_service_request(&mReceiveBuffer[paServiceRequestOffset], paServiceRequestLen, &rpdata);
  
  // if length of the data > 0, look up for the desired object
  if(len > 0) {
    CBacnetObject *obj = getBacnetObject(rpdata.object_type, rpdata.object_instance);
    if(obj == NULL) {
       DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): Target object not found in the object table\n");
       return;
    }
    DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): ObjectType=%d, ObjectInstance=%d, ObjectProperty=%d\n", rpdata.object_type, rpdata.object_instance, rpdata.object_property);

    
    // If found, start to encode ComplexAck message
    // addresses to libnbacnet format
    BACNET_ADDRESS dest = ipToBacnetAddress(paSourceAddress.sin_addr, paSourceAddress.sin_port, false);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.stLocalAddr, m_stServerConfig.nPort, false);
    
    // npdu portion 
    BACNET_NPDU_DATA NPDUData;
    npdu_encode_npdu_data(&NPDUData, false, MESSAGE_PRIORITY_NORMAL);
    TForteUInt16 PDULen = BVLC_HEADER_LEN;
    PDULen = (TForteUInt16) (PDULen + npdu_encode_pdu(&mSendBuffer[PDULen], &dest, &src, &NPDUData));
    
    // apdu portion
    PDULen = (TForteUInt16) (PDULen + rp_ack_encode_apdu_init(&mSendBuffer[PDULen], paInvokeID, &rpdata));
    // ask object to encode the desired property into the mSendBuffer, returns number of encoded bytes
    int appDataLen = obj->readProperty(&mSendBuffer[PDULen], rpdata.object_property);
    // if encoding a value succeeded, finish the construction of the message and send it
    if (appDataLen>0) {
      // encode end part of the ack msg
      PDULen = (TForteUInt16) (PDULen + appDataLen);
      PDULen = (TForteUInt16)( PDULen + rp_ack_encode_apdu_object_property_end(&mSendBuffer[PDULen]));
      // BVLC header
      mSendBuffer[BVLC_TYPE_BYTE] = BVLL_TYPE_BACNET_IP;
      mSendBuffer[BVLC_FUNCTION_BYTE] = BVLC_ORIGINAL_UNICAST_NPDU;
      encode_unsigned16(&mSendBuffer[BVLC_LEN_BYTE], PDULen);
      // Perform send
      if(sendPacket(PDULen, paSourceAddress.sin_addr, paSourceAddress.sin_port) > 0){
        DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): Sent ReadProperty Ack\n");
      }
    } else {
      DEVLOG_DEBUG("[CBacnetServerController] handleReadPropertyRequest(): obj->readProperty() failed\n");
    }
  }
}

void CBacnetServerController::handleWritePropertyRequest(const TForteUInt16 &service_req_offset, TForteUInt16 service_req_len, const TForteUInt8 &invoke_id, const sockaddr_in &paSrc) {
  // Decode WriteProperty service request data
  BACNET_WRITE_PROPERTY_DATA wpdata;
  int len = wp_decode_service_request(&mReceiveBuffer[service_req_offset], service_req_len, &wpdata);

  // if length of the data > 0, look up for the desired object
  if(len > 0) {
    CBacnetObject *obj = getBacnetObject(wpdata.object_type
    , wpdata.object_instance);
    if(obj == NULL) {
       DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): Target object not found in the object table\n");
       return;
    }
    DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): ObjectType=%d, ObjectInstance=%d, ObjectProperty=%d\n", wpdata.object_type, wpdata.object_instance, wpdata.object_property);
    
    // If found, decode the application data (value that has to be written)
    // and perform write
    BACNET_APPLICATION_DATA_VALUE value;
    if( (bacapp_decode_application_data(wpdata.application_data, wpdata.application_data_len, &value) > 0) &&
        obj->writeProperty(&value, wpdata.object_property)){
        
        // update ConfigFB; TODO - inside the WriteProperty operation
        startNewEventChain(obj->getConfigFB());
        // encode and send simple acknowledgement
        // adrresses to libbacnet format
        BACNET_ADDRESS dest = ipToBacnetAddress(paSrc.sin_addr, paSrc.sin_port, false);
        BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.stLocalAddr, m_stServerConfig.nPort, false);
        // NPDU portion
        BACNET_NPDU_DATA NPDUData;
        npdu_encode_npdu_data(&NPDUData, false, MESSAGE_PRIORITY_NORMAL);
        TForteUInt16 PDULen = BVLC_HEADER_LEN;
        PDULen = (TForteUInt16) (PDULen + npdu_encode_pdu(&mSendBuffer[PDULen], &dest, &src, &NPDUData));
        // APDU portion
        PDULen = (TForteUInt16) (PDULen + encode_simple_ack(&mSendBuffer[PDULen], invoke_id, SERVICE_CONFIRMED_WRITE_PROPERTY));
        // BVLC header
        mSendBuffer[BVLC_TYPE_BYTE] = BVLL_TYPE_BACNET_IP;
        mSendBuffer[BVLC_FUNCTION_BYTE] = BVLC_ORIGINAL_UNICAST_NPDU;
        // send
        encode_unsigned16(&mSendBuffer[BVLC_LEN_BYTE], PDULen);
        if(sendPacket(PDULen, paSrc.sin_addr, paSrc.sin_port) > 0) {
          DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): Sent WriteProperty Ack!\n");
        }
    } else {
      DEVLOG_DEBUG("[CBacnetServerController] handleWritePropertyRequest(): Execution of WriteProperty request failed\n");
    }
  }
}

void CBacnetServerController::handleWhoIsRequest(const TForteUInt16 &paServiceRequestOffset, TForteUInt16 paServiceRequestLen) {
  // decode target ids bounds
  TForteUInt32 deviceIDLowerBound = 0;
  TForteUInt32 deviceIDUpperBound = 0;
  int len = whois_decode_service_request(&mReceiveBuffer[paServiceRequestOffset], paServiceRequestLen, (TForteInt32 *)&deviceIDLowerBound, (TForteInt32 *)&deviceIDUpperBound);
  // get the device object 
  const CBacnetObject *obj = (*mObjectTable->begin());
  // compare and send I-Am 
  if(len >= 0 && (obj->mObjectID >= deviceIDLowerBound && obj->mObjectID <= deviceIDUpperBound)) {
    DEVLOG_DEBUG("[CBacnetServerController] handleWhoIsRequest(): Received Who-Is request, sending I-Am\n");
    BACNET_ADDRESS dest = ipToBacnetAddress(m_stServerConfig.stBroadcastAddr, m_stServerConfig.nPort, true);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.stLocalAddr, m_stServerConfig.nPort, false);
    BACNET_NPDU_DATA NPDUData;
    npdu_encode_npdu_data(&NPDUData, false, MESSAGE_PRIORITY_NORMAL);
    TForteUInt16 PDULen = BVLC_HEADER_LEN;
    PDULen = (TForteUInt16) (PDULen + npdu_encode_pdu(&mSendBuffer[PDULen], &dest, &src, &NPDUData));
    PDULen = (TForteUInt16) (PDULen + iam_encode_apdu(&mSendBuffer[PDULen], obj->mObjectID, MAX_APDU, SEGMENTATION_NONE, 260)); // 260 - BACnet Stack at Sourceforge vendor ID
    mSendBuffer[BVLC_TYPE_BYTE] = BVLL_TYPE_BACNET_IP;
    mSendBuffer[BVLC_FUNCTION_BYTE] = BVLC_ORIGINAL_UNICAST_NPDU; 
    encode_unsigned16(&mSendBuffer[BVLC_LEN_BYTE], PDULen);
    if(sendPacket(PDULen, m_stServerConfig.stBroadcastAddr, m_stServerConfig.nPort) > 0) {
      DEVLOG_DEBUG("Sent I-Am broadcast\n");
    }
  }
}

void CBacnetServerController::handleSubscribeCOVRequest(const TForteUInt16 &paServiceRequestOffset, const TForteUInt8 &paInvokeID, sockaddr_in &paSourceAddress) {
  // decode subcov app data
  BACNET_SUBSCRIBE_COV_DATA cov_data;
  int len = cov_subscribe_decode_service_request(&mReceiveBuffer[paServiceRequestOffset], paServiceRequestOffset, &cov_data);

  // not cancellation request and not subcov confirmed request
  if(len > 0 && !cov_data.cancellationRequest && !cov_data.issueConfirmedNotifications) {

    // TODO check for duplicates or allow multiple subscriptions? (probably iff subscriptions have different process ids)
    SCOVReporter *cov_reporters_entry = getCOVReportersEntry((BACNET_OBJECT_TYPE) cov_data.monitoredObjectIdentifier.type, cov_data.monitoredObjectIdentifier.instance);
    
    if(cov_reporters_entry != NULL) {
      // if found cov reporter, update its subs data list
      if(updateCOVSubscriptionDataList(cov_reporters_entry->pSubs, &cov_data, &paSourceAddress.sin_addr, &paSourceAddress.sin_port)) {
        
        DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): SubscribeCOV to ObjectType=%d, ObjectInstance=%d\n", cov_data.monitoredObjectIdentifier.type, cov_data.monitoredObjectIdentifier.instance);
        // respond with simple ack if everything is ok
        BACNET_NPDU_DATA npdu_data;
        npdu_encode_npdu_data(&npdu_data, false, MESSAGE_PRIORITY_NORMAL);
        BACNET_ADDRESS dest = ipToBacnetAddress(paSourceAddress.sin_addr, paSourceAddress.sin_port, false);
        BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.stLocalAddr, m_stServerConfig.nPort, false);
        TForteUInt16 PDULen = BVLC_HEADER_LEN;
        PDULen = (TForteUInt16) (PDULen + npdu_encode_pdu(&mSendBuffer[PDULen], &dest, &src, &npdu_data));
        PDULen = (TForteUInt16) (PDULen + encode_simple_ack(&mSendBuffer[PDULen], paInvokeID, SERVICE_CONFIRMED_SUBSCRIBE_COV));
        mSendBuffer[BVLC_TYPE_BYTE] = BVLL_TYPE_BACNET_IP;
        mSendBuffer[BVLC_FUNCTION_BYTE] = BVLC_ORIGINAL_UNICAST_NPDU;
        encode_unsigned16(&mSendBuffer[BVLC_LEN_BYTE], PDULen);
        if(sendPacket(PDULen, paSourceAddress.sin_addr, paSourceAddress.sin_port) > 0) {
          DEVLOG_DEBUG("[CBacnetServerController] handleSubscribeCOVRequest(): Sent SubscribeCOV Ack!\n");
        } 
      }
    } 
  }
}

CBacnetObject* CBacnetServerController::getBacnetObject(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectInstance) {
  TObjectTable::Iterator itEnd = mObjectTable->end();
  for(TObjectTable::Iterator it = mObjectTable->begin(); it != itEnd; ++it){ 
    if((*it)->mObjectType == paObjectType && (*it)->mObjectID == paObjectInstance) {
      return (*it);
    } 
  }
  return NULL;
}

CBacnetServerController::SCOVReporter* CBacnetServerController::getCOVReportersEntry(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectInstance) {
  TCOVReporters::Iterator itEnd = mCOVReporters->end();
  for(TCOVReporters::Iterator it = mCOVReporters->begin(); it != itEnd; ++it){
    if((*it)->pObj->mObjectType == paObjectType && (*it)->pObj->mObjectID == paObjectInstance) {
      return (*it);
    }
  }
  return NULL;
}

bool CBacnetServerController::updateCOVSubscriptionDataList(TSubscriptions *paSubsList, BACNET_SUBSCRIBE_COV_DATA *paCOVData, in_addr *paAddr, TForteUInt16 *paPort) {
  TSubscriptions::Iterator itEnd = paSubsList->end();
  for(TSubscriptions::Iterator it = paSubsList->begin(); it != itEnd; ++it){ 
    if(paCOVData->subscriberProcessIdentifier == (*it)->stCOVData.subscriberProcessIdentifier && paAddr->s_addr == (*it)->stAddr.s_addr && (*paPort) == (*it)->nPort) {
      return false;
    }
  }
  paSubsList->pushBack(new SSubscriptionData((*paAddr), (*paPort), (*paCOVData)));
  return true;
}

void CBacnetServerController::processPendingCOVReports() {
  TCOVReporters::Iterator itEnd = mCOVReporters->end();
  for(TCOVReporters::Iterator it = mCOVReporters->begin(); it != itEnd; ++it){
    if((*it)->pObj->COVCondition()) {
      // for each cov reporter with COVCondition set to true send COV notifications
      sendCOVReports((*it));
      (*it)->pObj->clearCOVCondition();
    }
  }
}

void CBacnetServerController::sendCOVReports(SCOVReporter *paCOVReporter) {

  // FIXME: for now only present_value+statusflags of i/o/val objects
  BACNET_PROPERTY_VALUE valueList[2];
  valueList[0].next = &valueList[1];
  valueList[1].next = NULL;
  // ask object to fille the struct holding reported values
  paCOVReporter->pObj->encodeValueList(valueList);
  // iterate through the list of subscriptions
  TSubscriptions::Iterator itEnd = paCOVReporter->pSubs->end();
  for(TSubscriptions::Iterator it = paCOVReporter->pSubs->begin(); it != itEnd; ++it){
    // for every subscription encode and send COV notification
    BACNET_ADDRESS dest = ipToBacnetAddress((*it)->stAddr, (*it)->nPort, false);
    BACNET_ADDRESS src = ipToBacnetAddress(m_stServerConfig.stLocalAddr, m_stServerConfig.nPort, false);
    //npdu
    BACNET_NPDU_DATA NPDUData;
    npdu_encode_npdu_data(&NPDUData, false, MESSAGE_PRIORITY_NORMAL);
    TForteUInt16 PDULen = BVLC_HEADER_LEN;
    PDULen = (TForteUInt16) (PDULen + npdu_encode_pdu(&mSendBuffer[PDULen], &dest, &src, &NPDUData));
    // cov data: process id, obj if, obj type, obj id, lifetime, list of values
    BACNET_COV_DATA cov_data;
    cov_data.subscriberProcessIdentifier = (*it)->stCOVData.subscriberProcessIdentifier;
    cov_data.initiatingDeviceIdentifier = (*mObjectTable->begin())->mObjectID;
    cov_data.monitoredObjectIdentifier.type = (*it)->stCOVData.monitoredObjectIdentifier.type;
    cov_data.monitoredObjectIdentifier.instance = (*it)->stCOVData.monitoredObjectIdentifier.instance;
    cov_data.timeRemaining = (*it)->stCOVData.lifetime;
    cov_data.listOfValues = valueList;
    // encode unconfirmed cov
    PDULen = (TForteUInt16) (PDULen + ucov_notify_encode_apdu(&mSendBuffer[PDULen], (unsigned int) (sizeof(mSendBuffer) - PDULen), &cov_data));
    // bvlc header
    mSendBuffer[BVLC_TYPE_BYTE] = BVLL_TYPE_BACNET_IP;
    mSendBuffer[BVLC_FUNCTION_BYTE] = BVLC_ORIGINAL_UNICAST_NPDU;
    encode_unsigned16(&mSendBuffer[BVLC_LEN_BYTE], PDULen);
    if(sendPacket(PDULen, (*it)->stAddr, (*it)->nPort) > 0){
      DEVLOG_DEBUG("[CBacnetServerController] sendCOVReports(): Sent COV Report!\n");
    }
  }
}
