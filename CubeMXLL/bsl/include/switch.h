#ifndef CUBELL_BSL_SWITCH_H
#define CUBELL_BSL_SWITCH_H
#include "main.h"
typedef enum { bsl_switchB1,bsl_switchEx1,bsl_switchEx2,bsl_switchEx3,bsl_switchEx4,bsl_switchEx5,bsl_switchEx6,bsl_switchEx7,bsl_switchEx8 } bsl_Switch;
static inline GPIO_TypeDef* bsl_switchPort(bsl_Switch id){ if(id==bsl_switchB1||id==bsl_switchEx5||id==bsl_switchEx6)return GPIOC; if(id==bsl_switchEx7)return GPIOD; if(id==bsl_switchEx8)return GPIOE; return GPIOA; }
static inline uint32_t bsl_switchPin(bsl_Switch id){ static const uint32_t pins[]={LL_GPIO_PIN_13,LL_GPIO_PIN_0,LL_GPIO_PIN_1,LL_GPIO_PIN_2,LL_GPIO_PIN_3,LL_GPIO_PIN_4,LL_GPIO_PIN_5,LL_GPIO_PIN_6,LL_GPIO_PIN_7}; return pins[(unsigned)id]; }
static inline int bsl_switchPressed(bsl_Switch id){ return LL_GPIO_IsInputPinSet(bsl_switchPort(id),bsl_switchPin(id)); }
#endif
