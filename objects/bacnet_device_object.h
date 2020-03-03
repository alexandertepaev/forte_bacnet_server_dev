#ifndef _BACNET_DEVICE_OBJCET_H_
#define _BACNET_DEVICE_OBJCET_H_

#include "bacnet_object.h"

class CBacnetDeviceObject : public CBacnetObject {
  public:
    CBacnetDeviceObject(uint32_t paObjectID, forte::core::io::IOConfigFBBase *paConfigFB);
    ~CBacnetDeviceObject();
    int readProperty(uint8_t *buffer,  BACNET_PROPERTY_ID property);
    bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID property);
  protected:
  private:
};


/*
  Required properties: Name - access specifier - Data Type

  Object_Identifier                 -     R     - BACnetObjectIdentifier
  Object_Name                       -     R     - CharacterString
  Object_Type                       -     R     - BACnetObjectType
  System_Status                     -     R     - BACnetDeviceStatus
  Vendor_Name                       -     R     - CharacterString
  Vendor_Identifier                 -     R     - Unsigned16
  Model_Name                        -     R     - CharacterString
  Firmware_Revision                 -     R     - CharacterString
  Application_Software_Version      -     R     - CharacterString
  Protocol_Version                  -     R     - Unsigned
  Protocol_Revision                 -     R     - Unsigned
  Protocol_Services_Supported       -     R     - BACnetServicesSupported
  Protocol_Object_Types_Supported   -     R     - BACnetObjectTypesSupported
  Object_List                       -     R     - BACnetARRAY[N] of BACnetObjectIdentifier
  Max_APDU_Length_Accepted          -     R     - Unsigned
  Segmentation_Supported            -     R     - BACnetSegmentation
  APDU_Timeout                      -     R     - Unsigned
  Number_Of_APDU_Retries            -     R     - Unsigned
  Device_Address_Binding            -     R     - BACnetLIST of BACnetAddressBinding
  Database_Revision                 -     R     - Unsigned
  Property_List                     -     R     - BACnetARRAY[N] of BACnetPropertyIdentifier

*/

#endif