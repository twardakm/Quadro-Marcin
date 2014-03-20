#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

#define AKCEL_ADR 0x30
#define ZYRO_ADR  0xD4

#define DT 10 //10 ms jako interwa³ pomiêdzy odbiorem kolejnych danych
#define MDEG 8.75 //milistopni na 1 cyfrê z ¿yroskopu

uint32_t dodaj_kat(uint32_t nowy, uint32_t stary);
uint32_t odejmij_kat(uint32_t nowy, uint32_t stary);

void inicjalizacja_akcelerometr();
void inicjalizacja_sensory();
void inicjalizacja_SysTick();
void inicjalizacja_zyroskop();

void odczyt_akcelerometr(uint8_t *bufor);
void odczyt_zyroskop(uint8_t *bufor);
#endif
