/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetBinaryValue
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     1.0: 2020-02-02/root -  - 
 *************************************************************************/

#include "BACnetBinaryValue.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryValue_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_BACnetBinaryValue, g_nStringIdBACnetBinaryValue)

const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdCOVReporting};

const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdBOOL, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdBOOL};

const TForteInt16 FORTE_BACnetBinaryValue::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_BACnetBinaryValue::scm_anEIWith[] = {0, 2, 1, 255};
const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID FORTE_BACnetBinaryValue::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_BACnetBinaryValue::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_BACnetBinaryValue::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef FORTE_BACnetBinaryValue::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec FORTE_BACnetBinaryValue::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  7,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


void FORTE_BACnetBinaryValue::executeEvent(int pa_nEIID){
 if(BACnetAdapterIn().INIT() == pa_nEIID) {
    DEVLOG_DEBUG("[FORTE_BACnetBinaryValue] executeEvent(): AdapterIn init event\n");
    if(BACnetAdapterOut().getPeer() == 0) {
      sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
    } else {
      // forward init
      sendAdapterEvent(scm_nBACnetAdapterOutAdpNum, FORTE_BACnetAdapter::scm_nEventINITID);
    }
 } else if (BACnetAdapterOut().INITO() == pa_nEIID) {
   sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
 }
}



