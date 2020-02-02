#include "bacnet_analog_value_object.h"

CBacnetAnalogValueObject::CBacnetAnalogValueObject(uint32_t paObjectID) : CBacnetObject(OBJECT_ANALOG_VALUE, paObjectID) {

}

CBacnetAnalogValueObject::~CBacnetAnalogValueObject() {
    
}