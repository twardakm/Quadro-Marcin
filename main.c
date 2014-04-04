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
#include "silniki.h"

int main(void)
{
	int i;
	inicjalizacja_zegara();
	inicjalizacja_zasilania();
	inicjalizacja_NVIC();

	for(i=0;i<500000;i++) //oczekiwanie na inicjalizacje zyro i akcel
		asm ("nop");


	inicjalizacja_dane();

	inicjalizacja_LED();

	inicjalizacja_I2C();
	inicjalizacja_USART();

	inicjalizacja_sensory();

	inicjalizacja_silniki();
	for(i=0;i<500000;i++) //ustabilizowanie na 0
			asm ("nop");

    while(1)
    {
    }
}
