#ifndef _BACNET_OBJCET_H_
#define _BACNET_OBJCET_H_

#include "../include/bacnet.h"
#include <stdint.h>

class CBacnetObject {
  friend class CBacnetServerController;
  public:
     CBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID);
    ~CBacnetObject();
  protected:
  private:
    BACNET_OBJECT_TYPE mObjectType;
    uint32_t mObjectID;
};

#endif