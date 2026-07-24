#!/usr/bin/env bash
# Created on: 2026-07-22
# Author: Michael Trummer
# Copyright 2026 Michael Trummer
# SPDX-License-Identifier: Apache-2.0

set -euo pipefail

readonly ROOT="$(cd "$(dirname "$0")" && pwd)"
readonly OUT="$ROOT/MemoryAnalysis"
readonly OBJ="$OUT/isolated/obj"
readonly ELF="$OUT/isolated/elf"
readonly FULL_BUILD="$OUT/full-build"
readonly ARM_GCC_PREFIX="${ARM_GCC_PREFIX:-arm-none-eabi}"

readonly CC="${ARM_GCC_PREFIX}-gcc"
readonly CXX_COMPILER="${ARM_GCC_PREFIX}-g++"
readonly LD="${ARM_GCC_PREFIX}-ld"
readonly SIZE="${ARM_GCC_PREFIX}-size"
readonly NM="${ARM_GCC_PREFIX}-nm"
readonly OBJDUMP="${ARM_GCC_PREFIX}-objdump"

require_tool() {
  if ! command -v "$1" >/dev/null 2>&1; then
    echo "Required tool not found: $1" >&2
    exit 1
  fi
}

for tool in \
  "$CC" "$CXX_COMPILER" "$LD" "$SIZE" "$NM" "$OBJDUMP" \
  cmake python3 git; do
  require_tool "$tool"
done

rm -rf "$OUT"
mkdir -p "$OBJ" "$ELF"

