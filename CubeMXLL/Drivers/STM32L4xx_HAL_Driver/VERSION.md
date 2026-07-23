# STM32CubeL4 LL GPIO benchmark baseline

- STM32CubeL4 package tag: **v1.18.2**
- STM32L4 HAL/LL driver tag: **v1.13.6**
- Upstream LL GPIO header reference: `72bb307ed18c80e4ffa418c74c1ffc1427981a01`
- Upstream LL bus header reference: `55927f386de6942fdf72a8dfcadc3fd427fcc416`
- Upstream LL GPIO source reference: `c3dd30c8ff35b75ee51fb495c0e823d862fbc249`

The benchmark-relevant LL API and `LL_GPIO_Init` behavior were aligned with v1.13.6. The source extraction is focused on GPIO and bus-clock access; it is not a complete copy of every STM32CubeL4 driver module.
