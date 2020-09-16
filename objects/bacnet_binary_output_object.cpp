/*******************************************************************************
 * Copyright (c) 2020 Alexander Tepaev github.com/alexandertepaev
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alexander Tepaev
 *******************************************************************************/

#include "bacnet_binary_output_object.h"

CBacnetBinaryOutputObject::CBacnetBinaryOutputObject(TForteUInt32 paObjectID, bool paPresentValue, bool paCOVReportingEnabled, CBacnetObjectConfigFB *paConfigFB) : CBacnetCOVReportingObject(OBJECT_BINARY_OUTPUT, paObjectID, paCOVReportingEnabled, paConfigFB), mPresentValue(paPresentValue)
{
  DEVLOG_DEBUG("[CBacnetBinaryOutputObject] CBacnetBinaryOutputObject(): Initialized Binary Output Object: ObjID=%d, PresentValue=%d\n", mObjectID, mPresentValue);
}

CBacnetBinaryOutputObject::~CBacnetBinaryOutputObject()
{
}

int CBacnetBinaryOutputObject::readProperty(TForteUInt8 *paBuffer,  BACNET_PROPERTY_ID paProperty) {
  int len = 0;
  switch (paProperty)
  {   
    case PROP_PRESENT_VALUE:
      len = encode_application_enumerated(paBuffer, mPresentValue);
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

void CBacnetBinaryOutputObject::encodeValueList(BACNET_PROPERTY_VALUE* paValueList) {
  //present value
  paValueList->propertyIdentifier = PROP_PRESENT_VALUE;
  paValueList->propertyArrayIndex = BACNET_ARRAY_ALL;
  paValueList->value.context_specific = false;
  paValueList->value.tag = BACNET_APPLICATION_TAG_ENUMERATED;
  paValueList->value.next = NULL;
  paValueList->value.type.Enumerated = mPresentValue;
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