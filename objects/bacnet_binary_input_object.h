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
#ifndef _BACNET_BI_OBJCET_H_
#define _BACNET_BI_OBJCET_H_

#include "bacnet_cov_reporting_object.h"

/*! @brief Concrete class representing BACnet Binary Input Object
 *
 * This class is a minimalistic representation of BACnet Binary Input Object.
 * Inherits from CBacnetCOVReportingObject.
 */
class CBacnetBinaryInputObject : public CBacnetCOVReportingObject
{
public:
  CBacnetBinaryInputObject(TForteUInt32 paObjectID, bool paPresentValue, bool paReversePolarity, bool paCOVReportingEnabled, CBacnetObjectConfigFB *paConfigFB);
  ~CBacnetBinaryInputObject();

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
  bool getPresentValue();
  
  /*! @brief Getter method for setting the value of the mPresentValue property
   *
   * @param paValue Value to write into the mPresentValue member variable
   */
  void setPresentValue(bool paValue);

  /*! @brief Method for setting the fields of the applicaiton data value list that is sent out in a COV notification.
   * 
   * @param paValueList Application Data Value List
   */
  void encodeValueList(BACNET_PROPERTY_VALUE* paValueList);

  //! Tells if the polarity of PresentValue property is reversed (0 means high/on, 1 means low/off)
  bool isPolarityReversed();

  //! Tells if the input is out of service (current state of the mPresentValue is not coupled with actual input)
  void setOutOfService(bool paValue); 
private:
  bool mPresentValue; // current state of the input
  bool mReversePolarity; // descriptive property which tells if the input is low-active (logical 0 on the input means that the input is active) TODO: should we also change the behaviour of PresentValueOut() ?
  bool mOutOfService; // tells if the input is out of service. If so, present value property can be changed using WriteProperty service requests, if not all WriteProperty requests to the present value should be ignored // TODO: change mOutOfService when triggering WRITE_PR_VAL event

};

#endif