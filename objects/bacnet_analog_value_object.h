#ifndef _BACNET_AV_OBJCET_H_
#define _BACNET_AV_OBJCET_H_

#include "bacnet_object.h"

class CBacnetAnalogValueObject : public CBacnetObject {
  public:
    CBacnetAnalogValueObject(uint32_t paObjectID);
    ~CBacnetAnalogValueObject();
  protected:
  private:
};

/*
  Required properties: Name - access specifier - Data Type

  Object_Identifier                 -     R     - BACnetObjectIdentifier
  Object_Name                       -     R     - CharacterString
  Object_Type                       -     R     - BACnetObjectType
  Present_Value                     -     W     - REAL
  Status_Flags                      -     R     - BACnetStatusFlags
  Event_State                       -     R     - BACnetEventState
  Out_Of_Service                    -     R     - BOOLEAN
  Units                             -     R     - BACnetEngineeringUnits
  Priority_Array                    -     R     - BACnetPriorityArray
  Relinquish_Default                -     R     - REAL
  COV_Increment                     -     R     - REAL
  Property_List                     -     R     - BACnetARRAY[N] of BACnetPropertyIdentifier
  
*/

#endif