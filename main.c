//automatycznie wygenerowany przez CoIDE, ja tylko includujê
#include "naglowki_include.h"
//----------------------------------------------------------
#include "inicjalizacja.h"
#include "LED.h"
#include "USART.h"
#include "NVIC.h"

int main(void)
{
	inicjalizacja_zegara();
	inicjalizacja_zasilania();
	inicjalizacja_LED();
	inicjalizacja_USART();
	inicjalizacja_NVIC();

    while(1)
    {
    	//USART_SendData(USART2, 100);
    }
}
