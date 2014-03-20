#include "naglowki_include.h"
#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "LED.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_akcelerometr()
{
	//OBOWI¥ZKOWO bez tego nie dzia³a
	wyslij_I2C(AKCEL_ADR, 0x20, 0b10010111);//ustawienie czestotliwosci
	wyslij_I2C(AKCEL_ADR, 0x21, 0b00000000); //filtrowanie wylaczone
	wyslij_I2C(AKCEL_ADR, 0x22, 0b00000000); //bit 4 - interrupt poprowadzony (nieaktywny)
	wyslij_I2C(AKCEL_ADR, 0x23, 0b00111000); //wysoka rozdzielczosci +-8G
	wyslij_I2C(AKCEL_ADR, 0x24, 0b00000000); //FIFO (bylo wlaczone przez Kamila)
}

void inicjalizacja_SysTick(void)
{
	SysTick_Config(SystemCoreClock / 100); //co 10 ms
}

void inicjalizacja_zyroskop()
{

	wyslij_I2C(ZYRO_ADR, 0x20, 0b00001111);  //wlaczony zyroskop
	wyslij_I2C(ZYRO_ADR, 0x21, 0b00000000); //filtry
	wyslij_I2C(ZYRO_ADR, 0x22, 0b00000000);// poki co pin zle poprowadzony wiec niepotrzebne, jakis dziwny
	wyslij_I2C(ZYRO_ADR, 0x23, 0b00100000); //2000 dps
	wyslij_I2C(ZYRO_ADR, 0x24, 0b00000000); //fifo, filtr wylaczone
}

void SysTick_Handler(void) //co 10 ms
{
	uint8_t bufor[6]; //6 - tyle danych zczytujemy

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
