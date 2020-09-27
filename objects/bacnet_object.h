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
#ifndef _BACNET_OBJCET_H_
#define _BACNET_OBJCET_H_

#include "../forte_bacnet.h"
#include "../bacnet_object_config_fb.h"

class CBacnetObjectConfigFB;

/*! @brief Abstract class representing BACnet Object
 *
 * This class represents abstract BACnet object.
 * Every concrete BACnet object inherits from this class.
 * 
 */
class CBacnetObject {
friend class CBacnetServerController;
public:
  CBacnetObject(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectID, CBacnetObjectConfigFB *paObjectConfigFB);
  ~CBacnetObject();

  /*! @brief Virtual method for reading object's property
   *
   * Using this method a property of an object can be read.
   * The value of the desired property gets encoded into the buffer provided as a parameter.
   * 
   * @param paBuffer buffer, into which the value of the property is encoded 
   * @param paProperty enum, indicating ID of the property(see bacenum.h)
   * @return number of encoded bits
   */
  virtual int readProperty(TForteUInt8 *paBuffer,  BACNET_PROPERTY_ID paProperty) = 0;
  
  /*! @brief Virtual method for writing object's property
   *
   * Using this method a property of an object can be written.
   * The value of the desired property gets overwritten by the value provided as a parameter.
   * 
   * @param paData Struct with BACnet's application data  
   * @param paProperty enum, indicating ID of the property(see bacenum.h)
   * @return 'true' on success, 'false' otherwise
   */
  virtual bool writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID paProperty) = 0;
  
  //! returns configuration FB
  CBacnetObjectConfigFB *getConfigFB() {
    return mConfigFB;
  }

protected:
  BACNET_OBJECT_TYPE mObjectType; //!< enum, indicating type of the object (see bacenum.h)
  TForteUInt32 mObjectID; //!< Number, indicating ID of the object, together with mObjectType they build a device-wide unique pair
  CBacnetObjectConfigFB *mConfigFB; //!< Pointer to configuration FB of the object

private:
  
};

#endif