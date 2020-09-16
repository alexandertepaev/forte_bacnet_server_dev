#ifndef _BACNET_OBJCET_H_
#define _BACNET_OBJCET_H_

#include "../bacnet.h"
// #include"../../forte-incubation_1.11.0/src/core/io/configFB/io_base.h"
#include <core/io/configFB/io_base.h>

class CBacnetObject {
  friend class CBacnetServerController;
  public:
    CBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID, forte::core::io::IOConfigFBBase *paObjectConfigFB);
    ~CBacnetObject();
    virtual int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property) = 0;
    virtual bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) = 0;
    forte::core::io::IOConfigFBBase *getConfigFB() {
      return mConfigFB;
    }
  protected:
    BACNET_OBJECT_TYPE mObjectType;
    uint32_t mObjectID;
    // this reference is needed in oreded to fire external events to the config FBs upon receipt of a service requests such as WP etc
    forte::core::io::IOConfigFBBase *mConfigFB;
  private:
   
    //virtual void init() = 0;
};

#endif