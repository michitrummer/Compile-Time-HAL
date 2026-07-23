# Benchmark examples

All examples use the board-support-layer LED and switch modules in the application code.
The `while (1)` sections therefore have the same conceptual form across implementations:
read a `Switch` module and drive a `Led` module.

- `Single`: one B1/LD4 pair.
- `MultiCall`: nine instances of the same B1/LD4 module types.
- `MultiExCall`: nine distinct benchmark LED/switch IDs mapped to different GPIO pins and configurations.

The `ex1` … `ex8` IDs are benchmark-only mappings used to exercise different ports, pins and configurations; they do not describe additional physical LEDs or switches on the Nucleo-64 board.
