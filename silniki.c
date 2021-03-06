#include "silniki.h"

void inicjalizacja_silniki()
{
	//alternate pull-up dla regulatorow
	GPIO_InitTypeDef *regulator = malloc(sizeof(GPIO_InitTypeDef));
	regulator->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	regulator->GPIO_Mode = GPIO_Mode_AF_PP; //alternate pull-up
	regulator->GPIO_Speed = GPIO_Speed_2MHz; //mo�na wybra� 2,10,50 MHz
	GPIO_Init(GPIOB, regulator);

	free(regulator);

	TIM4->PSC = PRESKALER_TIM4; //preskaler 16 72 MHz / 16 = 4,5 MHz
	TIM4->ARR = ARR_TIM4; //do tej liczby zlicza

	/*te ustawienia odpowiadaj� za por�wnanie, tzn dop�ki
	 * licznik nie b�dzie mniejszy r�wny  CCR1
	 */
	TIM4->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
	TIM4->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1;

	//uruchomienie kana��w wyjscia
	TIM4->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E  | TIM_CCER_CC3E | TIM_CCER_CC4E;

	//wlaczenie timera
	TIM4->CR1 = TIM_CR1_CEN;

	//ustawianie silnik�w na 0
	ustaw_silnik(1, 0);
	ustaw_silnik(2, 0);
	ustaw_silnik(3, 0);
	ustaw_silnik(4, 0);
}

void ustaw_silnik(int i, uint16_t wartosc)
{
	if (wartosc > 255)
		return;
	switch (i)
	{
	case 1:
		SILNIK1 = wartosc;
		return;
	case 2:
		SILNIK2 = wartosc;
		return;
	case 3:
		SILNIK3 = wartosc;
		return;
	case 4:
		SILNIK4 = wartosc;
		return;
	}
}

void awaryjny_stop()
{
	ustaw_silnik(1, 0);
	ustaw_silnik(2, 0);
	ustaw_silnik(3, 0);
	ustaw_silnik(4, 0);
}

