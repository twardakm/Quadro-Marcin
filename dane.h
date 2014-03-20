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

	uint16_t x;
	uint16_t y;
	uint16_t z;
} akcelTypeDef;

typedef struct
{
	uint8_t x_h;
	uint8_t x_l;
	uint8_t y_h;
	uint8_t y_l;
	uint8_t z_h;
	uint8_t z_l;

	uint16_t x;
	uint16_t y;
	uint16_t z;
} zyroTypeDef;

typedef struct
{
	uint32_t kat_z; //obrót wokó³ z, w mikrostopniach
} pozycjaTypeDef;

typedef struct
{
	akcelTypeDef akcel;
	zyroTypeDef zyro;
	pozycjaTypeDef pozycja;
}daneTypeDef;

void inicjalizacja_dane();

#endif
