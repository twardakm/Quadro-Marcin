#include "LED.h"
#include "dane.h"
#include "silniki.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_LED(void)
{
	GPIO_InitTypeDef *led_ready = malloc(sizeof(GPIO_InitTypeDef));
	led_ready->GPIO_Pin = LED_READY_PIN;
	led_ready->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	led_ready->GPIO_Speed = GPIO_Speed_2MHz; //mo¿na wybraæ 2,10,50 MHz
	GPIO_Init(LED_READY_GPIO, led_ready);

	free(led_ready);

	inicjalizacja_TIM2();
}

void inicjalizacja_TIM2(void)
{
	//struktura timera
	TIM_TimeBaseInitTypeDef *timer = malloc(sizeof(TIM_TimeBaseInitTypeDef));
	timer->TIM_CounterMode = TIM_CounterMode_Up; //zliczanie do góry
	timer->TIM_Prescaler = 1083; //preskaler z 72 MHz
	timer->TIM_Period = 16383; //do tej liczby zlicza
	timer->TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM2, timer);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //w³¹czenie przerwania TIM2
	TIM_Cmd(TIM2, ENABLE);

	free(timer);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) //sprawdzenie Ÿród³a
	{
		LED_READY_GPIO->ODR ^= (LED_READY_PIN);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update); //wyzerowanie flagi przerwania

		//sprawdzenie czy wy³¹czyæ silniki
		if (dane_czujniki.czy_polaczony == 0)
			awaryjny_stop();
		else
		{
			dane_czujniki.czy_polaczony = 0;
			dane_czujniki.ktory_PID = 0;
		}
	}
}
