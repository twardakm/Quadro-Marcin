//automatycznie wygenerowany przez CoIDE, ja tylko includuj�
#include "naglowki_include.h"
//----------------------------------------------------------
#include "inicjalizacja.h"
#include "LED.h"
#include "USART.h"
#include "NVIC.h"
#include "dane.h"

int main(void)
{
	inicjalizacja_zegara();
	inicjalizacja_zasilania();
	inicjalizacja_dane();
	inicjalizacja_LED();
	inicjalizacja_USART();
	inicjalizacja_NVIC();

    while(1)
    {
    }
}
