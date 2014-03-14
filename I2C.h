#include "naglowki_include.h"
#ifndef _I2C_H_
#define _I2C_H_


void inicjalizacja_I2C_GPIO();
void inicjalizacja_I2C();
void odczyt_I2C(uint8_t adres, uint8_t rejestr, uint8_t dlugosc, uint8_t *bufor);
int sprawdz_blad_I2C();


#endif
