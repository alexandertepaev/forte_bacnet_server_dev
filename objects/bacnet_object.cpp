#include "bacnet_object.h"

CBacnetObject::CBacnetObject(BACNET_OBJECT_TYPE paObjectType, uint32_t paObjectID) : mObjectType(paObjectType), mObjectID(paObjectID) {

}

CBacnetObject::~CBacnetObject() {
    
}