#ifndef _BACNET_BV_OBJCET_H_
#define _BACNET_BV_OBJCET_H_

#include "bacnet_cov_reporting_object.h"

// class CBacnetBinaryValueObject : public CBacnetObject, public CBacnetCOVReportingObject {

class CBacnetBinaryValueObject : public CBacnetCOVReportingObject { 
  public:
    CBacnetBinaryValueObject(uint32_t paObjectID, bool paPresentValue, bool paCOVReportingEnabled, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetBinaryValueObject();
    int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
    bool getPresentValue();
    void setPresentValue(bool paValue);
    void encodeValueList(BACNET_PROPERTY_VALUE* value_list);
  protected:

  private:
    bool mPresentValue;

    // in case OutOfService is required
    // bool mOutOfService;

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

