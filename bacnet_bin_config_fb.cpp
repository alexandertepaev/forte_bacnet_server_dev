/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetBinaryOutput
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     1.0: 2020-03-03/root -  - 
 *************************************************************************/

#include "BACnetBinaryInput.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryInput_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryInput_gen.cpp"
#endif

#include "objects/bacnet_binary_input_object.h"
#include "bacnet_server_controller.h"
#include "BACnetServer.h"

DEFINE_FIRMWARE_FB(FORTE_BACnetBinaryInput, g_nStringIdBACnetBinaryInput)

const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdReversePolarity, g_nStringIdCOVReporting};

const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdBOOL};

const TForteInt16 FORTE_BACnetBinaryInput::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_BACnetBinaryInput::scm_anEIWith[] = {1, 0, 2, 255};
const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID FORTE_BACnetBinaryInput::scm_anEOWith[] = {0, 1, 255, 2, 255};
const TForteInt16 FORTE_BACnetBinaryInput::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_BACnetBinaryInput::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef FORTE_BACnetBinaryInput::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec FORTE_BACnetBinaryInput::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


FORTE_BACnetBinaryInput::FORTE_BACnetBinaryInput(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData), mObject(NULL) {
}

FORTE_BACnetBinaryInput::~FORTE_BACnetBinaryInput(){
}

void FORTE_BACnetBinaryInput::executeEvent(int pa_nEIID){

  if(pa_nEIID == cg_nExternalEventID) { 
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else if (pa_nEIID == scm_nEventWRITE_PR_VALID) {
    mObject->setOutOfService(!InService());
    if(InService()) { // if input is in service, accept changes to PresentValue
      mObject->setPresentValue(PresentValueIn());
      updatePresentValueOutput(mObject->getPresentValue(), true);
    }
  } else {
    CBacnetObjectConfigFB::executeEvent(pa_nEIID);
  }
}

bool FORTE_BACnetBinaryInput::init() {
  DEVLOG_DEBUG("[FORTE_BACnetBinaryInput] init(): initialising config fg\n");
  mObject = new CBacnetBinaryInputObject(ObjectID(), PresentValueInit(), ReversePolarity(), COVReporting(), this);

  CBacnetServerController *controller = FORTE_BACnetServer::getServerController();

  if(controller == NULL)
    return false;

  controller->addObjectTableEntry(mObject);

  if(COVReporting() == true)
    controller->addCOVReportersEntry(mObject);

  updatePresentValueOutput(mObject->getPresentValue(), false);

  return true;
}

void FORTE_BACnetBinaryInput::updatePresentValueOutput(bool paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}


// bool FORTE_BACnetBinaryInput::isInService() {
//   return InService();
// }

