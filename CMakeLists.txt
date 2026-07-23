cmake_minimum_required(VERSION 3.22)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(CompileTimeHAL LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CTHAL_ALLOW_HOST_COMPILER "Allow non-ARM compilers (for syntax-only checks)" OFF)

if(NOT CTHAL_ALLOW_HOST_COMPILER)
  if(NOT CMAKE_C_COMPILER MATCHES "arm-none-eabi" AND
     NOT CMAKE_CXX_COMPILER MATCHES "arm-none-eabi")
    message(FATAL_ERROR
      "This project must be configured with an ARM embedded toolchain. "
      "Use CMake preset 'arm-gcc-debug' or 'arm-gcc-release' (for VS Code CMake Tools: enable presets and select one of these configure presets). "
      "Current compilers: C='${CMAKE_C_COMPILER}', CXX='${CMAKE_CXX_COMPILER}'.")
  endif()
endif()

find_program(CMAKE_OBJCOPY NAMES arm-none-eabi-objcopy)
find_program(CMAKE_SIZE NAMES arm-none-eabi-size)

set(CTHAL_CPU_FLAGS
  -mcpu=cortex-m4
  -mthumb
)

set(CTHAL_COMMON_COMPILE_FLAGS
  -O3
  -ffunction-sections
  -fdata-sections
  -fno-common
)

set(CTHAL_COMMON_LINK_FLAGS
  -Wl,--gc-sections
)

function(cthal_enable_embedded_flags target_name)
  target_compile_options(${target_name} PRIVATE
    ${CTHAL_CPU_FLAGS}
    ${CTHAL_COMMON_COMPILE_FLAGS}
  )

  target_link_options(${target_name} PRIVATE
    ${CTHAL_CPU_FLAGS}
    ${CTHAL_COMMON_LINK_FLAGS}
  )
endfunction()

function(cthal_add_example target_name)
  cmake_parse_arguments(CTH "" "IMPL_DIR;EXAMPLE_FILE;LINKER_SCRIPT;CXX_STANDARD" "EXTRA_SOURCES;INCLUDE_DIRS;DEFINES" ${ARGN})

  if(NOT CTH_IMPL_DIR)
    message(FATAL_ERROR "cthal_add_example(${target_name}) requires IMPL_DIR")
  endif()
  if(NOT CTH_EXAMPLE_FILE)
    message(FATAL_ERROR "cthal_add_example(${target_name}) requires EXAMPLE_FILE")
  endif()
  if(NOT CTH_LINKER_SCRIPT)
    message(FATAL_ERROR "cthal_add_example(${target_name}) requires LINKER_SCRIPT")
  endif()

  set(target_sources
    ${CTH_EXAMPLE_FILE}
    ${CTH_IMPL_DIR}/Startup/startup_stm32l433rctxp.s
    ${CTH_EXTRA_SOURCES}
  )

  if(EXISTS ${CTH_IMPL_DIR}/Src/syscalls.c)
    list(APPEND target_sources ${CTH_IMPL_DIR}/Src/syscalls.c)
  endif()

  if(EXISTS ${CTH_IMPL_DIR}/Src/sysmem.c)
    list(APPEND target_sources ${CTH_IMPL_DIR}/Src/sysmem.c)
  endif()

  add_executable(${target_name}
    ${target_sources}
  )

  target_include_directories(${target_name} PRIVATE
    ${CTH_IMPL_DIR}
    ${CTH_INCLUDE_DIRS}
  )

  target_compile_definitions(${target_name} PRIVATE
    STM32L433xx
    ${CTH_DEFINES}
  )

  target_link_options(${target_name} PRIVATE
    -T${CTH_LINKER_SCRIPT}
    -Wl,-Map=${target_name}.map
    -Wl,--print-memory-usage
  )

  cthal_enable_embedded_flags(${target_name})

  if(CTH_CXX_STANDARD)
    set_property(TARGET ${target_name} PROPERTY CXX_STANDARD ${CTH_CXX_STANDARD})
    set_property(TARGET ${target_name} PROPERTY CXX_STANDARD_REQUIRED ON)
    set_property(TARGET ${target_name} PROPERTY CXX_EXTENSIONS OFF)
  endif()

  if(CMAKE_OBJCOPY)
    add_custom_command(TARGET ${target_name} POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target_name}> $<TARGET_FILE_DIR:${target_name}>/${target_name}.hex
      COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target_name}> $<TARGET_FILE_DIR:${target_name}>/${target_name}.bin
      VERBATIM
    )
  endif()

  if(CMAKE_SIZE)
    add_custom_command(TARGET ${target_name} POST_BUILD
      COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target_name}>
      VERBATIM
    )
  endif()
endfunction()

set(CTHAL_EXAMPLE_VARIANTS
  Single
  MultiCall
  MultiExCall
)

foreach(variant IN LISTS CTHAL_EXAMPLE_VARIANTS)
  string(TOLOWER ${variant} variant_lc)

  cthal_add_example(chal_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CHAL
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/chal.c
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CHAL/STM32L433RCTXP_FLASH.ld
    EXTRA_SOURCES
      ${CMAKE_SOURCE_DIR}/CHAL/csl/src/pin.c
      ${CMAKE_SOURCE_DIR}/CHAL/csl/src/port.c
      ${CMAKE_SOURCE_DIR}/CHAL/csl/src/rcc.c
  )

  cthal_add_example(cppcthal_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CPPCTHAL
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/cppcthal.cpp
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CPPCTHAL/STM32L433RCTXP_FLASH.ld
    CXX_STANDARD 17
  )

  cthal_add_example(cpphal_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CPPHAL
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/cpphal.cpp
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CPPHAL/STM32L433RCTXP_FLASH.ld
    CXX_STANDARD 17
    EXTRA_SOURCES
      ${CMAKE_SOURCE_DIR}/CPPHAL/csl/src/Pin.cpp
      ${CMAKE_SOURCE_DIR}/CPPHAL/csl/src/Port.cpp
      ${CMAKE_SOURCE_DIR}/CPPHAL/csl/src/RCC.cpp
  )

  cthal_add_example(cthal23_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CT-HAL23
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/cthal23.cpp
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CT-HAL23/STM32L433RCTXP_FLASH.ld
    CXX_STANDARD 23
  )

  cthal_add_example(cubemxhal_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CubeMXHAL
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/cubemx-hal.c
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CubeMXHAL/STM32L433RCTXP_FLASH.ld
    DEFINES USE_HAL_DRIVER
    INCLUDE_DIRS
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Inc
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/CMSIS/Include
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/CMSIS/Device/ST/STM32L4xx/Include
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Inc
    EXTRA_SOURCES
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Src/system_stm32l4xx.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Src/stm32l4xx_hal_msp.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Src/stm32l4xx_it.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
      ${CMAKE_SOURCE_DIR}/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
  )

  cthal_add_example(cubemxll_${variant_lc}
    IMPL_DIR ${CMAKE_SOURCE_DIR}/CubeMXLL
    EXAMPLE_FILE ${CMAKE_SOURCE_DIR}/Examples/${variant}/cubemx-ll.c
    LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/CubeMXLL/STM32L433RCTXP_FLASH.ld
    DEFINES USE_FULL_LL_DRIVER
    INCLUDE_DIRS
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Inc
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Drivers/CMSIS/Include
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Drivers/CMSIS/Device/ST/STM32L4xx/Include
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Inc
    EXTRA_SOURCES
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Src/system_stm32l4xx.c
      ${CMAKE_SOURCE_DIR}/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c
  )
endforeach()

message(STATUS "Configured 18 benchmark firmware targets (6 HAL implementations x 3 examples).")
