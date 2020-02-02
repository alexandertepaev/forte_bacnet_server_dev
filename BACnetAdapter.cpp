/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetAdapter
 *** Description: Adapter Interface
 *** Version: 
 *************************************************************************/

#include "BACnetAdapter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "BACnetAdapter_gen.cpp"
#endif

DEFINE_ADAPTER_TYPE(FORTE_BACnetAdapter, g_nStringIdBACnetAdapter)

const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anDataInputNames[] = {g_nStringIdQO};

const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anDataOutputNames[] = {g_nStringIdQI, g_nStringIdMasterId, g_nStringIdIndex};

const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT, g_nStringIdUINT};

const TDataIOID FORTE_BACnetAdapter::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_BACnetAdapter::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anEventInputNames[] = {g_nStringIdINITO};

const TDataIOID FORTE_BACnetAdapter::scm_anEOWith[] = {0, 1, 2, 255};
const TForteInt16 FORTE_BACnetAdapter::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_BACnetAdapter::scm_anEventOutputNames[] = {g_nStringIdINIT};

const SFBInterfaceSpec FORTE_BACnetAdapter::scm_stFBInterfaceSpecSocket = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const SFBInterfaceSpec FORTE_BACnetAdapter::scm_stFBInterfaceSpecPlug = {
  1,  scm_anEventOutputNames,  scm_anEOWith,  scm_anEOWithIndexes,
  1,  scm_anEventInputNames,  scm_anEIWith, scm_anEIWithIndexes,  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0, 0
};



