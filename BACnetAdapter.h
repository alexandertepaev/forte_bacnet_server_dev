/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: BACnetAdapter
 *** Description: Adapter Interface
 *** Version: 
 *************************************************************************/

#ifndef _BACNETADAPTER_H_
#define _BACNETADAPTER_H_

#include <adapter.h>
#include <typelib.h>
#include <forte_bool.h>
#include <forte_uint.h>

class FORTE_BACnetAdapter: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_BACnetAdapter)

private:
 private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
 public:
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDI(0) : getDO(0));
  };

 private:
  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
 public:
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>((isSocket()) ? getDO(0) : getDI(0));
  };

  CIEC_UINT &MasterId() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(1) : getDI(1));
  };

  CIEC_UINT &Index() {
    return *static_cast<CIEC_UINT*>((isSocket()) ? getDO(2) : getDI(2));
  };

 public:
  static const TEventID scm_nEventINITOID = 0;
  int INITO() {
    return m_nParentAdapterListEventID + scm_nEventINITOID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventINITID = 0;
  int INIT() {
    return m_nParentAdapterListEventID + scm_nEventINITID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 1, 3, 0);

public:
  ADAPTER_CTOR(FORTE_BACnetAdapter){
  };

  virtual ~FORTE_BACnetAdapter(){};

};

#endif //close the ifdef sequence from the beginning of the file

