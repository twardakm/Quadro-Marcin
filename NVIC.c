#include "NVIC.h"

void inicjalizacja_NVIC()
{
	NVIC_InitTypeDef *nvic_timer = malloc(sizeof(NVIC_InitTypeDef));
	nvic_timer->NVIC_IRQChannel = TIM2_IRQn | USART2_IRQn; //timer2
	nvic_timer->NVIC_IRQChannelPreemptionPriority = 0;
	nvic_timer->NVIC_IRQChannelSubPriority = 1;
	nvic_timer->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(nvic_timer); //inicjalizacja NVIC

	free(nvic_timer);
}
