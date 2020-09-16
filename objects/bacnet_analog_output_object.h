#ifndef _BACNET_AO_OBJCET_H_
#define _BACNET_AO_OBJCET_H_

#include "bacnet_object.h"
#include "bacnet_cov_reporting_object.h"

/*! @brief Concrete class representing BACnet Analog Output Object
 *
 * This class is a minimalistic representation of BACnet Binary Value Object.
 * Inherits from CBacnetCOVReportingObject.
 */
class CBacnetAnalogOutputObject : public CBacnetCOVReportingObject {
public:
  CBacnetAnalogOutputObject(TForteUInt32 paObjectID, float paPresentValue, bool paCOVReportingEnabled, float paCOVIncrement, CBacnetObjectConfigFB *paConfigFB);
  ~CBacnetAnalogOutputObject();
  /*! @brief Method for reading object's property
   *
   * Using this method a property of an object can be read.
   * The value of the desired property gets encoded into the buffer provided as a parameter.
   * 
   * @param paBuffer buffer, into which the value of the property is encoded 
   * @param paProperty enum, indicating ID of the property(see bacenum.h)
   * @return number of encoded bits
   * 
   */
  int readProperty(TForteUInt8 *paBuffer,  BACNET_PROPERTY_ID paProperty);

  /*! @brief Method for writing object's property
   *
   * Using this method a property of an object can be written.
   * The value of the desired property gets overwritten by the value provided as a parameter.
   * * TODO not implemented for device object
   * 
   * @param paData Struct with BACnet's application data  
   * @param paProperty enum, indicating ID of the property(see bacenum.h)
   * @return 'true' on success, 'false' otherwise
   */
  bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID paProperty);

  /*! @brief Getter method for reading the value of the mPresentValue property
   *
   * @return Value of mPresentValue member
   */
  float getPresentValue();

  /*! @brief Getter method for setting the value of the mPresentValue property
   *
   * @param paValue Value to write into the mPresentValue member variable
   */
  void setPresentValue(float paValue);

  /*! @brief Method for setting the fields of the applicaiton data value list that is sent out in a COV notification.
   * 
   * @param paValueList Application Data Value List
   */
  void encodeValueList(BACNET_PROPERTY_VALUE* paValueList);

protected:

private:
  float mPresentValue; //!< Present Value property
  float mCOVIncrement; //!< COVIncrement property indicating a change in Present Value property, which should trigger a COV notification
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