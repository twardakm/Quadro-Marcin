#include "inicjalizacja.h"

void inicjalizacja_zegara(void) //72 MHz
{
	//zainicjalizownaie zegara
	SystemInit();
}

void inicjalizacja_zasilania(void)
{
	//w³¹czenie zasilania na linii APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//APB1 dla TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
