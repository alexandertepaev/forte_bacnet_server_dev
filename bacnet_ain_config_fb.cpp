/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetAnalogInput
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     1.0: 2020-02-02/root -  - 
 *************************************************************************/

#include "bacnet_ain_config_fb.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "bacnet_ain_config_fb_gen.cpp"
#endif
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetAnalogInput_gen.cpp"
#endif

#include "objects/bacnet_analog_input_object.h"
#include "bacnet_server_controller.h"
#include "bacnet_server_config_fb.h"

DEFINE_FIRMWARE_FB(FORTE_BACnetAnalogInput, g_nStringIdBACnetAnalogInput)

const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdCOVReporting, g_nStringIdCOVIncrement};

const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdREAL, g_nStringIdBOOL, g_nStringIdREAL};

const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdREAL};

const TForteInt16 FORTE_BACnetAnalogInput::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_BACnetAnalogInput::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID FORTE_BACnetAnalogInput::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_BACnetAnalogInput::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_BACnetAnalogInput::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef FORTE_BACnetAnalogInput::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec FORTE_BACnetAnalogInput::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


FORTE_BACnetAnalogInput::FORTE_BACnetAnalogInput(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
CBacnetObjectConfigFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData) {
}

FORTE_BACnetAnalogInput::~FORTE_BACnetAnalogInput(){
}

void FORTE_BACnetAnalogInput::executeEvent(int pa_nEIID){
  if(pa_nEIID == cg_nExternalEventID) { 
    updatePresentValueOutput(mObject->getPresentValue(), true);
  } else if (pa_nEIID == scm_nEventWRITE_PR_VALID) {


    
    // foo(PresentValueOut());

    mObject->setPresentValue(PresentValueIn());
    updatePresentValueOutput(mObject->getPresentValue(), true);

  
  } else {
    CBacnetObjectConfigFB::executeEvent(pa_nEIID);
  }
}

bool FORTE_BACnetAnalogInput::init(){
  DEVLOG_DEBUG("[FORTE_BACnetAnalogInput] init(): initialising config fg\n");

  mObject = new CBACnetAnalogInputObject(ObjectID(), PresentValueInit(), COVReporting(), COVIncrement(), this);

  CBacnetServerController *controller = CBacnetServerConfigFB::getServerController();

  if(controller == NULL)
    return false;
  
  controller->addObjectTableEntry(mObject);

  if(COVReporting() == true)
    controller->addCOVReportersEntry(mObject);

  updatePresentValueOutput(mObject->getPresentValue(), false);

  return true;
}


void FORTE_BACnetAnalogInput::updatePresentValueOutput(float paValue, bool paFireIndEvent) {
   PresentValueOut() = paValue;
   if(paFireIndEvent)
    sendOutputEvent(scm_nEventINDID);
}
