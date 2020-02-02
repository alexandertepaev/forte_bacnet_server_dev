#ifndef _BACNET_SERVER_CONTROLLER_H_
#define _BACNET_SERVER_CONTROLLER_H_

#include <fortelist.h>

class CBacnetObject;

class CBacnetServerController {
  friend class FORTE_BACnetServer;
  public:
    CBacnetServerController();
    ~CBacnetServerController();

    bool addObjectTableEntry(CBacnetObject *paObject);

  protected:

  private:
    uint32_t mPort;

    typedef CSinglyLinkedList<CBacnetObject *> TObjectTable;
    TObjectTable *mObjectTable;

    bool init(uint16_t paPort);
    
};

#endif