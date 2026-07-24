cmake_minimum_required(VERSION 3.22)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
project(CompileTimeHAL LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/CompileTimeHal.cmake)
cthal_validate_toolchain()

add_subdirectory(CHAL)
add_subdirectory(CPPHAL)
add_subdirectory(CPPCTHAL)
add_subdirectory(CPPCTHAL23)
add_subdirectory(CubeMXHAL)
add_subdirectory(CubeMXLL)
add_subdirectory(NoHAL)

message(STATUS
  "Configured 7 STM32Cube firmware targets and 21 benchmark targets "
  "(7 implementations x 3 examples)."
)
