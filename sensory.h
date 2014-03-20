#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define AKCEL_ADR 0x30
#define ZYRO_ADR  0xD4


void inicjalizacja_akcelerometr();
void inicjalizacja_SysTick();
void inicjalizacja_zyroskop();
#endif
