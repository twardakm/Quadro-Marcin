#include "naglowki_include.h"

#ifndef _LED_H_
#define _LED_H_

#define LED_READY_GPIO GPIOC
#define LED_READY_PIN GPIO_Pin_3
#define LED_READY_WL GPIO_WriteBit(LED_READY_GPIO, LED_READY_PIN, Bit_SET);
#define LED_READY_WYL GPIO_WriteBit(LED_READY_GPIO, LED_READY_PIN, Bit_RESET);

void inicjalizacja_LED(void);
void inicjalizacja_TIM2(void);

#endif
