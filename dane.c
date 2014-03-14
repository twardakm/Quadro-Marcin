#include "naglowki_include.h"
#include "dane.h"

volatile daneTypeDef dane_czujniki;
void inicjalizacja_dane()
{
	dane_czujniki.akcel.x_h = 10;
	dane_czujniki.akcel.x_l = 22;
	dane_czujniki.akcel.y_h = 3;
	dane_czujniki.akcel.y_l = 4;
	dane_czujniki.akcel.z_h = 130;
	dane_czujniki.akcel.z_l = 135;

	dane_czujniki.zyro.x_h = 200;
	dane_czujniki.zyro.x_l = 202;
	dane_czujniki.zyro.y_h = 103;
	dane_czujniki.zyro.y_l = 204;
	dane_czujniki.zyro.z_h = 100;
	dane_czujniki.zyro.z_l = 101;
}
