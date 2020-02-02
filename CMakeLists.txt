
forte_add_module(BACnet_Server_1.11dev "BACnet_Server_1.11dev")
SET(BACNET_LIB_DIR "" CACHE PATH "Path to Bacnet library directory ")

forte_add_sourcefile_hcpp(BACnetServer)
forte_add_sourcefile_hcpp(BACnetAdapter)

forte_add_include_directories( /home/adm1n/Desktop/bachelors_thesis/fordiac/ext_modules/bacnet_server_1.11_dev/bacnet-stack-0.8.6/include/)
forte_add_include_directories( /home/adm1n/Desktop/bachelors_thesis/fordiac/ext_modules/bacnet_client_1.11_dev/bacnet-stack-0.8.6/ports/linux/ )

forte_add_link_directories(${BACNET_LIB_DIR})
forte_add_link_library( libbacnet.a )