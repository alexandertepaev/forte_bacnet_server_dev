
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
#include <tuple>
#include "bacnet_device_object.h"

CBacnetDeviceObject::CBacnetDeviceObject(TForteUInt32 paObjectID, CBacnetObjectConfigFB *paConfigFB) : CBacnetObject(OBJECT_DEVICE, paObjectID, paConfigFB) {

}

CBacnetDeviceObject::~CBacnetDeviceObject() {
    
}

int CBacnetDeviceObject::readProperty(TForteUInt8 *paBuffer,  BACNET_PROPERTY_ID paProperty) {
  std::ignore = paBuffer; // supresses unused parameter warning
  std::ignore = paProperty; // supresses unused parameter warning
  return 0;
}

bool CBacnetDeviceObject::writeProperty(BACNET_APPLICATION_DATA_VALUE *paData, BACNET_PROPERTY_ID paProperty) {
  std::ignore = paData; // supresses unused parameter warning
  std::ignore = paProperty; // supresses unused parameter warning
  return false;
}