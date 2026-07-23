/**
 * STM32Cube HAL GPIO benchmark.
 * STM32CubeL4 v1.18.2 / STM32L4 HAL driver v1.13.6.
 */
#include "main.h"

#define INIT_GPIO_PAIR() do {                                      \
  GPIO_InitStruct.Pin = B1_Pin;                                    \
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;                          \
  GPIO_InitStruct.Pull = GPIO_NOPULL;                              \
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);                    \
  GPIO_InitStruct.Pin = LD4_Pin;                                   \
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;                      \
  GPIO_InitStruct.Pull = GPIO_NOPULL;                              \
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                     \
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);                   \
} while (0)

#define COPY_BUTTON_TO_LED() do {                                  \
  if (GPIO_PIN_SET == HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {    \
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);       \
  } else {                                                         \
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);     \
  }                                                                \
} while (0)

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
