#include "naglowki_include.h"
#include "dane.h"

volatile daneTypeDef dane_czujniki;
void inicjalizacja_dane()
{
	dane_czujniki.akcel.x_h = 0;
	dane_czujniki.akcel.x_l = 0;
	dane_czujniki.akcel.y_h = 0;
	dane_czujniki.akcel.y_l = 0;
	dane_czujniki.akcel.z_h = 0;
	dane_czujniki.akcel.z_l = 0;

	dane_czujniki.akcel.x = 0;
	dane_czujniki.akcel.y = 0;
	dane_czujniki.akcel.z = 0;

	dane_czujniki.zyro.x_h = 0;
	dane_czujniki.zyro.x_l = 0;
	dane_czujniki.zyro.y_h = 0;
	dane_czujniki.zyro.y_l = 0;
	dane_czujniki.zyro.z_h = 0;
	dane_czujniki.zyro.z_l = 0;

	dane_czujniki.zyro.x = 0;
	dane_czujniki.zyro.y = 0;
	dane_czujniki.zyro.z = 0;

	dane_czujniki.pozycja.kat_z = 0;
	dane_czujniki.pozycja.kat_y = 0;
	dane_czujniki.pozycja.kat_x = 0;

	dane_czujniki.przysp.przysp_x = 0;
	dane_czujniki.przysp.przysp_y = 0;
	dane_czujniki.przysp.przysp_z = 0;

}
