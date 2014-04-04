#include "naglowki_include.h"

#ifndef _SILNIKI_H_
#define _SILNIKI_H_

#define PRESKALER_TIM4 0x10
#define ARR_TIM4 0xfa0
#define SILNIK_MAX 72000000/PRESKALER_TIM4/ARR_TIM4

void inicjalizacja_silniki();
void ustaw_silnik(int i, uint16_t wartosc);

#endif
