#include "naglowki_include.h"
#include "sensory.h"
#include "I2C.h"
#include "dane.h"
#include "LED.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_akcelerometr()
{
	//OBOWI�ZKOWO bez tego nie dzia�a
	wyslij_I2C(AKCEL_ADR, 0x20, 0b10010111);//ustawienie czestotliwosci
	wyslij_I2C(AKCEL_ADR, 0x21, 0b00000000); //filtrowanie wylaczone
	wyslij_I2C(AKCEL_ADR, 0x22, 0b00000000); //bit 4 - interrupt poprowadzony (nieaktywny)
	wyslij_I2C(AKCEL_ADR, 0x23, 0b00111000); //wysoka rozdzielczosci +-8G
	wyslij_I2C(AKCEL_ADR, 0x24, 0b00000000); //FIFO (bylo wlaczone przez Kamila)
}

void inicjalizacja_TIM3(void)
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do g�ry
	timer->TIM_Prescaler = 12; //preskaler z 72 MHz, dt = 10 ms
	timer->TIM_Period = 60000; //do tej liczby zlicza, po ci�kiej matmie czas odczytu 10 ms
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM3, timer);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //w��czenie przerwania TIM2
	TIM_Cmd(TIM3, ENABLE);

	free(timer);
}

void inicjalizacja_zyroskop()
{

	wyslij_I2C(ZYRO_ADR, 0x20, 0b00001111);  //wlaczony zyroskop
	wyslij_I2C(ZYRO_ADR, 0x21, 0b00000000); //filtry
	wyslij_I2C(ZYRO_ADR, 0x22, 0b00000000);// poki co pin zle poprowadzony wiec niepotrzebne, jakis dziwny
	wyslij_I2C(ZYRO_ADR, 0x23, 0b00100000); //2000 dps
	wyslij_I2C(ZYRO_ADR, 0x24, 0b00000000); //fifo, filtr wylaczone
}

void TIM3_IRQHandler(void) //co 10 ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //sprawdzenie �r�d�a
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

		LED_READY_GPIO->ODR ^= (LED_READY_PIN);

		TIM_ClearFlag(TIM3, TIM_FLAG_Update); //wyzerowanie flagi przerwania
	}


}
