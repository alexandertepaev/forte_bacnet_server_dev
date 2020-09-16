#include "bacnet_binary_input_object.h"
// #include "../BACnetBinaryInput.h"

CBacnetBinaryInputObject::CBacnetBinaryInputObject(uint32_t paObjectID, bool paPresentValue, bool paReversePolarity, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetCOVReportingObject(OBJECT_BINARY_INPUT, paObjectID, paCOVReportingEnabled, paConfigFB), mPresentValue(paPresentValue), mReversePolarity(paReversePolarity), mOutOfService(true)
{
  DEVLOG_DEBUG("[CBacnetBinaryInputObject] CBacnetBinaryInputObject(): Initialized Binary Output Object: ObjID=%d, PresentValue=%d\n", mObjectID, mPresentValue);
}

CBacnetBinaryInputObject::~CBacnetBinaryInputObject()
{
}

int CBacnetBinaryInputObject::readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property) {
  int len = 0;
  switch (property)
  {   
    case PROP_PRESENT_VALUE:
      len = encode_application_enumerated(buffer, mPresentValue);
      break;
    default:
      break;
  }
  return len;
}

bool CBacnetBinaryInputObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
  switch (property)
  {
    case PROP_PRESENT_VALUE:
      // mOutOfService = !(static_cast<FORTE_BACnetBinaryInput *>(mConfigFB)->isInService());
      if(mOutOfService){
        return false;
      }
      setPresentValue(paData->type.Enumerated);
      return true;
      break;
  default:
      break;
  }
  return false;
}

bool CBacnetBinaryInputObject::getPresentValue() {
  return mPresentValue;
}

void CBacnetBinaryInputObject::setPresentValue(bool paValue) {
  if(COVReportingEnabled() && mPresentValue != paValue)
    setCOVCondition();
  mPresentValue = paValue;
}

void CBacnetBinaryInputObject::encodeValueList(BACNET_PROPERTY_VALUE* value_list) {
  //present value
  value_list->propertyIdentifier = PROP_PRESENT_VALUE;
  value_list->propertyArrayIndex = BACNET_ARRAY_ALL;
  value_list->value.context_specific = false;
  value_list->value.tag = BACNET_APPLICATION_TAG_ENUMERATED;
  value_list->value.next = NULL;
  value_list->value.type.Enumerated = mPresentValue;
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

bool CBacnetBinaryInputObject::isPolarityReversed() {
  return mReversePolarity;
}

void CBacnetBinaryInputObject::setOutOfService(bool paValue) {
  mOutOfService = paValue;
}