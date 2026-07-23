#!/usr/bin/env bash
# Created on: 2026-07-22
# Author: Michael Trummer
# Copyright 2026 Michael Trummer
# SPDX-License-Identifier: Apache-2.0

set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
OUT="$ROOT/MemoryAnalysis"
OBJ="$OUT/obj"
ELF="$OUT/elf"
rm -rf "$OUT"
mkdir -p "$OBJ" "$ELF"
cat > "$OUT/minimal.ld" <<'LDS'
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

COMMON=(-target arm-none-eabi -mcpu=cortex-m4 -mthumb -ffreestanding -fno-builtin -O3 -fno-unroll-loops -DSTM32L433xx -ffunction-sections -fdata-sections -fstack-usage -fno-unwind-tables -fno-asynchronous-unwind-tables -c)
CXX=(-fno-exceptions -fno-rtti -fno-threadsafe-statics -I"$ROOT/InstructionAnalysis/support")
HAL_INC=(-I"$ROOT/CubeMXHAL/Inc" -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Include" -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Inc")
LL_INC=(-I"$ROOT/CubeMXLL/Inc" -I"$ROOT/CubeMXLL/Drivers/CMSIS/Include" -I"$ROOT/CubeMXLL/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Inc")

compile_c() { local out=$1; shift; clang "${COMMON[@]}" "$@" -o "$out"; }
compile_cpp() { local out=$1; shift; clang++ "${COMMON[@]}" "${CXX[@]}" "$@" -o "$out"; }
link_elf() { local out=$1; local entry=$2; shift 2; ld.lld -flavor gnu -T "$OUT/minimal.ld" -e "$entry" --gc-sections -Map="${out%.elf}.map" -o "$out" "$@"; }

for example in Single MultiCall MultiExCall; do
  SRC="$ROOT/Examples/$example"
  for impl in nohal chal cpphal cppcthal cubemx-hal cubemx-ll; do
    DIR="$OBJ/$example/$impl"
    mkdir -p "$DIR"
    objs=()
    case "$impl" in
      nohal)
        compile_c "$DIR/main.o" -I"$ROOT" "$SRC/nohal.c"; objs+=("$DIR/main.o") ;;
      chal)
        compile_c "$DIR/main.o" -I"$ROOT/CHAL" "$SRC/chal.c"; objs+=("$DIR/main.o")
        for s in pin port rcc; do compile_c "$DIR/$s.o" -I"$ROOT/CHAL" "$ROOT/CHAL/csl/src/$s.c"; objs+=("$DIR/$s.o"); done ;;
      cpphal)
        compile_cpp "$DIR/main.o" -I"$ROOT/CPPHAL" "$SRC/cpphal.cpp"; objs+=("$DIR/main.o")
        for s in Pin Port RCC; do compile_cpp "$DIR/$s.o" -I"$ROOT/CPPHAL" "$ROOT/CPPHAL/csl/src/$s.cpp"; objs+=("$DIR/$s.o"); done ;;
      cppcthal)
        compile_cpp "$DIR/main.o" -I"$ROOT/CPPCTHAL" "$SRC/cppcthal.cpp"; objs+=("$DIR/main.o") ;;
      cubemx-hal)
        compile_c "$DIR/main.o" "${HAL_INC[@]}" "$SRC/cubemx-hal.c"; objs+=("$DIR/main.o")
        compile_c "$DIR/gpio.o" "${HAL_INC[@]}" "$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c"; objs+=("$DIR/gpio.o") ;;
      cubemx-ll)
        compile_c "$DIR/main.o" -DUSE_FULL_LL_DRIVER "${LL_INC[@]}" "$SRC/cubemx-ll.c"; objs+=("$DIR/main.o")
        compile_c "$DIR/gpio.o" -DUSE_FULL_LL_DRIVER "${LL_INC[@]}" "$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c"; objs+=("$DIR/gpio.o") ;;
    esac
    mkdir -p "$ELF/$example"
    entry=main; [[ "$impl" == cpphal || "$impl" == cppcthal ]] && entry=_Z4mainv
    link_elf "$ELF/$example/$impl.elf" "$entry" "${objs[@]}"
  done
done
clang --version > "$OUT/compiler.txt"
