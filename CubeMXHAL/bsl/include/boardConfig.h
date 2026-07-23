#ifndef CUBEHAL_BSL_BOARD_CONFIG_H
#define CUBEHAL_BSL_BOARD_CONFIG_H
#include "main.h"
static inline void bsl_boardConfigureGpio(void) {
  GPIO_InitTypeDef i = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  i.Pin = B1_Pin;
  i.Mode = GPIO_MODE_INPUT;
  i.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &i);
  i.Pin = LD4_Pin;
  i.Mode = GPIO_MODE_OUTPUT_PP;
  i.Pull = GPIO_NOPULL;
  i.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &i);
}
static inline void bsl_boardConfigureGpioMultiEx(void) {
  GPIO_InitTypeDef i = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
#define INIT_IN(PORT, PIN, PULL) \
  do {                           \
    i.Pin = (PIN);               \
    i.Mode = GPIO_MODE_INPUT;    \
    i.Pull = (PULL);             \
    HAL_GPIO_Init((PORT), &i);   \
  } while (0)
#define INIT_OUT(PORT, PIN, MODE, PULL, SPEED) \
  do {                                         \
    i.Pin = (PIN);                             \
    i.Mode = (MODE);                           \
    i.Pull = (PULL);                           \
    i.Speed = (SPEED);                         \
    HAL_GPIO_Init((PORT), &i);                 \
  } while (0)
  INIT_IN(GPIOC, GPIO_PIN_13, GPIO_NOPULL);
  INIT_OUT(GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
           GPIO_SPEED_FREQ_LOW);
  INIT_IN(GPIOA, GPIO_PIN_0, GPIO_PULLUP);
  INIT_OUT(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,
           GPIO_SPEED_FREQ_MEDIUM);
  INIT_IN(GPIOA, GPIO_PIN_1, GPIO_PULLDOWN);
  INIT_OUT(GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_OD, GPIO_PULLDOWN,
           GPIO_SPEED_FREQ_HIGH);
  INIT_IN(GPIOA, GPIO_PIN_2, GPIO_NOPULL);
  INIT_OUT(GPIOB, GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
           GPIO_SPEED_FREQ_VERY_HIGH);
  INIT_IN(GPIOA, GPIO_PIN_3, GPIO_PULLUP);
  INIT_OUT(GPIOB, GPIO_PIN_3, GPIO_MODE_OUTPUT_OD, GPIO_PULLUP,
           GPIO_SPEED_FREQ_LOW);
  INIT_IN(GPIOC, GPIO_PIN_4, GPIO_PULLDOWN);
  INIT_OUT(GPIOB, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN,
           GPIO_SPEED_FREQ_MEDIUM);
  INIT_IN(GPIOC, GPIO_PIN_5, GPIO_NOPULL);
  INIT_OUT(GPIOB, GPIO_PIN_5, GPIO_MODE_OUTPUT_OD, GPIO_NOPULL,
           GPIO_SPEED_FREQ_HIGH);
  INIT_IN(GPIOD, GPIO_PIN_6, GPIO_PULLUP);
  INIT_OUT(GPIOE, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP,
           GPIO_SPEED_FREQ_VERY_HIGH);
  INIT_IN(GPIOE, GPIO_PIN_7, GPIO_PULLDOWN);
  INIT_OUT(GPIOH, GPIO_PIN_0, GPIO_MODE_OUTPUT_OD, GPIO_PULLDOWN,
           GPIO_SPEED_FREQ_LOW);
#undef INIT_IN
#undef INIT_OUT
}
#endif
