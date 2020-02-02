/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetServer
 *** Description: Service Interface Function Block Type
 *** Version: 
 ***     1.0: 2020-02-02/root - null - 
 *************************************************************************/

#include "BACnetServer.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetServer_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_BACnetServer, g_nStringIdBACnetServer)

const CStringDictionary::TStringId FORTE_BACnetServer::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPort, g_nStringIdDeviceID, g_nStringIdDeviceName};

const CStringDictionary::TStringId FORTE_BACnetServer::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_BACnetServer::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_BACnetServer::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_BACnetServer::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_BACnetServer::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_BACnetServer::scm_anEventInputNames[] = {g_nStringIdINIT};

const TDataIOID FORTE_BACnetServer::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_BACnetServer::scm_anEOWithIndexes[] = {0, 3, -1};
const CStringDictionary::TStringId FORTE_BACnetServer::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SAdapterInstanceDef FORTE_BACnetServer::scm_astAdapterInstances[] = {
{g_nStringIdBACnetAdapter, g_nStringIdBACnetAdapterOut, true }};

const SFBInterfaceSpec FORTE_BACnetServer::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,scm_astAdapterInstances};


void FORTE_BACnetServer::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventINITID:
      DEVLOG_DEBUG("Hello world!\n");
      break;
  }
}


