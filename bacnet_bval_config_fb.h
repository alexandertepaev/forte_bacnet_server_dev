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

#ifndef _BACNETBINARYVALUE_H_
#define _BACNETBINARYVALUE_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_uint.h>
#include <forte_wstring.h>
#include "BACnetAdapter.h"

#include "bacnet_object_config_fb.h"

class CBacnetBinaryValueObject;

/*! @brief Binary Value BACnet object configuration FB
 * 
 * This class is a concrete class of the CBacnetObjectConfigFB and represents a Binary Value 
 * BACnet object configuration FB.
 * Creates an instance of CBacnetBinaryValueObject, sets its parameters and updates 
 * server controller's object table and COVReporters list in case the object is configured to send
 * COV notifications.
 * Overrides executeEvent and init methods of the base class.
 * Implements updatePresentValueOutput method.  
 * 
 */
class CBacnetBinaryValueConfigFB: public CBacnetObjectConfigFB{
  DECLARE_FIRMWARE_FB(CBacnetBinaryValueConfigFB)

public:
  CBacnetBinaryValueConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);
  ~CBacnetBinaryValueConfigFB();


protected:

  /*! @brief Initializes BACnet Binary Value object
   *
   * This method is responsible for the initialization of the a BACnet Binary Value.
   * In particular, it creates an instance of CBacnetBinaryValueObject object, pushes it into the server controller's 
   * object table, and, if needed, updates server controller's COVReporters list. 
   * After these steps, updates present value output of the configuration fb by calling updatePresentValueOutput method.
   * 
   * @return 0 on success, string containing error message otherwise
   */
  const char * init();

  CBacnetBinaryValueObject *mObject; //!< Pointer to the CBacnetBinaryValueObject created in init method

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

  CIEC_BOOL &COVReporting() {
    return *static_cast<CIEC_BOOL*>(getDI(6));
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

   FORTE_FB_DATA_ARRAY(1, 7, 3, 2);

  /*! @brief Handles input events 
   *
   * This method represents execution entry point of BACnet Binary Value Object configuration FB.
   * Handles external (cg_nExternalEventID) and write present value (scm_nEventWRITE_PR_VALID) events.
   * In other cases, passes the control to CBacnetObjectConfigFB class (calls its executeEvent).
   * External event means, that the Present Value property of the underlying object was updated using BACnet 
   * WriteProperty service request.
   * scm_nEventWRITE_PR_VALID event means, that the present value property was updated from within the IEC 61499
   * application.
   * In both cases triggers IND output event.
   * 
   * @param pa_nEIID ID of the triggered input event
   */
  void executeEvent(int pa_nEIID);

  /*! @brief Updates PresentValueOut output of the configuration FB
   *
   * This method is used for updating the PresentValueOut output.
   * 
   * @param paValue New value of the output
   * @param paFireIndEvent Specifies, if the IND event has to be fired after updating the output
   */
  void updatePresentValueOutput(bool paValue, bool paFireIndEvent);
};

#endif

