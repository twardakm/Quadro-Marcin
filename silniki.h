#include "naglowki_include.h"

#ifndef _SILNIKI_H_
#define _SILNIKI_H_

#define PRESKALER_TIM4 719 //czestotliwoc = Clock/(Preskaler + 1) = 72 MHZ / 720 = 100 kHz
#define ARR_TIM4 255 //do tej liczby zlicza

#define SILNIK4 TIM4->CCR4
#define SILNIK3 TIM4->CCR3
#define SILNIK2 TIM4->CCR2
#define SILNIK1 TIM4->CCR1

void inicjalizacja_silniki();
void ustaw_silnik(int i, uint16_t wartosc);

#endif
