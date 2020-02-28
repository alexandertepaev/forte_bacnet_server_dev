#include "bacnet_object_config_fb.h"

const char* const CBacnetObjectConfigFB::scmOK = "OK";

CBacnetObjectConfigFB::CBacnetObjectConfigFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) : forte::core::io::IOConfigFBBase(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData) {}


CBacnetObjectConfigFB::~CBacnetObjectConfigFB(){}

void CBacnetObjectConfigFB::executeEvent(int pa_nEIID) {
  if(BACnetAdapterIn().INIT() == pa_nEIID && QI()) {

    if(init()) {
      QO() = true;
      STATUS() = scmOK;
      setEventChainExecutor(m_poInvokingExecEnv);
    }


    if(BACnetAdapterOut().getPeer() == 0) {
      sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
    } else {
      // forward init
      sendAdapterEvent(scm_nBACnetAdapterOutAdpNum, FORTE_BACnetAdapter::scm_nEventINITID);
    }
  } else if (BACnetAdapterOut().INITO() == pa_nEIID && QI()) {
    sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
  }
}