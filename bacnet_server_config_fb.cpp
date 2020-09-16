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

#include "bacnet_server_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_server_config_fb_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetServer_gen.cpp"
#endif

#include "objects/bacnet_device_object.h"
#include "bacnet_server_controller.h"

#include <core/resource.h>
#include <core/device.h>

CBacnetServerController *CBacnetServerConfigFB::mController = NULL;

DEFINE_FIRMWARE_FB(CBacnetServerConfigFB, g_nStringIdBACnetServer)

const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPort, g_nStringIdDeviceID, g_nStringIdDeviceName};

const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT, g_nStringIdWSTRING};

const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 CBacnetServerConfigFB::scm_anEIWithIndexes[] = {0};
const TDataIOID CBacnetServerConfigFB::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anEventInputNames[] = {g_nStringIdINIT};

const TDataIOID CBacnetServerConfigFB::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 CBacnetServerConfigFB::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId CBacnetServerConfigFB::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SAdapterInstanceDef CBacnetServerConfigFB::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true }};

const SFBInterfaceSpec CBacnetServerConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


CBacnetServerConfigFB::CBacnetServerConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 forte::core::io::IOConfigFBBase(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)
{
}

CBacnetServerConfigFB::~CBacnetServerConfigFB(){};

void CBacnetServerConfigFB::executeEvent(int pa_nEIID){
  if(pa_nEIID == scm_nEventINITID && true == QI()) {
    // INIT event
    DEVLOG_DEBUG("[CBacnetServerConfigFB] executeEvent(): INIT BACnet Server\n");
    const char *error = init();
    if(0 == error) {
      if (BACnetAdapterOut().getPeer() != 0) {
        // Pass the INIT event to the next config FB
        sendAdapterEvent(scm_nBACnetAdapterOutAdpNum, FORTE_BACnetAdapter::scm_nEventINITID);
      } else {
        // no chained FBs, only Who-Is/I-Am service requests are handled
        QO() = true;
        STATUS() = scmOK;
        sendOutputEvent(scm_nEventINITOID);
        mController->initDone();
      }  
    } else {
      QO() = false;
      STATUS() = error;
      sendOutputEvent(scm_nEventINITOID);
    }
  } else if(pa_nEIID == BACnetAdapterOut().INITO()) {
      // BACnetAdapterOut.INITO -> daisy-chained init done 
      QO() = true;
      STATUS() = scmOK;
      sendOutputEvent(scm_nEventINITOID);
      mController->initDone();
  }
}

const char* CBacnetServerConfigFB::init() {
  // create and init server controller
  mController = new CBacnetServerController(getResource().getDevice().getDeviceExecution());
  const char *error = mController->init(Port());
  
  if(error != 0)
    return error; // TODO deinit controller

  // Add new object into controller's obj table
  mDeviceObject = new CBacnetDeviceObject(DeviceID(), this);
  mController->addObjectTableEntry(mDeviceObject);
  
  return 0;
}

CBacnetServerController* CBacnetServerConfigFB::getServerController() {
  return mController;
}



