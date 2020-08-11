#ifndef _BACNET_AO_OBJCET_H_
#define _BACNET_AO_OBJCET_H_

#include "bacnet_object.h"
#include "bacnet_cov_reporting_object.h"

class CBacnetAnalogOutputObject : public CBacnetCOVReportingObject {
  public:
    CBacnetAnalogOutputObject(uint32_t paObjectID, float paPresentValue, bool paCOVReportingEnabled, float paCOVIncrement, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetAnalogOutputObject();
    int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
    float getPresentValue();
    void setPresentValue(float paValue);
    void encodeValueList(BACNET_PROPERTY_VALUE* value_list);
  protected:

  private:
    float mPresentValue;
    float mCOVIncrement;
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