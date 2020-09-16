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
#ifndef _BACNET_DEVICE_OBJCET_H_
#define _BACNET_DEVICE_OBJCET_H_

#include "bacnet_object.h"

/*! @brief Concrete class representing BACnet Device Object
 *
 * This class is a minimalistic representation of BACnet Device Object.
 * Contains only mObjectType and mObjectID variables.
 */
class CBacnetDeviceObject : public CBacnetObject {
public:
  CBacnetDeviceObject(TForteUInt32 paObjectID, CBacnetObjectConfigFB *paConfigFB);
  ~CBacnetDeviceObject();

  /*! @brief Method for reading object's property
   *
   * Using this method a property of an object can be read.
   * The value of the desired property gets encoded into the buffer provided as a parameter.
   * TODO not implemented for device object
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

protected:

private:
};
#endif