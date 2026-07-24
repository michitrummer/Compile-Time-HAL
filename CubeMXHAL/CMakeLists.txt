cmake_minimum_required(VERSION 3.22)

set(CUBEMXHAL_STANDALONE OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(CUBEMXHAL_STANDALONE ON)
  if(NOT CMAKE_TOOLCHAIN_FILE)
    set(
      CMAKE_TOOLCHAIN_FILE
      "${CMAKE_CURRENT_LIST_DIR}/../cmake/toolchains/arm-none-eabi-gcc.cmake"
      CACHE FILEPATH "CMake toolchain file"
    )
  endif()
  project(CubeMXHAL LANGUAGES C ASM)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/CompileTimeHal.cmake")
cthal_validate_toolchain()

set(CUBEMXHAL_COMMON_SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/Src/syscalls.c"
  "${CMAKE_CURRENT_LIST_DIR}/Src/sysmem.c"
  "${CMAKE_CURRENT_LIST_DIR}/Src/system_stm32l4xx.c"
  "${CMAKE_CURRENT_LIST_DIR}/Src/stm32l4xx_hal_msp.c"
  "${CMAKE_CURRENT_LIST_DIR}/Src/stm32l4xx_it.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c"
)
set(CUBEMXHAL_INCLUDE_DIRS
  "${CMAKE_CURRENT_LIST_DIR}"
  "${CMAKE_CURRENT_LIST_DIR}/Inc"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/Include"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
  "${CMAKE_CURRENT_LIST_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc"
)
set(CUBEMXHAL_STARTUP
  "${CMAKE_CURRENT_LIST_DIR}/Src/startup_stm32l433xx.S"
)
set(CUBEMXHAL_LINKER_SCRIPT
  "${CTHAL_REPOSITORY_ROOT}/CPPCTHAL/stm32l433xc_flash.ld"
)

cthal_add_firmware(CubeMXHAL
  MODE STM32CUBE
  MAIN_SOURCE "${CMAKE_CURRENT_LIST_DIR}/Src/main.c"
  STARTUP_SOURCE "${CUBEMXHAL_STARTUP}"
  LINKER_SCRIPT "${CUBEMXHAL_LINKER_SCRIPT}"
  SOURCES ${CUBEMXHAL_COMMON_SOURCES}
  INCLUDE_DIRS ${CUBEMXHAL_INCLUDE_DIRS}
  DEFINES USE_HAL_DRIVER
)

if(NOT CUBEMXHAL_STANDALONE)
  cthal_add_benchmarks(
    TARGET_PREFIX cubemxhal
    SOURCE_BASENAME cubemx-hal.c
    STARTUP_SOURCE "${CUBEMXHAL_STARTUP}"
    LINKER_SCRIPT "${CUBEMXHAL_LINKER_SCRIPT}"
    SOURCES ${CUBEMXHAL_COMMON_SOURCES}
    INCLUDE_DIRS ${CUBEMXHAL_INCLUDE_DIRS}
    DEFINES USE_HAL_DRIVER
  )
endif()
