#include "inicjalizacja.h"

void inicjalizacja_zegara(void) //72 MHz
{
	//zainicjalizownaie zegara
	SystemInit();
}

void inicjalizacja_zasilania(void)
{
	//w³¹czenie zasilania na linii APB2
	//GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//USART2 + GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//APB1 dla TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //funkcje alternatywne pinów
}
