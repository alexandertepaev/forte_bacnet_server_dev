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

#include "objects/bacnet_device_object.h"
#include "bacnet_server_controller.h"


// #include "../../forte-incubation_1.11.0/src/core/resource.h"
// #include "../../forte-incubation_1.11.0/src/core/device.h"
#include <core/resource.h>
#include <core/device.h>

const char * const FORTE_BACnetServer::scmInitFailed = "Initialization failed";

CBacnetServerController *FORTE_BACnetServer::mController = NULL;

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


// FORTE_BACnetServer::FORTE_BACnetServer(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
//   CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData), mController(NULL) {

//  }

FORTE_BACnetServer::FORTE_BACnetServer(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) : \
 forte::core::io::IOConfigFBBase(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, m_anFBConnData, m_anFBVarsData)
{
}

FORTE_BACnetServer::~FORTE_BACnetServer(){};

void FORTE_BACnetServer::executeEvent(int pa_nEIID){
  if(pa_nEIID == scm_nEventINITID && QI() == true) {
    
    bool nError = init();
    if(nError) {
      if (BACnetAdapterOut().getPeer() == 0) {
        //TODO no chained FBs - start controller and respond only to who-is/i-am
        QO() = true;
        sendOutputEvent(scm_nEventINITOID);
        return;
      }
      sendAdapterEvent(scm_nBACnetAdapterOutAdpNum, FORTE_BACnetAdapter::scm_nEventINITID);    
    } else {
      QO() = false;
      STATUS() = scmInitFailed;
      sendOutputEvent(scm_nEventINITOID);
    }

  } else if(pa_nEIID == BACnetAdapterOut().INITO()) {
      DEVLOG_DEBUG("[FORTE_BACnetServer] executeEvent(): AdapterOut INITO event\n");
      QO() = true;
      sendOutputEvent(scm_nEventINITOID);
      mController->initDone();
  }
}

bool FORTE_BACnetServer::init() {
  
  mController = new CBacnetServerController(getResource().getDevice().getDeviceExecution());
  
  if(!mController->init(Port()))
    return false;

  mDeviceObject = new CBacnetDeviceObject(DeviceID(), this);

  mController->addObjectTableEntry(mDeviceObject);
  
  return true;
}

CBacnetServerController* FORTE_BACnetServer::getServerController() {
  return mController;
}



