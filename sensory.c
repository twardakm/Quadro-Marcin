#include "naglowki_include.h"
#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "LED.h"

extern volatile daneTypeDef dane_czujniki;

uint32_t dodaj_kat(uint32_t nowy, uint32_t stary)
{
	stary += nowy;
	if (stary >= 360000000)
		stary -= 360000000;
	return stary;
}

uint32_t odejmij_kat(uint32_t nowy, uint32_t stary)
{
	if (nowy > stary)
		stary += 360000000 - nowy;
	else
		stary -= nowy;
	return stary;
}

void inicjalizacja_akcelerometr()
{
	//OBOWI�ZKOWO bez tego nie dzia�a
	wyslij_I2C(AKCEL_ADR, 0x20, 0b01110111);//ustawienie czestotliwosci
	wyslij_I2C(AKCEL_ADR, 0x21, 0b00000000); //filtrowanie wylaczone
	wyslij_I2C(AKCEL_ADR, 0x22, 0b00000000); //bit 4 - interrupt poprowadzony (nieaktywny)
	wyslij_I2C(AKCEL_ADR, 0x23, 0b00111000); //wysoka rozdzielczosci +-16G
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
	odczyt_akcelerometr(bufor);
	odczyt_zyroskop(bufor);

	//obliczanie kata z
	if (dane_czujniki.zyro.z > 32768)
		dane_czujniki.pozycja.kat_z = odejmij_kat((65536 - dane_czujniki.zyro.z) * DT *MDEG, dane_czujniki.pozycja.kat_z);
	else
		dane_czujniki.pozycja.kat_z = dodaj_kat(dane_czujniki.zyro.z * DT *MDEG, dane_czujniki.pozycja.kat_z);
	dane_czujniki.pozycja.kat_z += ZYRO_Z_KALIBR;

	//obliczanie kata y
	if (dane_czujniki.zyro.y > 32768)
		dane_czujniki.pozycja.kat_y = odejmij_kat((65536 - dane_czujniki.zyro.y) * DT *MDEG, dane_czujniki.pozycja.kat_y);
	else
		dane_czujniki.pozycja.kat_y = dodaj_kat(dane_czujniki.zyro.y * DT *MDEG, dane_czujniki.pozycja.kat_y);
	dane_czujniki.pozycja.kat_y += ZYRO_Y_KALIBR;

	//obliczanie kata x
	if (dane_czujniki.zyro.x > 32768)
		dane_czujniki.pozycja.kat_x = odejmij_kat((65536 - dane_czujniki.zyro.x) * DT *MDEG, dane_czujniki.pozycja.kat_x);
	else
		dane_czujniki.pozycja.kat_x = dodaj_kat(dane_czujniki.zyro.x * DT *MDEG, dane_czujniki.pozycja.kat_x);
	dane_czujniki.pozycja.kat_x += ZYRO_X_KALIBR;

	//obliczanie przysp z
	/*if (dane_czujniki.akcel.z > 32768)
		dane_czujniki.przysp.przysp_z = (65536 - dane_czujniki.akcel.z) * MACC;
	else*/
		dane_czujniki.przysp.przysp_z = dane_czujniki.akcel.z * MACC;

}
