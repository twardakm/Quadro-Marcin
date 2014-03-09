//automatycznie wygenerowany przez CoIDE, ja tylko includujê
#include "cmsis_boot/stm32f10x.h"
#include "cmsis/core_cm3.h"
#include "cmsis_boot/system_stm32f10x.h"
#include "stm_lib/inc/stm32f10x_gpio.h"
#include "stm_lib/inc/stm32f10x_i2c.h"
#include "stm_lib/inc/stm32f10x_rcc.h"
#include "stm_lib/inc/stm32f10x_tim.h"
//----------------------------------------------------------


int main(void)
{
	//zainicjalizownaie zegara
	SystemInit();

	//w³¹czenie zasilania na linii APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//APB1 dla TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//struktura LED
	GPIO_InitTypeDef Led_Ready;
	Led_Ready.GPIO_Pin = GPIO_Pin_All;
	Led_Ready.GPIO_Mode = GPIO_Mode_Out_PP;
	Led_Ready.GPIO_Speed = GPIO_Speed_2MHz;

	//inicjalizacja LED i ustawienie bitu
	GPIO_Init(GPIOC, &Led_Ready);
	GPIO_WriteBit(GPIOC, Led_Ready.GPIO_Pin, Bit_SET);

	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Prescaler = 1024;
	timer.TIM_Period = 65535; //do tej liczby zlicza
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);

    while(1)
    {
    	if (TIM_GetCounter(TIM2) > 65530)
    		GPIO_WriteBit(GPIOC, Led_Ready.GPIO_Pin, Bit_RESET);
    }
}
