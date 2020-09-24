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
#include "bacnet_object_config_fb.h"

CBacnetObjectConfigFB::CBacnetObjectConfigFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData) : CEventSourceFB(paSrcRes, paInterfaceSpec, paInstanceNameId, paFBConnData, paFBVarsData) {}
 


CBacnetObjectConfigFB::~CBacnetObjectConfigFB(){}

void CBacnetObjectConfigFB::executeEvent(int pa_nEIID) {
  if(BACnetAdapterIn().INIT() == pa_nEIID && QI()) {

    const char *error = init();

    if(0 == error){
      QO() = true;
      STATUS() = scmInitOK;
      setEventChainExecutor(m_poInvokingExecEnv);
    } else {
      QO() = false;
      STATUS() = error;
      setEventChainExecutor(m_poInvokingExecEnv);
    }

    if(BACnetAdapterOut().getPeer() == 0) {
      // pass the INITO back
      sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
    } else {
      // forward init
      BACnetAdapterOut().ControllerID() = BACnetAdapterIn().ControllerID();
      sendAdapterEvent(scm_nBACnetAdapterOutAdpNum, FORTE_BACnetAdapter::scm_nEventINITID);
    }
  } else if (BACnetAdapterOut().INITO() == pa_nEIID && QI()) {
    sendAdapterEvent(scm_nBACnetAdapterInAdpNum, FORTE_BACnetAdapter::scm_nEventINITOID);
  }
}