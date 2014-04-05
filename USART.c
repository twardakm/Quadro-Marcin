#include "naglowki_include.h"
#include "USART.h"
#include "LED.h"
#include "dane.h"
#include "I2C.h"
#include "sensory.h"
#include "silniki.h"
/*extern potrzebne aby poinformowaæ kompilator ¿e zmienna zosta³a zadeklarowana
 * w innym pliku. Bez tego nie dzia³a
 */
extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_USART()
{
	GPIO_InitTypeDef *usart_gpio = malloc(sizeof (GPIO_InitTypeDef));
	//PA2 jako TX
	usart_gpio->GPIO_Pin = GPIO_Pin_2;
	usart_gpio->GPIO_Mode = GPIO_Mode_AF_PP;
	usart_gpio->GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, usart_gpio);
	//PA3 jako RX
	usart_gpio->GPIO_Pin = GPIO_Pin_3;
	usart_gpio->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, usart_gpio);
	free(usart_gpio);

	USART_Cmd(USART2, ENABLE);

	USART_InitTypeDef *usart = malloc(sizeof(USART_InitTypeDef));
	usart->USART_BaudRate = 115200;
	usart->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart->USART_Parity = USART_Parity_No;
	usart->USART_StopBits = USART_StopBits_1;
	usart->USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, usart);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART2_IRQn); //w³¹czenie przerwania NVIC

	free(usart);
	//free(usart_irq);
}

void wyslij_dane()
{
	/*Wysy³anie danych z AKCELEROMETRU
	 * -------------------------------
	 */
	USART_SendData(USART2, dane_czujniki.akcel.x >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.x);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.y >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.y);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.z >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.z);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	/* -------------------------------- */

	/*Wysy³anie danych z ZYROSKOPU
	 * --------------------------------
	 */
	USART_SendData(USART2, dane_czujniki.zyro.x >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.x);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.y >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.y);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.z >> 8);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.z);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	/* -------------------------------- */

	/*Wysy³anie obliczonych danych
	 * --------------------------------
	 */
	//¯YROSKOP
	uint32_t temp = dane_czujniki.pozycja.kat_z;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	temp=dane_czujniki.pozycja.kat_y;

	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	temp=dane_czujniki.pozycja.kat_x;

	USART_SendData(USART2, temp);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	//AKCELEROMETR
	temp=dane_czujniki.akcel.z_srednia;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	temp=dane_czujniki.akcel.y;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	temp=dane_czujniki.akcel.x_srednia;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	temp = temp >> 8;
	USART_SendData(USART2, temp);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	/* -------------------------------- */
}

void USART2_IRQHandler(void)
{
	uint8_t dane=0;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //sprawdzenie czy aby na pewno odpowiednie przerwanie
	{
		dane = USART_ReceiveData(USART2);
		if (dane == DANE_START)
			wyslij_dane();
		else if (dane == SILNIK1_REG)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
			dane = USART_ReceiveData(USART2);
			ustaw_silnik(1, dane);
			USART_SendData(USART2, 'y');
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
		}
		else if (dane == SILNIK2_REG)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
			dane = USART_ReceiveData(USART2);
			ustaw_silnik(2, dane);
			USART_SendData(USART2, 'y');
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
		}
		else if (dane == SILNIK3_REG)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
			dane = USART_ReceiveData(USART2);
			ustaw_silnik(3, dane);
			USART_SendData(USART2, 'y');
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
		}
		else if (dane == SILNIK4_REG)
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
			dane = USART_ReceiveData(USART2);
			ustaw_silnik(4, dane);
			USART_SendData(USART2, 'y');
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
		}
		else
		{
			USART_SendData(USART2, dane);
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
		}
		dane_czujniki.czy_polaczony = 1;
		dane_czujniki.ktory_PID = 0;
	}
}
