#ifndef _BACNET_SERVER_CONTROLLER_H_
#define _BACNET_SERVER_CONTROLLER_H_

#include <fortelist.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <arpa/inet.h>

// #include "../../forte-incubation_1.11.0/src/core/io/device/io_controller.h"
#include <core/io/device/io_controller.h>
#include "include/bacnet.h"

class CBacnetObject;
class CBacnetCOVReportingObject;

class CBacnetServerController: protected CThread, protected CExternalEventHandler {
  friend class FORTE_BACnetServer;
  public:
    CBacnetServerController(CDeviceExecution& paDeviceExecution);
    ~CBacnetServerController();

    bool addObjectTableEntry(CBacnetObject *paObject);
    void addCOVReportersEntry(CBacnetCOVReportingObject *paObject);

    //void addCOVReportersEntry(CBacnetObject *paObject);

  protected:
    void initDone();
  private:

    typedef CSinglyLinkedList<CBacnetObject *> TObjectTable;
    TObjectTable *mObjectTable;

    struct SSubscriptionData {
      in_addr addr;
      uint16_t port;
      BACNET_SUBSCRIBE_COV_DATA covData;

      SSubscriptionData(in_addr paAddr, uint16_t paPort, BACNET_SUBSCRIBE_COV_DATA paCovData) : addr(paAddr), port(paPort), covData(paCovData) {
      }
    };

    typedef CSinglyLinkedList<struct SSubscriptionData *> TSubscriptions;

    // typedef CSinglyLinkedList<struct in_addr *> TSubscribers;


    struct SCOVReporter {
      CBacnetCOVReportingObject *obj;
      //CBacnetObject *obj;
      //TSubscribers *subs;
      TSubscriptions *subs;
      //BACNET_SUBSCRIBE_COV_DATA *covData;

      SCOVReporter(CBacnetCOVReportingObject *paObj, TSubscriptions *paSubs) : obj(paObj), subs(paSubs){
      //SCOVReporter(CBacnetObject *paObj, TSubscribers *paSubs) : obj(paObj), subs(paSubs) {
      }
    };

    typedef CSinglyLinkedList<SCOVReporter *> TCOVReporters;
    TCOVReporters *mCOVReporters;
    bool updateCOVSubscriptionDataList(TSubscriptions *list, BACNET_SUBSCRIBE_COV_DATA *cov_data, in_addr *paAddr, uint16_t *paPort); 


    bool init(uint16_t paPort);

    struct ServerConfig {
      struct in_addr LocalAddr;
      struct in_addr BroadcastAddr;
      uint16_t Port;
    };
    ServerConfig m_stServerConfig;

    bool initNetworkAddresses();

    //socket descriptor
    typedef int TSocketDescriptor;
    TSocketDescriptor mBacnetSocket;
    TSocketDescriptor openBacnetIPSocket();

    void run();
    
    uint8_t mReceiveBuffer[MAX_APDU];
    uint8_t mSendBuffer[MAX_APDU];
    ssize_t receivePacket(uint16_t timeout, sockaddr_in *src);
    int sendPacket(uint16_t len, const sockaddr_in &paDest);
    void handleReceivedPacket(sockaddr_in &src);
    bool decodeNPDU(uint32_t &apdu_offset, uint32_t &apdu_len);
    bool decodeAPDU(const uint32_t &apdu_offset, uint8_t &invoke_id, uint8_t &service_choice, uint32_t &service_req_offset);
    BACNET_ADDRESS ipToBacnetAddress(struct in_addr paDeviceAddr, uint16_t paPort, bool paBroadcastAddr);
    
    void handleReadPropertyRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &src, const uint8_t &invoke_id);
    void handleWritePropertyRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &src, const uint8_t &invoke_id);
    void handleWhoIsRequest(const uint32_t &service_req_offset, uint16_t service_req_len, const sockaddr_in &src);
    void handleSubscribeCOVRequest(const uint32_t &service_req_offset, uint16_t service_req_len, sockaddr_in &src, const uint8_t &invoke_id);
    
    void processPendingCOVReports();
    void sendCOVReports(SCOVReporter *paCOVReporter);
    
    CBacnetObject* getBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectInstance);
    SCOVReporter* getCOVReportersEntry(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectInstance);

    void enableHandler(void) {
    };

    void disableHandler(void) {
    };

    void setPriority(int paPriority){
    };
    int getPriority(void) const {
      return 0;
    };
    size_t getIdentifier() const {
      return 0;
    };



};

#endif