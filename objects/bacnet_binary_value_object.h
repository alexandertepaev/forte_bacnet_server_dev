#ifndef _BACNET_BV_OBJCET_H_
#define _BACNET_BV_OBJCET_H_

#include "bacnet_object.h"

class CBacnetBinaryValueObject : public CBacnetObject {
  public:
    CBacnetBinaryValueObject(uint32_t paObjectID, bool paPresentValue, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetBinaryValueObject();
    int encodeApplicationData(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    int writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
    bool getPresentValue();
    void setPresentValue(bool paValue);
  protected:

  private:
    bool mPresentValue;

};

/*
  Required properties: Name - access specifier - Data Type

  Object_Identifier                 -     R     - BACnetObjectIdentifier
  Object_Name                       -     R     - CharacterString
  Object_Type                       -     R     - BACnetObjectType
  Present_Value                     -     W     - BACnetBinaryPV
  Status_Flags                      -     R     - BACnetStatusFlags
  Event_State                       -     R     - BACnetEventState
  Out_Of_Service                    -     R     - BOOLEAN
  Priority_Array                    -     R     - BACnetPriorityArray
  Relinquish_Default                -     R     - REAL
  Property_List                     -     R     - BACnetARRAY[N] of BACnetPropertyIdentifier
*/

#endif

