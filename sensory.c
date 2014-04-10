#include "naglowki_include.h"
#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "LED.h"
#include "PID.h"

extern volatile daneTypeDef dane_czujniki;

uint32_t dodaj_kat(uint32_t nowy, uint32_t stary)
{
	if (stary == 0)
		return nowy;
	stary += nowy;
	if (stary >= 360000000)
		stary -= 360000000;
	return stary;
}

uint32_t odejmij_kat(uint32_t nowy, uint32_t stary)
{
	if (stary == 0)
		return 360000000 - nowy;
	if (nowy >= stary)
		stary += (360000000 - nowy);
	else
		stary -= nowy;
	if (stary >= 360000000)
		stary -= 360000000;
	return stary;
}

void inicjalizacja_akcelerometr()
{
	//OBOWI¥ZKOWO bez tego nie dzia³a
	wyslij_I2C(AKCEL_ADR, 0x20, 0b01110111);//ustawienie czestotliwosci
	wyslij_I2C(AKCEL_ADR, 0x21, 0b00000000); //filtrowanie wylaczone
	wyslij_I2C(AKCEL_ADR, 0x22, 0b00000000); //bit 4 - interrupt poprowadzony (nieaktywny)
	wyslij_I2C(AKCEL_ADR, 0x23, 0b00000000); //wysoka rozdzielczosci +-16G
	wyslij_I2C(AKCEL_ADR, 0x24, 0b00000000); //FIFO (bylo wlaczone przez Kamila)
}

void inicjalizacja_sensory()
{
	inicjalizacja_akcelerometr();
	inicjalizacja_zyroskop();
	inicjalizacja_SysTick();
}

void inicjalizacja_SysTick(void) //na SysTick oparty jest odczyt danych
{
	SysTick_Config(SystemCoreClock / 100); //co 10 ms
}

void inicjalizacja_zyroskop()
{

	wyslij_I2C(ZYRO_ADR, 0x20, 0b11111111);  //wlaczony zyroskop
	wyslij_I2C(ZYRO_ADR, 0x21, 0b00100000); //filtry
	wyslij_I2C(ZYRO_ADR, 0x22, 0b00000000);// poki co pin zle poprowadzony wiec niepotrzebne, jakis dziwny
	wyslij_I2C(ZYRO_ADR, 0x23, 0b00000000); //250 dps
	wyslij_I2C(ZYRO_ADR, 0x24, 0b00000000); //fifo, filtr wylaczone
}

void odczyt_akcelerometr(uint8_t *bufor)
{
	/*odczyt z akcelerometru
	 * ------------------------
	 */
	odczyt_I2C(AKCEL_ADR,0xA8,6,bufor);
	dane_czujniki.akcel.x_l = bufor[0];
	dane_czujniki.akcel.x_h = bufor[1];
	dane_czujniki.akcel.y_l = bufor[2];
	dane_czujniki.akcel.y_h = bufor[3];
	dane_czujniki.akcel.z_l = bufor[4];
	dane_czujniki.akcel.z_h = bufor[5];

	dane_czujniki.akcel.x = (dane_czujniki.akcel.x_h<<8)+dane_czujniki.akcel.x_l;
	dane_czujniki.akcel.y = (dane_czujniki.akcel.y_h<<8)+dane_czujniki.akcel.y_l;
	dane_czujniki.akcel.z = (dane_czujniki.akcel.z_h<<8)+dane_czujniki.akcel.z_l;
	//--------------------------
}

void odczyt_zyroskop(uint8_t *bufor)
{
	/*odczyt z zyroskopu
	* ------------------------
	*/
	odczyt_I2C(ZYRO_ADR,0xA8,6,bufor);
	dane_czujniki.zyro.x_l = bufor[0];
	dane_czujniki.zyro.x_h = bufor[1];
	dane_czujniki.zyro.y_l = bufor[2];
	dane_czujniki.zyro.y_h = bufor[3];
	dane_czujniki.zyro.z_l = bufor[4];
	dane_czujniki.zyro.z_h = bufor[5];

	dane_czujniki.zyro.x = (dane_czujniki.zyro.x_h<<8)+dane_czujniki.zyro.x_l;
	dane_czujniki.zyro.y = (dane_czujniki.zyro.y_h<<8)+dane_czujniki.zyro.y_l;
	dane_czujniki.zyro.z = (dane_czujniki.zyro.z_h<<8)+dane_czujniki.zyro.z_l;
	//--------------------------
}

