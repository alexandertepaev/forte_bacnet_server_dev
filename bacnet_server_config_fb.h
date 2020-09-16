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
#ifndef _BACNETSERVER_H_
#define _BACNETSERVER_H_

#include <funcbloc.h>
#include <forte_bool.h>
#include <forte_uint.h>
#include <forte_wstring.h>
#include "BACnetAdapter.h"

#include <core/io/configFB/io_base.h>

class CBacnetServerController;
class CBacnetDeviceObject;

/*! @brief BACnet Server configuration FB class
 *
 * BACnet Server configuration FB class, which represents a FB standing in the head of the Server 
 * configuration daisy-chain.
 * The main purpose of Server COnfiguration FB is to gather all the data required by the BACnet 
 * server and to instantiate and configure the BACnet server controller.
 */
class CBacnetServerConfigFB: public forte::core::io::IOConfigFBBase {
  DECLARE_FIRMWARE_FB(CBacnetServerConfigFB)

public:
  CBacnetServerConfigFB(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes);

  ~CBacnetServerConfigFB();

  /*! @brief Returns a pointer to server controller
   *
   * This method returns a pointer to server controller instance.
   * Used by derivatives of CBacnetObjectConfigFB. 
   * 
   * @return Pointer to server controller instance
   */
  static CBacnetServerController* getServerController();

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
  
  /*! @brief Handles input events 
   *
   * This method represents an entry point of CServerConfigFB's execution.
   * Handle INIT(+), INIT(-), BACnetAdapterOut.INITO(+) events.
   * Calls init() function upon initialization, and server controller's initDone() after 
   * the initialization is finished
   * 
   * @param pa_nEIID ID of the triggered input event
   */
  void executeEvent(int pa_nEIID);

  //! STATUS messages
  constexpr static const char * const scmOK = "Initialized";
  constexpr static const char * const scmInitFailed = "Initialization failed";


  /*! @brief Initializes BACnet server controller
   *
   * This method is used for the initialization of the BACnet server controller.
   * The initialization is performed by creating the client controller and adding a 
   * new instance of BACnet Device object into its object list.
   * 
   * @return String containing error message, or 0 in case of success
   */
  const char* init();

  // FIXME: static -> same for all instances, rm static or create 
  // static list of server controllers and assign unique ID to each server controller -> in 
  // getServer controller look up for a specific server controller
  static CBacnetServerController *mController; //!< Static pointer to Server Controller instance

  CBacnetDeviceObject *mDeviceObject; //!< Pointer to BACnet Device Object instance


};

#endif
