#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define AKCEL_ADR 0x30
#define ZYRO_ADR  0xD4


void inicjalizacja_akcelerometr();
void inicjalizacja_sensory();
void inicjalizacja_SysTick();
void inicjalizacja_zyroskop();

void odczyt_akcelerometr(uint8_t *bufor);
void odczyt_zyroskop(uint8_t *bufor);
#endif
