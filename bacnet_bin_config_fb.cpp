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
#include "bacnet_bin_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_bin_config_fb_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryInput_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryInput_gen.cpp"
#endif

#include "objects/bacnet_binary_input_object.h"
#include "bacnet_server_controller.h"
#include "bacnet_server_config_fb.h"

DEFINE_FIRMWARE_FB(CBacnetBinaryInputConfigFB, g_nStringIdBACnetBinaryInput)

const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdReversePolarity, g_nStringIdCOVReporting};

const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL};

const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdBOOL};

const TForteInt16 CBacnetBinaryInputConfigFB::scm_anEIWithIndexes[] = {0};
const TDataIOID CBacnetBinaryInputConfigFB::scm_anEIWith[] = {1, 0, 2, 255};
const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID CBacnetBinaryInputConfigFB::scm_anEOWith[] = {0, 1, 255, 2, 255};
const TForteInt16 CBacnetBinaryInputConfigFB::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId CBacnetBinaryInputConfigFB::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef CBacnetBinaryInputConfigFB::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec CBacnetBinaryInputConfigFB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


CBacnetBinaryInputConfigFB::CBacnetBinaryInputConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData), mObject(NULL) {
}

CBacnetBinaryInputConfigFB::~CBacnetBinaryInputConfigFB(){
}

void CBacnetBinaryInputConfigFB::executeEvent(int pa_nEIID){
  if(pa_nEIID == cg_nExternalEventID) { 
    QO() = true;
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else if (pa_nEIID == scm_nEventWRITE_PR_VALID) {
    mObject->setOutOfService(!InService());
    if(InService()) { // if input is in service, accept changes to PresentValue
      mObject->setPresentValue(PresentValueIn());
      QO() = true;
      updatePresentValueOutput(mObject->getPresentValue(), true);
    }
  } else {
    CBacnetObjectConfigFB::executeEvent(pa_nEIID);
  }
}

const char *CBacnetBinaryInputConfigFB::init() {
  // create new object
  mObject = new CBacnetBinaryInputObject(ObjectID(), PresentValueInit(), ReversePolarity(), COVReporting(), this);
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

void CBacnetBinaryInputConfigFB::updatePresentValueOutput(bool paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}

