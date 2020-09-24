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
#include "bacnet_aout_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_aout_config_fb_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetAnalogOutput_gen.cpp"
#endif

#include "objects/bacnet_analog_output_object.h"
#include "bacnet_server_controller.h"
#include "bacnet_server_config_fb.h"

DEFINE_FIRMWARE_FB(CBacnetAnalogOutputConfigFB, g_nStringIdBACnetAnalogOutput)

const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdCOVReporting, g_nStringIdCOVIncrement};

const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdREAL};

const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdREAL};

const TForteInt16 CBacnetAnalogOutputConfigFB::scm_anEIWithIndexes[] = {0};
const TDataIOID CBacnetAnalogOutputConfigFB::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID CBacnetAnalogOutputConfigFB::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 CBacnetAnalogOutputConfigFB::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId CBacnetAnalogOutputConfigFB::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef CBacnetAnalogOutputConfigFB::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec CBacnetAnalogOutputConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};

CBacnetAnalogOutputConfigFB::CBacnetAnalogOutputConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
}

CBacnetAnalogOutputConfigFB::~CBacnetAnalogOutputConfigFB(){
}

void CBacnetAnalogOutputConfigFB::executeEvent(int pa_nEIID){
  if(pa_nEIID == cg_nExternalEventID) { 
    QO() = true;
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else if (pa_nEIID == scm_nEventWRITE_PR_VALID) {
    mObject->setPresentValue(PresentValueIn());
    QO() = true;
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else {
    CBacnetObjectConfigFB::executeEvent(pa_nEIID);
  }
}

const char * CBacnetAnalogOutputConfigFB::init(){
  // create new object
  mObject = new CBacnetAnalogOutputObject(ObjectID(), PresentValueInit(), COVReporting(), COVIncrement(), this);
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


void CBacnetAnalogOutputConfigFB::updatePresentValueOutput(float paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}

