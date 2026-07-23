#ifndef CUBEHAL_BSL_LED_H
#define CUBEHAL_BSL_LED_H
#include "main.h"
typedef enum {
  bsl_led4,
  bsl_ledEx1,
  bsl_ledEx2,
  bsl_ledEx3,
  bsl_ledEx4,
  bsl_ledEx5,
  bsl_ledEx6,
  bsl_ledEx7,
  bsl_ledEx8
} bsl_Led;
static inline GPIO_TypeDef* bsl_ledPort(bsl_Led id) {
  return id == bsl_ledEx7 ? GPIOE : id == bsl_ledEx8 ? GPIOH : GPIOB;
}
static inline uint16_t bsl_ledPin(bsl_Led id) {
  static const uint16_t pins[] = {GPIO_PIN_13, GPIO_PIN_0, GPIO_PIN_1,
                                  GPIO_PIN_2,  GPIO_PIN_3, GPIO_PIN_4,
                                  GPIO_PIN_5,  GPIO_PIN_6, GPIO_PIN_0};
  return pins[(unsigned)id];
}
static inline void bsl_ledOn(bsl_Led id) {
  HAL_GPIO_WritePin(bsl_ledPort(id), bsl_ledPin(id), GPIO_PIN_SET);
}
static inline void bsl_ledOff(bsl_Led id) {
  HAL_GPIO_WritePin(bsl_ledPort(id), bsl_ledPin(id), GPIO_PIN_RESET);
}
#endif
