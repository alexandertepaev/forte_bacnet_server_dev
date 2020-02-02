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

class FORTE_BACnetAdapter: public CAdapter{
  DECLARE_ADAPTER_TYPE(FORTE_BACnetAdapter)

private:
 public:
  static const TEventID scm_nEventINITOID = 0;
  int INITO() {
    return m_nParentAdapterListEventID + scm_nEventINITOID;
  }
 private:
  static const TForteInt16 scm_anEIWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

 public:
  static const TEventID scm_nEventINITID = 0;
  int INIT() {
    return m_nParentAdapterListEventID + scm_nEventINITID;
  }
 private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpecSocket;

  static const SFBInterfaceSpec scm_stFBInterfaceSpecPlug;

   FORTE_ADAPTER_DATA_ARRAY(1, 1, 0, 0, 0);

public:
  ADAPTER_CTOR(FORTE_BACnetAdapter){
  };

  virtual ~FORTE_BACnetAdapter(){};

};

#endif //close the ifdef sequence from the beginning of the file

