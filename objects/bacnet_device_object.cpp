#include "bacnet_device_object.h"

CBacnetDeviceObject::CBacnetDeviceObject(uint32_t paObjectID) : CBacnetObject(OBJECT_DEVICE, paObjectID) {

}

CBacnetDeviceObject::~CBacnetDeviceObject() {
    
}