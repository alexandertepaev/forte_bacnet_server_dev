#include "bacnet_device_object.h"

CBacnetDeviceObject::CBacnetDeviceObject(uint32_t paObjectID, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetObject(OBJECT_DEVICE, paObjectID, paConfigFB) {

}

CBacnetDeviceObject::~CBacnetDeviceObject() {
    
}

int CBacnetDeviceObject::encodeApplicationData(uint8_t *buffer,  BACNET_PROPERTY_ID property) {

}

 int CBacnetDeviceObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {

 }