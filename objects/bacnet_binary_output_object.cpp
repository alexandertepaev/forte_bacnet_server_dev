#include "bacnet_binary_output_object.h"

CBacnetBinaryOutputObject::CBacnetBinaryOutputObject(uint32_t paObjectID, bool paPresentValue, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paConfigFB) : CBacnetCOVReportingObject(OBJECT_BINARY_OUTPUT, paObjectID, paCOVReportingEnabled, paConfigFB), mPresentValue(paPresentValue)
{
  DEVLOG_DEBUG("[CBacnetBinaryOutputObject] CBacnetBinaryOutputObject(): Initialized Binary Output Object: ObjID=%d, PresentValue=%d\n", mObjectID, mPresentValue);
}

CBacnetBinaryOutputObject::~CBacnetBinaryOutputObject()
{
}

int CBacnetBinaryOutputObject::readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property) {
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

bool CBacnetBinaryOutputObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property) {
  switch (property)
  {
    case PROP_PRESENT_VALUE:
      // mOutOfService = !(static_cast<FORTE_BACnetBinaryValue *>(mConfigFB)->isInService());
      // if(!mOutOfService){
      //   setPresentValue(paData->type.Enumerated);
      //   return true;
      // } else {
      //   DEVLOG_DEBUG("OUT OF SERVICE!\n");
      //   return false;
      // }
      setPresentValue(paData->type.Enumerated);
      return true;
      break;
  default:
      break;
  }
  return false;
}

bool CBacnetBinaryOutputObject::getPresentValue() {
  return mPresentValue;
}

void CBacnetBinaryOutputObject::setPresentValue(bool paValue) {
  if(COVReportingEnabled() && mPresentValue != paValue)
    setCOVCondition();
  mPresentValue = paValue;
}

void CBacnetBinaryOutputObject::encodeValueList(BACNET_PROPERTY_VALUE* value_list) {
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