#include "naglowki_include.h"
#include "USART.h"
#include "LED.h"

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

	USART_InitTypeDef *usart = malloc(sizeof(USART_InitTypeDef));
	usart->USART_BaudRate = 115200;
	usart->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart->USART_Parity = USART_Parity_No;
	usart->USART_StopBits = USART_StopBits_1;
	usart->USART_WordLength = USART_WordLength_8b;

	//konfigurowanie przerwania usart
	NVIC_InitTypeDef *usart_irq = malloc(sizeof(NVIC_InitTypeDef));
	usart_irq->NVIC_IRQChannel = USART2_IRQn; //USART2
	usart_irq->NVIC_IRQChannelPreemptionPriority = 0;
	usart_irq->NVIC_IRQChannelSubPriority = 1;
	usart_irq->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(usart_irq); //inicjalizacja NVIC
	NVIC_EnableIRQ(USART2_IRQn); //w³¹czenie przerwania NVIC

	USART_Init(USART2, usart);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);

	free(usart);
	free(usart_irq);
}

void USART2_IRQHandler(void)
{
	uint16_t *dane = malloc (sizeof (uint16_t));
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //sprawdzenie czy aby na pewno odpowiednie przerwanie
	{
		*dane = USART_ReceiveData(USART2);
		USART_SendData(USART2, *dane);
	}
	free(dane);
}
