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

#include "bacnet_bout_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_bout_config_fb_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryOutput_gen.cpp"
#endif

#include "objects/bacnet_binary_output_object.h"
#include "bacnet_server_controller.h"
#include "bacnet_server_config_fb.h"

DEFINE_FIRMWARE_FB(CBacnetBinaryOutputConfigFB, g_nStringIdBACnetBinaryOutput)

const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdReversePolarity, g_nStringIdCOVReporting};

const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL};

const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdBOOL};

const TForteInt16 CBacnetBinaryOutputConfigFB::scm_anEIWithIndexes[] = {0};
const TDataIOID CBacnetBinaryOutputConfigFB::scm_anEIWith[] = {1, 0, 2, 255};
const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID CBacnetBinaryOutputConfigFB::scm_anEOWith[] = {0, 1, 255, 2, 255};
const TForteInt16 CBacnetBinaryOutputConfigFB::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId CBacnetBinaryOutputConfigFB::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef CBacnetBinaryOutputConfigFB::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec CBacnetBinaryOutputConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


CBacnetBinaryOutputConfigFB::CBacnetBinaryOutputConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData), mObject(NULL) {
}

CBacnetBinaryOutputConfigFB::~CBacnetBinaryOutputConfigFB(){
}

void CBacnetBinaryOutputConfigFB::executeEvent(int pa_nEIID){
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

const char *CBacnetBinaryOutputConfigFB::init() {
  // create new object
  mObject = new CBacnetBinaryOutputObject(ObjectID(), PresentValueInit(), COVReporting(), this);
  // get server controller and update its data structures
  CBacnetServerController *controller = CBacnetServerConfigFB::getServerController();
  if(controller == NULL)
    return scmInitFailed;
  controller->addObjectTableEntry(mObject);
  if(COVReporting() == true)
    controller->addCOVReportersEntry(mObject);
  // set PresentValueOut output
  updatePresentValueOutput(mObject->getPresentValue(), false);
  return 0;
}

void CBacnetBinaryOutputConfigFB::updatePresentValueOutput(bool paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}



