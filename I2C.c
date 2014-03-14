#include "I2C.h"
#include "dane.h"
#include "sensory.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_I2C_GPIO()
{
	//konfiguracja portów - SPRAWDZONE jest OK
	GPIO_InitTypeDef  *i2c_gpio = malloc (sizeof (GPIO_InitTypeDef));
	i2c_gpio->GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	i2c_gpio->GPIO_Speed = GPIO_Speed_50MHz;
	i2c_gpio->GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, i2c_gpio);
	free(i2c_gpio);
}

void inicjalizacja_I2C()
{
	//SPRAWDZONE jest OK
	I2C_InitTypeDef  *i2c = malloc (sizeof (I2C_InitTypeDef));

	/* I2C configuration */
	i2c->I2C_Mode = I2C_Mode_I2C;
	i2c->I2C_DutyCycle = I2C_DutyCycle_2;
	i2c->I2C_OwnAddress1 = 0x55;
	i2c->I2C_Ack = I2C_Ack_Enable;
	i2c->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c->I2C_ClockSpeed = 100000;

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C2, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C2, i2c);
}

void odczyt_I2C(uint8_t adres, uint8_t rejestr, uint8_t dlugosc, uint8_t *bufor)
{
	while(I2C2->SR2 & I2C_SR2_BUSY) //sprawdzanie czy I2C wolny
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_START; //Wyslanie START
	while( !( I2C2->SR1 & I2C_SR1_SB )) //sprawdzanie czy siê wyslalo
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->DR = adres; //przed zapisaniem SR2, trzeba do DR zapisaæ adres
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->SR2; //konieczne wg datasheet, czyszczenie rejestru SR1
	while( !( I2C2->SR1 & I2C_SR1_TXE ))
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}

	/*
	wys³anie nr rejestru do czytania, najbardziej
	znacz¹cy bit musi byæ równy 1 */

	I2C2->DR = rejestr;

	while( !( I2C2->SR1 & I2C_SR1_BTF ))
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->CR1 |= I2C_CR1_START; //Wyslanie START
	while( !( I2C2->SR1 & I2C_SR1_SB ))
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->DR = adres | 0x01; //Wyslanie adresu czytania
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(sprawdz_blad_I2C())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->SR2; //konieczne wg datasheet, czyszczenie rejestru SR1

	I2C2->CR1 |= I2C_CR1_ACK; //wlaczenie potwierdzenia master
	for(; dlugosc > 0; dlugosc-- )
	{
		if( dlugosc == 1 )
		I2C2->CR1 &= ~I2C_CR1_ACK; //przy ostatnim wylaczenie potwierdzenia

		while( !( I2C2->SR1 & I2C_SR1_RXNE ))
		{
		   if(sprawdz_blad_I2C())
		   {
			   //i2c_config();
			   return;
		   }
		}
		*(bufor++) = I2C2->DR;
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}

int sprawdz_blad_I2C()
{
	if((I2C2->SR1 & I2C_SR1_TIMEOUT) | (I2C2->SR1 & I2C_SR1_PECERR) | (I2C2->SR1 & I2C_SR1_AF) | (I2C2->SR1 & I2C_SR1_ARLO) |
			(I2C2->SR1 & I2C_SR1_BERR))
	{
		//sensors_error_flag = 1;
		return 1;
	}
	return 0;

}
