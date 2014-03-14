#include "sensory.h"

void inicjalizacja_akcelerometr()
{
	i2c_config();

	uint8_t accaddress = 48;

	/*GPIOB->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7  | GPIO_CRL_CNF0);
GPIOB->CRL |= GPIO_CRL_MODE1 | GPIO_CRL_CNF1_1 | GPIO_CRL_MODE6 | GPIO_CRL_CNF6_1 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1;

GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF8 | GPIO_CRH_CNF9);
GPIOB->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF11 |
		GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14;*/

i2c_write2(accaddress, 0x20, 151);//ustawienie czestotliwosci
	i2c_write2(accaddress, 0x21, 0); //filtrowanie
	i2c_write2(accaddress, 0x22, 8); //nie wiemy czemu to samo co bit5
	i2c_write2(accaddress, 0x23, 40); //wysoka rozdzielczosc +-8G
	i2c_write2(accaddress, 0x24, 64); //FIFO

}

void i2c_config()
{
	/*I2C2->CR1 &= ~I2C_CR1_PE;
	I2C2->CR1 |= I2C_CR1_SWRST;
	I2C2->CR1 &= ~I2C_CR1_SWRST;

	I2C2->TRISE = 37;               // limit slope
	I2C2->CCR = 30;               // setup speed (100kHz)
	I2C2->CR2 |= 36;      // config I2C2 module

	I2C2->CR1 |= I2C_CR1_PE;// enable peripheral*/
}

void i2c_write2(uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t acc[] = {reg, data};
	i2c_write(address, acc, 2);

}

void i2c_write(uint8_t address, uint8_t* data, uint32_t length)
{
	uint32_t dummy;

	while(I2C2->SR2 & I2C_SR2_BUSY)
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_START;
	while(!(I2C2->SR1 & I2C_SR1_SB))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	dummy = I2C2->SR1;
	I2C2->DR = address;
	while (!(I2C2->SR1 & I2C_SR1_ADDR))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	dummy = I2C2->SR1;
	dummy = I2C2->SR2;

	while (length--)
	{
		while (!(I2C2->SR1 & I2C_SR1_TXE))
		{
			if(error_check())
			{
				i2c_config();
				return;
			}

		}
		I2C2->DR = *data++;
	}

	while (!(I2C2->SR1 & I2C_SR1_TXE) || !(I2C2->SR1 & I2C_SR1_BTF))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}

uint8_t i2c_read2(uint8_t adres, uint8_t reg_adres)
{
	uint8_t dane[1];
	i2c_read(adres, reg_adres, dane, 1);
	return dane[0];
}

void i2c_read( uint8_t adres, uint8_t reg_adres, uint8_t * dane, uint8_t len )
{
	uint32_t dummy;

	while(I2C2->SR2 & I2C_SR2_BUSY)
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_START;
	while( !( I2C2->SR1 & I2C_SR1_SB ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	I2C2->DR = adres;
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	dummy = I2C2->SR2;
	while( !( I2C2->SR1 & I2C_SR1_TXE ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	I2C2->DR = reg_adres;
	while( !( I2C2->SR1 & I2C_SR1_BTF ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	I2C2->CR1 |= I2C_CR1_START;
	while( !( I2C2->SR1 & I2C_SR1_SB ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	I2C2->DR = adres | 0x01;
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(error_check())
		{
			i2c_config();
			return;
		}
	}
	dummy = I2C2->SR2;

	 I2C2->CR1 |= I2C_CR1_ACK;
	while( len )
	{
	   if( len == 1 )
	      I2C2->CR1 &= ~I2C_CR1_ACK;

	   while( !( I2C2->SR1 & I2C_SR1_RXNE ))
	   {
		   if(error_check())
		   	{
			   i2c_config();
		   		return;
		   	}
	   }
	   *( dane++ ) = I2C2->DR;

	   len--;
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}

int error_check()
{
	if((I2C2->SR1 & I2C_SR1_TIMEOUT) | (I2C2->SR1 & I2C_SR1_PECERR) | (I2C2->SR1 & I2C_SR1_AF) | (I2C2->SR1 & I2C_SR1_ARLO) |
			(I2C2->SR1 & I2C_SR1_BERR))
	{
		//sensors_error_flag = 1;
		return 1;
	}
	return 0;

}
