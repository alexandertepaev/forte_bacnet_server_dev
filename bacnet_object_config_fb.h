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
#ifndef _BACNET_OBJECT_CONFIG_FB_H_
#define _BACNET_OBJECT_CONFIG_FB_H_

// #include <core/io/configFB/io_base.h>
#include <esfb.h>
#include "BACnetAdapter.h"

/*! @brief Abstract base class for BACnet object config FBs
 *
 * This class represents an abstract class that should be used as a base class for specific object
 * configuration FBs. In particular, it defines pure virtual init() method, which any derived config FB
 * class should implement. In addition to that, it provides implementation of executeEvent method,
 * in which input events of a config fb are handled.
 */
// class CBacnetObjectConfigFB: public forte::core::io::IOConfigFBBase { // Replace IOConfigFBBase with CEventSourceFB
class CBacnetObjectConfigFB: public CEventSourceFB {
public:
  CBacnetObjectConfigFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId, TForteByte *paFBConnData, TForteByte *paFBVarsData);
  ~CBacnetObjectConfigFB();
protected:

  /*! @brief Handles input events 
   *
   * This method represents execution entry point of object configuration FBs.
   * Upon the initialization (incoming BACnetAdapterIn.INIT), calls init method and continues the propagation of BACnetAdapterOut.INIT event.
   * Upon reception of BACnetAdapterOut.INITO event, continues its backpropagation.
   * 
   * @param pa_nEIID ID of the triggered input event
   */
  void executeEvent(int pa_nEIID);
  
  /*! @brief Initializes BACnet object
   *
   * This method is responsible for the initialization of a BACnet object.
   * Should be implemented by derived classes
   * 
   * @return 'true' in success, 'false' otherwise
   */
  virtual const char * init() = 0;

  constexpr static const char* const scmInitOK = "Initialized"; //!< Initialization succeeded message
  constexpr static const char* const scmInitFailed = "Initialization Failed"; //!< Initialization failed message
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
};
#endif