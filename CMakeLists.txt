#/*******************************************************************************
#* Copyright (c) 2020 Alexander Tepaev github.com/alexandertepaev
# * This program and the accompanying materials are made available under the
# * terms of the Eclipse Public License 2.0 which is available at
# * http://www.eclipse.org/legal/epl-2.0.
# *
# * SPDX-License-Identifier: EPL-2.0
# *
# * Contributors:
# *   Alexander Tepaev
# *******************************************************************************/


forte_add_module(BACnetServer "BACnetServer")

if(FORTE_MODULE_BACnetServer)

  ### BACnet Stack Library
  SET(BACNET_STACK_DIR "" CACHE PATH "Root directory of BACnet Stack library")
  if(EXISTS ${BACNET_STACK_DIR})
    forte_add_include_directories(${BACNET_STACK_DIR}/include)
    forte_add_include_directories(${BACNET_STACK_DIR}/ports/linux)
    forte_add_link_directories(${BACNET_STACK_DIR}/lib)
    forte_add_link_library( libbacnet.a )
  else()
    message(FATAL_ERROR "Provide correct path to BACnet Stack library root directory")
  endif()

  ### Config FBs+Adapter
  forte_add_sourcefile_hcpp(bacnet_server_config_fb)
  forte_add_sourcefile_hcpp(bacnet_server_controller)
  forte_add_sourcefile_hcpp(bacnet_object_config_fb)
  forte_add_sourcefile_hcpp(bacnet_aval_config_fb)
  forte_add_sourcefile_hcpp(bacnet_aout_config_fb)
  forte_add_sourcefile_hcpp(bacnet_ain_config_fb)
  forte_add_sourcefile_hcpp(bacnet_bval_config_fb)
  forte_add_sourcefile_hcpp(bacnet_bout_config_fb)
  forte_add_sourcefile_hcpp(bacnet_bin_config_fb)
  forte_add_sourcefile_hcpp(BACnetAdapter)

  ### BACnet objects
  forte_add_sourcefile_hcpp(objects/bacnet_object)
  forte_add_sourcefile_hcpp(objects/bacnet_cov_reporting_object)
  forte_add_sourcefile_hcpp(objects/bacnet_device_object)
  forte_add_sourcefile_hcpp(objects/bacnet_binary_value_object)
  forte_add_sourcefile_hcpp(objects/bacnet_binary_output_object)
  forte_add_sourcefile_hcpp(objects/bacnet_binary_input_object)
  forte_add_sourcefile_hcpp(objects/bacnet_analog_value_object)
  forte_add_sourcefile_hcpp(objects/bacnet_analog_output_object)
  forte_add_sourcefile_hcpp(objects/bacnet_analog_input_object)

  
endif()