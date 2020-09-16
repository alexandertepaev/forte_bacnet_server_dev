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
#include "bacnet_object.h"

CBacnetObject::CBacnetObject(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectID, CBacnetObjectConfigFB *paConfigFB) : mObjectType(paObjectType), mObjectID(paObjectID), mConfigFB(paConfigFB) {

}

CBacnetObject::~CBacnetObject() {
    
}