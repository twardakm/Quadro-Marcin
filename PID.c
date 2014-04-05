#include "PID.h"
#include "dane.h"
#include "silniki.h"

extern volatile daneTypeDef dane_czujniki;

void regulacja_PID()
{
	if(dane_czujniki.akcel.z_srednia > 15000 && dane_czujniki.akcel.z_srednia < 17000)
	{
		dane_czujniki.pozycja.kat_y = 0;
	}
	if (dane_czujniki.czy_polaczony == 0)
	{
		if (dane_czujniki.ktory_PID >= 25) //PID wywo³ywany co 10 ms, najpozniej po 250 ms od rozloczenia wylaczenie silnikow
		{
			awaryjny_stop();
			return;
		}
		else
			dane_czujniki.ktory_PID++;
	}
	else
		dane_czujniki.ktory_PID = 0;
	if (dane_czujniki.pozycja.kat_y < 360000000 * 0.95)
	{
		ustaw_silnik(1, 120);
		ustaw_silnik(2, 120);
	}
	else
	{
		ustaw_silnik(1, 0);
		ustaw_silnik(2, 0);
	}
}
