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

#ifndef _BACNETBINARYOUTPUT_H_
#define _BACNETBINARYOUTPUT_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_uint.h>
#include <forte_wstring.h>
#include "BACnetAdapter.h"

class FORTE_BACnetBinaryOutput: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_BACnetBinaryOutput)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_BOOL &QI() {
    return *static_cast<CIEC_BOOL*>(getDI(0));
  };

  CIEC_BOOL &PresentValueIn() {
    return *static_cast<CIEC_BOOL*>(getDI(1));
  };

  CIEC_BOOL &InService() {
    return *static_cast<CIEC_BOOL*>(getDI(2));
  };

  CIEC_UINT &ObjectID() {
    return *static_cast<CIEC_UINT*>(getDI(3));
  };

  CIEC_WSTRING &ObjectName() {
    return *static_cast<CIEC_WSTRING*>(getDI(4));
  };

  CIEC_BOOL &PresentValueInit() {
    return *static_cast<CIEC_BOOL*>(getDI(5));
  };

  CIEC_BOOL &ReversePolarity() {
    return *static_cast<CIEC_BOOL*>(getDI(6));
  };

  CIEC_BOOL &COVReporting() {
    return *static_cast<CIEC_BOOL*>(getDI(7));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_BOOL &QO() {
    return *static_cast<CIEC_BOOL*>(getDO(0));
  };

  CIEC_WSTRING &STATUS() {
    return *static_cast<CIEC_WSTRING*>(getDO(1));
  };

  CIEC_BOOL &PresentValueOut() {
    return *static_cast<CIEC_BOOL*>(getDO(2));
  };

  static const TEventID scm_nEventWRITE_PR_VALID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventINDID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_BACnetAdapter& BACnetAdapterOut() {
    return (*static_cast<FORTE_BACnetAdapter*>(m_apoAdapters[0]));
  };
  static const int scm_nBACnetAdapterOutAdpNum = 0;
  FORTE_BACnetAdapter& BACnetAdapterIn() {
    return (*static_cast<FORTE_BACnetAdapter*>(m_apoAdapters[1]));
  };
  static const int scm_nBACnetAdapterInAdpNum = 1;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 8, 3, 2);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_BACnetBinaryOutput){
  };

  virtual ~FORTE_BACnetBinaryOutput(){};

};

#endif //close the ifdef sequence from the beginning of the file
