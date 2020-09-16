#include "bacnet_analog_input_object.h"

CBACnetAnalogInputObject::CBACnetAnalogInputObject(TForteUInt32 paObjectID, float paPresentValue, bool paCOVReportingEnabled, float paCOVIncrement, CBacnetObjectConfigFB *paConfigFB) : CBacnetCOVReportingObject(OBJECT_ANALOG_INPUT, paObjectID, paCOVReportingEnabled, paConfigFB), mPresentValue(paPresentValue), mCOVIncrement(paCOVIncrement) {
  mPresentValue = paPresentValue;
  DEVLOG_DEBUG("[CBACnetAnalogInputObject] CBACnetAnalogInputObject(): Initialized Analog Value Object: ObjID=%d, PresentValue=%f\n", mObjectID, mPresentValue);
}

CBACnetAnalogInputObject::~CBACnetAnalogInputObject() {
    
}

int CBACnetAnalogInputObject::readProperty(TForteUInt8 *paBuffer,  BACNET_PROPERTY_ID paProperty) {
  int len = 0;
  switch (paProperty)
  {   
    case PROP_PRESENT_VALUE:
      len = encode_application_real(paBuffer, mPresentValue);
      break;

    default:
      break;
  }
  return len;
}

bool CBACnetAnalogInputObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
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

float CBACnetAnalogInputObject::getPresentValue() {
  return mPresentValue;
}

void CBACnetAnalogInputObject::setPresentValue(float paValue) {
  if(COVReportingEnabled() && mPresentValue != paValue && std::abs(mPresentValue-paValue) >= mCOVIncrement)
    setCOVCondition();
  mPresentValue = paValue;
}

void CBACnetAnalogInputObject::encodeValueList(BACNET_PROPERTY_VALUE* paValueList) {
  //present value
  paValueList->propertyIdentifier = PROP_PRESENT_VALUE;
  paValueList->propertyArrayIndex = BACNET_ARRAY_ALL;
  paValueList->value.context_specific = false;
  paValueList->value.tag = BACNET_APPLICATION_TAG_REAL;
  paValueList->value.next = NULL;
  paValueList->value.type.Real = mPresentValue;
  paValueList->priority = BACNET_NO_PRIORITY;
  paValueList = paValueList->next;

  //status flags (TODO: hardcoded)
  paValueList->propertyIdentifier = PROP_PRESENT_VALUE;
  paValueList->propertyArrayIndex = BACNET_ARRAY_ALL;
  paValueList->value.context_specific = false;
  paValueList->value.tag = BACNET_APPLICATION_TAG_BIT_STRING;
  paValueList->value.next = NULL;
  bitstring_init(&paValueList->value.type.Bit_String);
  bitstring_set_bit(&paValueList->value.type.Bit_String, STATUS_FLAG_IN_ALARM, false);
  bitstring_set_bit(&paValueList->value.type.Bit_String, STATUS_FLAG_FAULT, false);
  bitstring_set_bit(&paValueList->value.type.Bit_String, STATUS_FLAG_OVERRIDDEN, false);
  bitstring_set_bit(&paValueList->value.type.Bit_String, STATUS_FLAG_OUT_OF_SERVICE, false);
  paValueList->priority = BACNET_NO_PRIORITY;
  paValueList = paValueList->next;
}