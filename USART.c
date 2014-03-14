#include "naglowki_include.h"
#include "USART.h"
#include "LED.h"
#include "dane.h"
#include "I2C.h"
#include "sensory.h"
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
	USART_SendData(USART2, dane_czujniki.akcel.x_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.x_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.y_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.y_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.z_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.akcel.z_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}

	USART_SendData(USART2, dane_czujniki.zyro.x_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.x_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.y_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.y_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.z_h);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
	USART_SendData(USART2, dane_czujniki.zyro.z_l);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){}
}

void USART2_IRQHandler(void)
{
	uint16_t dane=0;
	uint8_t bufor[6]; //6 - tyle danych zczytujemy

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //sprawdzenie czy aby na pewno odpowiednie przerwanie
	{
		dane = USART_ReceiveData(USART2);
		if (dane == DANE_START)
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
			//--------------------------
			wyslij_dane();
		}
		else
			USART_SendData(USART2, 1);
	}
}
