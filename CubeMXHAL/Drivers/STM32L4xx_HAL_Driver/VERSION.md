# STM32CubeL4 HAL GPIO benchmark baseline

- STM32CubeL4 package tag: **v1.18.2**
- STM32L4 HAL/LL driver tag: **v1.13.6**
- Upstream GPIO source reference: `c56443318e1c927f19ddae5738bb6c561e143001`
- Upstream GPIO header reference: `aaa7b6dfd270907d98ebac2879776cb59a68ec09`

The compiled benchmark path implements the v1.13.6 `HAL_GPIO_Init`, `HAL_GPIO_ReadPin`, and `HAL_GPIO_WritePin` behavior. The surrounding directory originates from the older CubeMX project and is retained for device/project context; unused peripheral modules were not wholesale refreshed.
