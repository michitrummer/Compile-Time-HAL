/**
 * STM32Cube LL GPIO benchmark.
 * STM32CubeL4 v1.18.2 / STM32L4 LL driver v1.13.6.
 */
#include "main.h"

#define INIT_GPIO_PAIR() do {                                      \
  GPIO_InitStruct.Pin = B1_Pin;                                    \
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;                       \
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;                  \
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;            \
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                          \
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;                        \
  LL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);                     \
  GPIO_InitStruct.Pin = LD4_Pin;                                   \
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;                      \
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;                  \
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;            \
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                          \
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;                        \
  LL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);                    \
} while (0)

#define COPY_BUTTON_TO_LED() do {                                  \
  if (LL_GPIO_IsInputPinSet(B1_GPIO_Port, B1_Pin)) {               \
    LL_GPIO_SetOutputPin(LD4_GPIO_Port, LD4_Pin);                  \
  } else {                                                         \
    LL_GPIO_ResetOutputPin(LD4_GPIO_Port, LD4_Pin);                \
  }                                                                \
} while (0)

int main(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();
  INIT_GPIO_PAIR();

  while (1)
  {
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
    COPY_BUTTON_TO_LED();
  }
}
