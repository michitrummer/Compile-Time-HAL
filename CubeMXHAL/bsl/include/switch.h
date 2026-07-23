#ifndef CUBEHAL_BSL_SWITCH_H
#define CUBEHAL_BSL_SWITCH_H
#include "main.h"
typedef enum {
  bsl_switchB1,
  bsl_switchEx1,
  bsl_switchEx2,
  bsl_switchEx3,
  bsl_switchEx4,
  bsl_switchEx5,
  bsl_switchEx6,
  bsl_switchEx7,
  bsl_switchEx8
} bsl_Switch;
static inline GPIO_TypeDef* bsl_switchPort(bsl_Switch id) {
  if (id == bsl_switchB1 || id == bsl_switchEx5 || id == bsl_switchEx6)
    return GPIOC;
  if (id == bsl_switchEx7) return GPIOD;
  if (id == bsl_switchEx8) return GPIOE;
  return GPIOA;
}
static inline uint16_t bsl_switchPin(bsl_Switch id) {
  static const uint16_t pins[] = {GPIO_PIN_13, GPIO_PIN_0, GPIO_PIN_1,
                                  GPIO_PIN_2,  GPIO_PIN_3, GPIO_PIN_4,
                                  GPIO_PIN_5,  GPIO_PIN_6, GPIO_PIN_7};
  return pins[(unsigned)id];
}
static inline int bsl_switchPressed(bsl_Switch id) {
  return HAL_GPIO_ReadPin(bsl_switchPort(id), bsl_switchPin(id)) ==
         GPIO_PIN_SET;
}
#endif
