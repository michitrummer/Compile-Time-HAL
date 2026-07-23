#ifndef CUBELL_BSL_LED_H
#define CUBELL_BSL_LED_H
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
static inline uint32_t bsl_ledPin(bsl_Led id) {
  static const uint32_t pins[] = {LL_GPIO_PIN_13, LL_GPIO_PIN_0, LL_GPIO_PIN_1,
                                  LL_GPIO_PIN_2,  LL_GPIO_PIN_3, LL_GPIO_PIN_4,
                                  LL_GPIO_PIN_5,  LL_GPIO_PIN_6, LL_GPIO_PIN_0};
  return pins[(unsigned)id];
}
static inline void bsl_ledOn(bsl_Led id) {
  LL_GPIO_SetOutputPin(bsl_ledPort(id), bsl_ledPin(id));
}
static inline void bsl_ledOff(bsl_Led id) {
  LL_GPIO_ResetOutputPin(bsl_ledPort(id), bsl_ledPin(id));
}
#endif