cat >"$OUT/minimal.ld" <<'LDS'
/*
 * Generated minimal linker script for the benchmark-only ELF.
 * Created on: 2026-07-22
 * Author: Michael Trummer
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

ENTRY(main)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.text.main))
    *(.text .text.*)
  } > FLASH

  .rodata :
  {
    *(.rodata .rodata.*)
  } > FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > FLASH
  .init_array : { KEEP(*(.init_array .init_array.*)) } > FLASH
  .fini_array : { KEEP(*(.fini_array .fini_array.*)) } > FLASH

  .data :
  {
    *(.data .data.*)
  } > RAM AT > FLASH

  .bss (NOLOAD) :
  {
    *(.bss .bss.*)
    *(COMMON)
  } > RAM

  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
  }
}
LDS

readonly COMMON=(
  -mcpu=cortex-m4
  -mthumb
  -ffreestanding
  -fno-builtin
  -O3
  -fno-unroll-loops
  -DSTM32L433xx
  -ffunction-sections
  -fdata-sections
  -fstack-usage
  -fno-unwind-tables
  -fno-asynchronous-unwind-tables
  -c
)
readonly CXX_FLAGS=(
  -fno-exceptions
  -fno-rtti
  -fno-threadsafe-statics
)
readonly HAL_INC=(
  -I"$ROOT/CubeMXHAL"
  -I"$ROOT/CubeMXHAL/Inc"
  -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Include"
  -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
  -I"$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Inc"
)
readonly LL_INC=(
  -I"$ROOT/CubeMXLL"
  -I"$ROOT/CubeMXLL/Inc"
  -I"$ROOT/CubeMXLL/Drivers/CMSIS/Include"
  -I"$ROOT/CubeMXLL/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
  -I"$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Inc"
)

compile_c() {
  local output="$1"
  shift
  "$CC" "${COMMON[@]}" "$@" -o "$output"
}

compile_cpp() {
  local output="$1"
  local standard="$2"
  shift 2
  "$CXX_COMPILER" \
    "${COMMON[@]}" \
    "${CXX_FLAGS[@]}" \
    "-std=$standard" \
    "$@" \
    -o "$output"
}

link_isolated_elf() {
  local output="$1"
  shift
  "$LD" \
    -T "$OUT/minimal.ld" \
    -e main \
    --gc-sections \
    "-Map=${output%.elf}.map" \
    -o "$output" \
    "$@"
}

echo "Building 14 isolated benchmark ELFs ..."
for example in Single MultiCall; do
  readonly_source="$ROOT/Examples/$example"
  example_key="${example,,}"

  for implementation in \
    nohal chal cpphal cppcthal cppcthal23 cubemxhal cubemxll; do
    object_dir="$OBJ/$example_key/$implementation"
    elf_dir="$ELF/$example_key"
    mkdir -p "$object_dir" "$elf_dir"
    objects=()

    case "$implementation" in
      nohal)
        compile_c \
          "$object_dir/main.o" \
          -I"$ROOT/NoHAL" \
          -I"$ROOT/NoHAL/Src" \
          "$readonly_source/nohal.c"
        objects+=("$object_dir/main.o")
        ;;
      chal)
        compile_c \
          "$object_dir/main.o" \
          -I"$ROOT/CHAL" \
          "$readonly_source/chal.c"
        objects+=("$object_dir/main.o")
        for unit in pin port rcc; do
          compile_c \
            "$object_dir/$unit.o" \
            -I"$ROOT/CHAL" \
            "$ROOT/CHAL/csl/src/$unit.c"
          objects+=("$object_dir/$unit.o")
        done
        ;;
      cpphal)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++17 \
          -I"$ROOT/CPPHAL" \
          "$readonly_source/cpphal.cpp"
        objects+=("$object_dir/main.o")
        for unit in Pin Port RCC; do
          compile_cpp \
            "$object_dir/$unit.o" \
            gnu++17 \
            -I"$ROOT/CPPHAL" \
            "$ROOT/CPPHAL/csl/src/$unit.cpp"
          objects+=("$object_dir/$unit.o")
        done
        ;;
      cppcthal)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++17 \
          -I"$ROOT/CPPCTHAL" \
          "$readonly_source/cppcthal.cpp"
        objects+=("$object_dir/main.o")
        ;;
      cppcthal23)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++23 \
          -I"$ROOT/CPPCTHAL23" \
          "$readonly_source/cppcthal23.cpp"
        objects+=("$object_dir/main.o")
        ;;
      cubemxhal)
        compile_c \
          "$object_dir/main.o" \
          -DUSE_HAL_DRIVER \
          "${HAL_INC[@]}" \
          "$readonly_source/cubemx-hal.c"
        compile_c \
          "$object_dir/gpio.o" \
          -DUSE_HAL_DRIVER \
          "${HAL_INC[@]}" \
          "$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c"
        objects+=("$object_dir/main.o" "$object_dir/gpio.o")
        ;;
      cubemxll)
        compile_c \
          "$object_dir/main.o" \
          -DUSE_FULL_LL_DRIVER \
          "${LL_INC[@]}" \
          "$readonly_source/cubemx-ll.c"
        compile_c \
          "$object_dir/gpio.o" \
          -DUSE_FULL_LL_DRIVER \
          "${LL_INC[@]}" \
          "$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c"
        objects+=("$object_dir/main.o" "$object_dir/gpio.o")
        ;;
    esac

    link_isolated_elf \
      "$elf_dir/$implementation.elf" \
      "${objects[@]}"
  done
done

"$CC" --version >"$OUT/compiler.txt"

echo "Building 14 complete STM32 firmware ELFs ..."
cmake \
  -S "$ROOT" \
  -B "$FULL_BUILD" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE="$ROOT/cmake/toolchains/arm-none-eabi-gcc.cmake"

full_targets=()
for example_key in single multicall; do
  for implementation in \
    nohal chal cpphal cppcthal cppcthal23 cubemxhal cubemxll; do
    full_targets+=("${implementation}_${example_key}")
  done
done
cmake \
  --build "$FULL_BUILD" \
  --parallel \
  --target "${full_targets[@]}"

echo "Extracting instruction, memory and section values ..."
python3 "$ROOT/MemoryAnalysisResults.py" \
  "$ROOT" \
  "$OUT" \
  "$FULL_BUILD" \
  "$SIZE" \
  "$NM" \
  "$OBJDUMP" \
  "$CC"

echo "Analysis completed successfully."
