//automatycznie wygenerowany przez CoIDE, ja tylko includujê
#include "naglowki_include.h"
//----------------------------------------------------------
#include "inicjalizacja.h"
#include "LED.h"
#include "USART.h"
#include "NVIC.h"
#include "dane.h"
#include "I2C.h"
#include "sensory.h"

int main(void)
{
	inicjalizacja_zegara();
	inicjalizacja_zasilania();
	inicjalizacja_dane();
	inicjalizacja_LED();
	inicjalizacja_I2C_GPIO();
	inicjalizacja_I2C();
	inicjalizacja_USART();
	inicjalizacja_NVIC();
	inicjalizacja_akcelerometr();
	inicjalizacja_zyroskop();
	inicjalizacja_TIM3();

    while(1)
    {
    }
}
