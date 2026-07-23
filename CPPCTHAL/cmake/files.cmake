# Add sources to executable/library
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/stm32l433xc_flash.ld" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/stm32l433xc_flash.ld")
