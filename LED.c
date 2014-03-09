#include "LED.h"

void inicjalizacja_LED(void)
{
	GPIO_InitTypeDef *led_ready = malloc(sizeof(GPIO_InitTypeDef));
	led_ready->GPIO_Pin = LED_READY_PIN;
	led_ready->GPIO_Mode = GPIO_Mode_Out_PP; //wyjscie pull-up
	led_ready->GPIO_Speed = GPIO_Speed_2MHz; //mo¿na wybraæ 2,10,50 MHz
	GPIO_Init(LED_READY_GPIO, led_ready);

	free(led_ready);
}
