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
#ifndef _BACNET_COV_REPORTING_OBJECT_H_
#define _BACNET_COV_REPORTING_OBJECT_H_

#include "bacnet_object.h"

#include <cmath> // TODO - check 

/*! @brief Abstract class representing BACnet Objects that can issue COV notifications
 *
 * This abstract class represents all BACnet objects that can be configured to issue COV notifications.
 * Containt COV specific flags and methods.
 */
class CBacnetCOVReportingObject: public CBacnetObject {
public:
  CBacnetCOVReportingObject(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectID, bool paCOVReportingEnabled, CBacnetObjectConfigFB *paObjectConfigFB); 
  ~CBacnetCOVReportingObject();

  /*! @brief Method for checking if COV condition is set to true
   *
   * @return 'true' if mCOVCondition is set, 'false' if cleared 
   */
  bool COVCondition();
  
  //! Method for setting the mCOVCondition flag
  void setCOVCondition();
  
  //! Method for clearing the mCOVCondition flag
  void clearCOVCondition();

  /*! @brief Method for checking if the object is configured to issue COV notifications
   *
   * @return 'true' if the object is configure to issu COV notifications, 'false' otherwise
   */
  bool COVReportingEnabled();

  /*! @brief Abstract method for setting the fields of the applicaiton data value list that is sent out in a COV notification.
   * 
   * Analog/Binary objects send out values of 2 propertie in a COV notification - PresentValue and StatusFlags
   * 
   * @param paValueList Application Data Value List
   */
  virtual void encodeValueList(BACNET_PROPERTY_VALUE* paValueList) = 0;

private:
  bool mCOVCondition; //!< COV condition member, set to 'true' if COV condition holds and a COV notificaiton has to be issued
  bool mCOVReportingEnabled; //!< Flag indicating if a COV reporting feature of an object is enabled 
};

#endif // _BACNET_COV_REPORTING_OBJECT_H_
