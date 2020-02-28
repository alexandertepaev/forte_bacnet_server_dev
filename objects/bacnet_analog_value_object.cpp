#include "bacnet_analog_value_object.h"

CBacnetAnalogValueObject::CBacnetAnalogValueObject(uint32_t paObjectID, float paPresentValue, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetObject(OBJECT_ANALOG_VALUE, paObjectID, paConfigFB), mPresentValue(paPresentValue) {
  mPresentValue = paPresentValue;
  DEVLOG_DEBUG("[CBacnetAnalogValueObject] CBacnetAnalogValueObject(): Initialized Analog Value Object: ObjID=%d, PresentValue=%f\n", mObjectID, mPresentValue);
}

CBacnetAnalogValueObject::~CBacnetAnalogValueObject() {
    
}

int CBacnetAnalogValueObject::encodeApplicationData(uint8_t *buffer,  BACNET_PROPERTY_ID property) {
  int len = 0;
  switch (property)
  {   
  case PROP_PRESENT_VALUE:
      len = encode_application_real(buffer, mPresentValue);
      break;
  
  default:
      break;
  }
  return len;
}

 int CBacnetAnalogValueObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
   switch (property)
    {
    case PROP_PRESENT_VALUE:
        mPresentValue = paData->type.Real;
        break;
    
    default:
        break;
    }
 }

 float CBacnetAnalogValueObject::getPresentValue() {
   return mPresentValue;
 }

 void CBacnetAnalogValueObject::setPresentValue(float paValue) {
   mPresentValue = paValue;
 }