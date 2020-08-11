#include "bacnet_analog_output_object.h"

CBacnetAnalogOutputObject::CBacnetAnalogOutputObject(uint32_t paObjectID, float paPresentValue, bool paCOVReportingEnabled, float paCOVIncrement, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetCOVReportingObject(OBJECT_ANALOG_OUTPUT, paObjectID, paCOVReportingEnabled, paConfigFB), mPresentValue(paPresentValue), mCOVIncrement(paCOVIncrement) {
  mPresentValue = paPresentValue;
  DEVLOG_DEBUG("[CBacnetAnalogOutputObject] CBacnetAnalogOutputObject(): Initialized Analog Value Object: ObjID=%d, PresentValue=%f\n", mObjectID, mPresentValue);
}

CBacnetAnalogOutputObject::~CBacnetAnalogOutputObject() {
    
}

int CBacnetAnalogOutputObject::readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property) {
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

bool CBacnetAnalogOutputObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
  switch (property)
  {
    case PROP_PRESENT_VALUE:
      setPresentValue(paData->type.Real);
      return true;
      break;
    default:
      break;
  }
  return false;
}

float CBacnetAnalogOutputObject::getPresentValue() {
  
  return mPresentValue;
}

void CBacnetAnalogOutputObject::setPresentValue(float paValue) {
  if(COVReportingEnabled() && mPresentValue != paValue && std::abs(mPresentValue-paValue) >= mCOVIncrement)
    setCOVCondition();
  mPresentValue = paValue;
}

void CBacnetAnalogOutputObject::encodeValueList(BACNET_PROPERTY_VALUE* value_list) {
  //present value
  value_list->propertyIdentifier = PROP_PRESENT_VALUE;
  value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
  value_list->value.context_specific = false;
  value_list->value.tag = BACNET_APPLICATION_TAG_REAL;
  value_list->value.next = NULL;
  value_list->value.type.Real = mPresentValue;
  value_list->priority = BACNET_NO_PRIORITY;
  value_list = value_list->next;

  //status flags (TODO: hardcoded)
  value_list->propertyIdentifier = PROP_PRESENT_VALUE;
  value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
  value_list->value.context_specific = false;
  value_list->value.tag = BACNET_APPLICATION_TAG_BIT_STRING;
  value_list->value.next = NULL;
  bitstring_init(&value_list->value.type.Bit_String);
  bitstring_set_bit(&value_list->value.type.Bit_String, STATUS_FLAG_IN_ALARM, false);
  bitstring_set_bit(&value_list->value.type.Bit_String, STATUS_FLAG_FAULT, false);
  bitstring_set_bit(&value_list->value.type.Bit_String, STATUS_FLAG_OVERRIDDEN, false);
  bitstring_set_bit(&value_list->value.type.Bit_String, STATUS_FLAG_OUT_OF_SERVICE, false);
  value_list->priority = BACNET_NO_PRIORITY;
  value_list = value_list->next;
}