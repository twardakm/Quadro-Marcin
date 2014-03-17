#include "NVIC.h"

void inicjalizacja_NVIC()
{
	/*NVIC_InitTypeDef *nvic_timer = malloc(sizeof(NVIC_InitTypeDef));
	nvic_timer->NVIC_IRQChannel = TIM2_IRQn | USART2_IRQn; //timer2
	nvic_timer->NVIC_IRQChannelPreemptionPriority = 1;
	nvic_timer->NVIC_IRQChannelSubPriority = 1;
	nvic_timer->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(nvic_timer); //inicjalizacja NVIC

	nvic_timer->NVIC_IRQChannel = TIM3_IRQn; //timer3 najważniejszy, odpowiada za dane
	nvic_timer->NVIC_IRQChannelPreemptionPriority = 0;
	nvic_timer->NVIC_IRQChannelSubPriority = 1;
	nvic_timer->NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(nvic_timer); //inicjalizacja NVIC*/

	//konfigurowanie przerwania timera
	NVIC_SetPriority(TIM3_IRQn, 0);
	NVIC_SetPriority(TIM2_IRQn, 1);

	NVIC_EnableIRQ(TIM2_IRQn); //włączenie przerwania NVIC
	NVIC_EnableIRQ(TIM3_IRQn); //włączenie przerwania NVIC

	//free(nvic_timer);
}
