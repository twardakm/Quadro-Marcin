#include "naglowki_include.h"

#ifndef _DANE_H_
#define _DANE_H_

#define DANE_START 'd'

typedef struct
{
	uint8_t x_h;
	uint8_t x_l;
	uint8_t y_h;
	uint8_t y_l;
	uint8_t z_h;
	uint8_t z_l;
} akcelTypeDef;

typedef struct
{
	uint8_t x_h;
	uint8_t x_l;
	uint8_t y_h;
	uint8_t y_l;
	uint8_t z_h;
	uint8_t z_l;
} zyroTypeDef;

typedef struct
{
	akcelTypeDef akcel;
	zyroTypeDef zyro;
}daneTypeDef;

void inicjalizacja_dane();

#endif
