include_guard(GLOBAL)

include(CMakeParseArguments)

set(CTHAL_REPOSITORY_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")

option(
  CTHAL_ALLOW_HOST_COMPILER
  "Allow non-ARM compilers for configure and syntax checks"
  OFF
)

find_program(CTHAL_OBJCOPY NAMES arm-none-eabi-objcopy)
find_program(CTHAL_SIZE NAMES arm-none-eabi-size)

function(cthal_validate_toolchain)
  if(CTHAL_ALLOW_HOST_COMPILER)
    return()
  endif()

  if(NOT CMAKE_C_COMPILER MATCHES "arm-none-eabi" AND
     NOT CMAKE_CXX_COMPILER MATCHES "arm-none-eabi")
    message(FATAL_ERROR
      "This project must be configured with an ARM embedded toolchain. "
      "Use CMake preset 'arm-gcc-debug' or 'arm-gcc-release'. "
      "Current compilers: C='${CMAKE_C_COMPILER}', "
      "CXX='${CMAKE_CXX_COMPILER}'."
    )
  endif()
endfunction()

function(cthal_add_firmware target_name)
  set(options)
  set(one_value_args
    MODE
    MAIN_SOURCE
    STARTUP_SOURCE
    LINKER_SCRIPT
    CXX_STANDARD
  )
  set(multi_value_args
    SOURCES
    INCLUDE_DIRS
    DEFINES
  )
  cmake_parse_arguments(
    CTH
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )

  foreach(required_arg IN ITEMS MODE MAIN_SOURCE STARTUP_SOURCE LINKER_SCRIPT)
    if(NOT CTH_${required_arg})
      message(FATAL_ERROR
        "cthal_add_firmware(${target_name}) requires ${required_arg}"
      )
    endif()
  endforeach()

  if(NOT CTH_MODE STREQUAL "BENCHMARK" AND
     NOT CTH_MODE STREQUAL "STM32CUBE")
    message(FATAL_ERROR
      "cthal_add_firmware(${target_name}) MODE must be BENCHMARK or STM32CUBE"
    )
  endif()

  add_executable(${target_name}
    ${CTH_MAIN_SOURCE}
    ${CTH_STARTUP_SOURCE}
    ${CTH_SOURCES}
  )

  set_target_properties(${target_name} PROPERTIES
    SUFFIX ".elf"
    LINK_DEPENDS "${CTH_LINKER_SCRIPT}"
  )

  target_include_directories(${target_name} PRIVATE
    ${CTH_INCLUDE_DIRS}
  )

  target_compile_definitions(${target_name} PRIVATE
    STM32L433xx
    ${CTH_DEFINES}
  )

  target_compile_options(${target_name} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -ffunction-sections
    -fdata-sections
    -fno-common
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
  )

  target_link_options(${target_name} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -T${CTH_LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${target_name}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
  )

  if(CTH_MODE STREQUAL "BENCHMARK")
    target_compile_options(${target_name} PRIVATE -O3)
  else()
    target_compile_options(${target_name} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wno-unused-parameter
      $<$<CONFIG:Debug>:-O0>
      $<$<CONFIG:Debug>:-g3>
      $<$<CONFIG:Debug>:-ggdb>
      $<$<CONFIG:Release>:-Os>
    )

    target_compile_definitions(${target_name} PRIVATE
      RTE_DEVICE_STARTUP_STM32L4XX
      $<$<CONFIG:Debug>:DEBUG>
    )

    target_link_options(${target_name} PRIVATE
      --specs=nosys.specs
      -Wl,-z,max-page-size=8
    )
  endif()

  if(CTH_CXX_STANDARD)
    set_target_properties(${target_name} PROPERTIES
      CXX_STANDARD ${CTH_CXX_STANDARD}
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
    )
  endif()

  if(CTHAL_OBJCOPY)
    add_custom_command(TARGET ${target_name} POST_BUILD
      COMMAND ${CTHAL_OBJCOPY}
        -O ihex
        $<TARGET_FILE:${target_name}>
        $<TARGET_FILE_DIR:${target_name}>/${target_name}.hex
      COMMAND ${CTHAL_OBJCOPY}
        -O binary
        $<TARGET_FILE:${target_name}>
        $<TARGET_FILE_DIR:${target_name}>/${target_name}.bin
      VERBATIM
    )
  endif()

  if(CTHAL_SIZE)
    add_custom_command(TARGET ${target_name} POST_BUILD
      COMMAND ${CTHAL_SIZE} $<TARGET_FILE:${target_name}>
      VERBATIM
    )
  endif()
endfunction()

function(cthal_add_benchmarks)
  set(options)
  set(one_value_args
    TARGET_PREFIX
    SOURCE_BASENAME
    STARTUP_SOURCE
    LINKER_SCRIPT
    CXX_STANDARD
  )
  set(multi_value_args
    SOURCES
    INCLUDE_DIRS
    DEFINES
  )
  cmake_parse_arguments(
    CTH
    "${options}"
    "${one_value_args}"
    "${multi_value_args}"
    ${ARGN}
  )

  foreach(required_arg IN ITEMS
      TARGET_PREFIX
      SOURCE_BASENAME
      STARTUP_SOURCE
      LINKER_SCRIPT)
    if(NOT CTH_${required_arg})
      message(FATAL_ERROR
        "cthal_add_benchmarks requires ${required_arg}"
      )
    endif()
  endforeach()

  foreach(variant IN ITEMS Single MultiCall)
    string(TOLOWER "${variant}" variant_lower)

    cthal_add_firmware(${CTH_TARGET_PREFIX}_${variant_lower}
      MODE BENCHMARK
      MAIN_SOURCE
        "${CTHAL_REPOSITORY_ROOT}/Examples/${variant}/${CTH_SOURCE_BASENAME}"
      STARTUP_SOURCE "${CTH_STARTUP_SOURCE}"
      LINKER_SCRIPT "${CTH_LINKER_SCRIPT}"
      CXX_STANDARD "${CTH_CXX_STANDARD}"
      SOURCES ${CTH_SOURCES}
      INCLUDE_DIRS ${CTH_INCLUDE_DIRS}
      DEFINES ${CTH_DEFINES}
    )
  endforeach()
endfunction()
