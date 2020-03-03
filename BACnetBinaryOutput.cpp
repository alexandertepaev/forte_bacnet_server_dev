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

#include "BACnetBinaryOutput.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetBinaryOutput_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_BACnetBinaryOutput, g_nStringIdBACnetBinaryOutput)

const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPresentValueIn, g_nStringIdInService, g_nStringIdObjectID, g_nStringIdObjectName, g_nStringIdPresentValueInit, g_nStringIdReversePolarity, g_nStringIdCOVReporting};

const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdWSTRING, g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdPresentValueOut};

const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdBOOL};

const TForteInt16 FORTE_BACnetBinaryOutput::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_BACnetBinaryOutput::scm_anEIWith[] = {1, 0, 2, 255};
const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anEventInputNames[] = {g_nStringIdWRITE_PR_VAL};

const TDataIOID FORTE_BACnetBinaryOutput::scm_anEOWith[] = {0, 1, 255, 2, 255};
const TForteInt16 FORTE_BACnetBinaryOutput::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_BACnetBinaryOutput::scm_anEventOutputNames[] = {g_nStringIdIND};

const SAdapterInstanceDef FORTE_BACnetBinaryOutput::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true },
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterIn, false }};

const SFBInterfaceSpec FORTE_BACnetBinaryOutput::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  2,scm_astAdapterInstances};


void FORTE_BACnetBinaryOutput::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
//     case scm_nEventWRITE_PR_VALID:
// #error add code for WRITE_PR_VAL event!
// /*
//   do not forget to send output event, calling e.g.
//       sendOutputEvent(scm_nEventCNFID);
// */
//       break;
  }
}



