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

#ifndef _BACNETSERVER_H_
#define _BACNETSERVER_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_uint.h>
#include <forte_wstring.h>
#include "BACnetAdapter.h"

// #include"../../forte-incubation_1.11.0/src/core/io/configFB/io_base.h"
#include <core/io/configFB/io_base.h>

class CBacnetServerController;
class CBacnetDeviceObject;

class FORTE_BACnetServer: public forte::core::io::IOConfigFBBase {
  DECLARE_FIRMWARE_FB(FORTE_BACnetServer)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_UINT &Port() {
    return *static_cast<CIEC_UINT*>(getDI(1));
  };

  CIEC_UINT &DeviceID() {
    return *static_cast<CIEC_UINT*>(getDI(2));
  };

  CIEC_WSTRING &DeviceName() {
    return *static_cast<CIEC_WSTRING*>(getDI(3));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };

  static const TEventID scm_nEventINITID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINITOID = 0;
  static const TEventID scm_nEventINDID = 1;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_BACnetAdapter& BACnetAdapterOut() {
    return (*static_cast<FORTE_BACnetAdapter*>(m_apoAdapters[0]));
  };
  static const int scm_nBACnetAdapterOutAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(2, 4, 2, 1);

  void executeEvent(int pa_nEIID);

  static const char * const scmOK;
  static const char * const scmInitFailed;

  bool init();

  static CBacnetServerController *mController;

  CBacnetDeviceObject *mDeviceObject;

public:
  FORTE_BACnetServer(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  //FORTE_BACnetServer(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);

  ~FORTE_BACnetServer();

  static CBacnetServerController* getServerController();
};

#endif //close the ifdef sequence from the beginning of the file

