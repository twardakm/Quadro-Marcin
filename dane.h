#include "naglowki_include.h"

#ifndef _DANE_H_
#define _DANE_H_

#define DANE_START 'd'
#define SILNIK1_REG 'A'
#define SILNIK2_REG 'B'
#define SILNIK3_REG 'C'
#define SILNIK4_REG 'D'

#define SREDNIA 32

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

	uint8_t ktora_srednia;
	uint16_t x_srednia;
	uint16_t x_srednia_temp[SREDNIA];
	uint16_t z_srednia;
	uint16_t z_srednia_temp[SREDNIA];

	uint32_t temp; //s�u�y do sumowania srednia_temp i przesuniecia bitowego
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
	uint32_t kat_z; //obr�t wok� z, w mikrostopniach
	uint32_t kat_y; //obr�t wok� y
	uint32_t kat_x; //obr�t wok� x
} pozycjaTypeDef;

typedef struct
{
	akcelTypeDef akcel;
	zyroTypeDef zyro;
	pozycjaTypeDef pozycja;

	uint8_t czy_polaczony;
	uint8_t ktory_PID;

	/*
	program sprawdza, jesli 0 to znaczy ze rozlaczony (silniki wylaczyc),
	jesli 1 mozna pracowac dalej
	sprawdzenie w przerwaniu LED TIM2_IRQHandler
	*/
}daneTypeDef;

void inicjalizacja_dane();

#endif
