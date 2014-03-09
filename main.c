//automatycznie wygenerowany przez CoIDE, ja tylko includujê
#include "naglowki_include.h"
//----------------------------------------------------------
#include "inicjalizacja.h"
#include "LED.h"

int main(void)
{
	inicjalizacja_zegara();
	inicjalizacja_zasilania();
	inicjalizacja_LED();

	GPIO_WriteBit(GPIOC, LED_READY_PIN, Bit_SET);

	TIM_TimeBaseInitTypeDef timer;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Prescaler = 1024;
	timer.TIM_Period = 65535; //do tej liczby zlicza
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_Cmd(TIM2, ENABLE);

	//konfigurowanie przerwania timera
	NVIC_InitTypeDef nvicStructure;
	    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	    nvicStructure.NVIC_IRQChannelSubPriority = 1;
	    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&nvicStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);

    while(1)
    {
    	/*if (TIM_GetCounter(TIM2) > 65530)
    		GPIO_WriteBit(GPIOC, Led_Ready.GPIO_Pin, Bit_RESET);*/
    }
}

void TIM2_IRQHandler(void)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_All, Bit_SET);
}
