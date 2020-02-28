#include "bacnet_object.h"

CBacnetObject::CBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID, forte::core::io::IOConfigFBBase *paConfigFB) : mObjectType(paObjectType), mObjectID(paObjectID), mConfigFB(paConfigFB) {

}

CBacnetObject::~CBacnetObject() {
    
}