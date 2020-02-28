#ifndef _BACNET_OBJECT_CONFIG_FB_H_
#define _BACNET_OBJECT_CONFIG_FB_H_

#include"../../forte-incubation_1.11.0/src/core/io/configFB/io_base.h"
#include "BACnetAdapter.h"


class CBacnetObjectConfigFB: public forte::core::io::IOConfigFBBase {
  public:
    CBacnetObjectConfigFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
    ~CBacnetObjectConfigFB();
  protected:
    void executeEvent(int pa_nEIID);
    virtual bool init() = 0;
  private:

    CIEC_BOOL &QI() {
      return *static_cast<CIEC_BOOL*>(getDI(0));
    };

    CIEC_BOOL &QO() {
      return *static_cast<CIEC_BOOL*>(getDO(0));
    };

    CIEC_WSTRING &STATUS() {
      return *static_cast<CIEC_WSTRING*>(getDO(1));
    };

    FORTE_BACnetAdapter& BACnetAdapterOut() {
    return (*static_cast<FORTE_BACnetAdapter*>(m_apoAdapters[0]));
    };
    static const int scm_nBACnetAdapterOutAdpNum = 0;
    
    FORTE_BACnetAdapter& BACnetAdapterIn() {
        return (*static_cast<FORTE_BACnetAdapter*>(m_apoAdapters[1]));
    };
    static const int scm_nBACnetAdapterInAdpNum = 1;

    static const char* const scmOK;
};
#endif