void SysTick_Handler(void) //co 10 ms przerwanie SysTick
{
	uint8_t bufor[6]; //6 - tyle danych zczytujemy
	uint8_t i; //do petli
	odczyt_akcelerometr(bufor);
	odczyt_zyroskop(bufor);

	//obliczanie kata z
	if (dane_czujniki.zyro.z > 32768)
		dane_czujniki.pozycja.kat_z = odejmij_kat((int)((65536 - dane_czujniki.zyro.z) * DT *MDEG), dane_czujniki.pozycja.kat_z);
	else
		dane_czujniki.pozycja.kat_z = dodaj_kat((int)(dane_czujniki.zyro.z * DT *MDEG), dane_czujniki.pozycja.kat_z);
	odejmij_kat(ZYRO_Z_KALIBR, dane_czujniki.pozycja.kat_z);

	//obliczanie kata y
	if (dane_czujniki.zyro.y > 32768)
		dane_czujniki.pozycja.kat_y = odejmij_kat((int)((65536 - dane_czujniki.zyro.y) * DT *MDEG), dane_czujniki.pozycja.kat_y);
	else
		dane_czujniki.pozycja.kat_y = dodaj_kat((int)(dane_czujniki.zyro.y * DT *MDEG), dane_czujniki.pozycja.kat_y);
	odejmij_kat(ZYRO_Y_KALIBR, dane_czujniki.pozycja.kat_y);

	//obliczanie kata x
	if (dane_czujniki.zyro.x > 32768)
		dane_czujniki.pozycja.kat_x = odejmij_kat((int)((65536 - dane_czujniki.zyro.x) * DT *MDEG), dane_czujniki.pozycja.kat_x);
	else
		dane_czujniki.pozycja.kat_x = dodaj_kat((int)(dane_czujniki.zyro.x * DT *MDEG), dane_czujniki.pozycja.kat_x);
	dodaj_kat(ZYRO_X_KALIBR, dane_czujniki.pozycja.kat_x);

	//srednia dla przyspieszen
	if (dane_czujniki.akcel.ktora_srednia >= SREDNIA)
		dane_czujniki.akcel.ktora_srednia = 0;

	dane_czujniki.akcel.x_srednia_temp[dane_czujniki.akcel.ktora_srednia] = dane_czujniki.akcel.x;
	dane_czujniki.akcel.z_srednia_temp[dane_czujniki.akcel.ktora_srednia] = dane_czujniki.akcel.z;

	for (i = 0, dane_czujniki.akcel.temp = 0; i < SREDNIA; i++)
	{
		if(dane_czujniki.akcel.x_srednia_temp[dane_czujniki.akcel.ktora_srednia] < 32768)
			dane_czujniki.akcel.temp += dane_czujniki.akcel.x_srednia_temp[dane_czujniki.akcel.ktora_srednia];
		else
			dane_czujniki.akcel.temp -= dane_czujniki.akcel.x_srednia_temp[dane_czujniki.akcel.ktora_srednia];
	}
	dane_czujniki.akcel.x_srednia = (dane_czujniki.akcel.temp >> 5);

	//Z
	//-------------

	for (i = 0, dane_czujniki.akcel.temp = 0; i < SREDNIA; i++)
	{
		if(dane_czujniki.akcel.z_srednia_temp[i] < 32768)
			dane_czujniki.akcel.temp += dane_czujniki.akcel.z_srednia_temp[i];
		else
			dane_czujniki.akcel.temp += (65536 - dane_czujniki.akcel.z_srednia_temp[i]);
	}
	dane_czujniki.akcel.z_srednia = (dane_czujniki.akcel.temp >> 5);

	//--------------

	dane_czujniki.akcel.ktora_srednia++;

//	regulacja_PID();
}
