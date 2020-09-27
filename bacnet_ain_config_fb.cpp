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
#include "bacnet_ain_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_ain_config_fb_gen.cpp"
#endif

#include "objects/bacnet_analog_input_object.h"
#include "bacnet_server_controller.h"
#include "bacnet_server_config_fb.h"

DEFINE_FIRMWARE_FB(CBacnetAnalogInputConfigFB, g_nStringIdBACnetAnalogInput)

const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdCOVReporting, g_nStringIdCOVIncrement};

const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdREAL};

const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdREAL};

const TForteInt16 CBacnetAnalogInputConfigFB::scm_anEIWithIndexes[] = {0};
const TDataIOID CBacnetAnalogInputConfigFB::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID CBacnetAnalogInputConfigFB::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 CBacnetAnalogInputConfigFB::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId CBacnetAnalogInputConfigFB::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef CBacnetAnalogInputConfigFB::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec CBacnetAnalogInputConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


CBacnetAnalogInputConfigFB::CBacnetAnalogInputConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
}

CBacnetAnalogInputConfigFB::~CBacnetAnalogInputConfigFB(){
}

void CBacnetAnalogInputConfigFB::executeEvent(int pa_nEIID){
  if(pa_nEIID == cg_nExternalEventID) {
    QO() = true;
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else if (pa_nEIID == scm_nEventWRITE_PR_VALID) {
    if(InService()) { // if input is in service, accept changes to PresentValue
      mObject->setPresentValue(PresentValueIn());
      QO() = true;
      updatePresentValueOutput(mObject->getPresentValue(), true);
    }
  } else {
    CBacnetObjectConfigFB::executeEvent(pa_nEIID);
  }
}

const char * CBacnetAnalogInputConfigFB::init(){
  // create new object
  mObject = new CBACnetAnalogInputObject(ObjectID(), PresentValueInit(), COVReporting(), COVIncrement(), this);
  // get server controller and update its data structures
  CBacnetServerController *controller = CBacnetServerConfigFB::getServerController(BACnetAdapterIn().ControllerID());
  if(controller == NULL)
    return scmInitFailed;
  controller->addObjectTableEntry(mObject);
  if(COVReporting() == true)
    controller->addCOVReportersEntry(mObject);
  // set PresentValueOut output
  updatePresentValueOutput(mObject->getPresentValue(), false);
  return 0;
}


void CBacnetAnalogInputConfigFB::updatePresentValueOutput(float paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}
