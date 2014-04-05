#include "naglowki_include.h"

#ifndef _DANE_H_
#define _DANE_H_

#define DANE_START 'd'
#define SILNIK1_REG 'A'
#define SILNIK2_REG 'B'
#define SILNIK3_REG 'C'
#define SILNIK4_REG 'D'

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
	uint32_t kat_y; //obrót wokó³ y
	uint32_t kat_x; //obrót wokó³ x
} pozycjaTypeDef;

typedef struct
{
	akcelTypeDef akcel;
	zyroTypeDef zyro;
	pozycjaTypeDef pozycja;

	uint8_t czy_polaczony; /*
	program sprawdza, jesli 0 to znaczy ze rozlaczony (silniki wylaczyc),
	jesli 1 mozna pracowac dalej
	sprawdzenie w przerwaniu LED TIM2_IRQHandler
	*/
}daneTypeDef;

void inicjalizacja_dane();

#endif
