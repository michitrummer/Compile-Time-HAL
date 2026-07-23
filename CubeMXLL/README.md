# CubeMX LL benchmark variant

This variant mirrors the Cube HAL benchmark but uses the STM32Cube Low-Layer GPIO and bus APIs:

- `LL_AHB2_GRP1_EnableClock`
- `LL_GPIO_Init`
- `LL_GPIO_IsInputPinSet`
- `LL_GPIO_SetOutputPin`
- `LL_GPIO_ResetOutputPin`

Compile with `STM32L433xx` and `USE_FULL_LL_DRIVER` defined. The source deliberately repeats initialization and access nine times to preserve the original benchmark structure.
