# Benchmark examples

The MultiCall examples for the HAL variants use board-support-layer LED and
switch modules in the application code. The NoHAL MultiCall example
deliberately keeps the direct register access of the original Nucleo64HAL
implementation visible.

The `while (1)` sections perform the same work in every implementation: read a
switch input and drive its corresponding LED output.

- `Single`: one B1/LD4 pair.
- `MultiCall`: the B1/LD4 pair plus eight benchmark-only pairs mapped to
  different GPIO pins and configurations.

The `ex1` … `ex8` IDs used by the HAL variants are benchmark-only mappings.
They exercise different ports, pins and configurations; they do not describe
additional physical LEDs or switches on the Nucleo-64 board.
