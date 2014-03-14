#include "sensory.h"
#include "I2C.h"

void inicjalizacja_akcelerometr()
{
	//OBOWI�ZKOWO bez tego nie dzia�a
	i2c_write2(48, 0x20, 151);//ustawienie czestotliwosci
	i2c_write2(48, 0x21, 0); //filtrowanie
	i2c_write2(48, 0x22, 8); //nie wiemy czemu to samo co bit5
	i2c_write2(48, 0x23, 40); //wysoka rozdzielczosci +-8G
	i2c_write2(48, 0x24, 64); //FIFO
}

void i2c_write2(uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t acc[] = {reg, data};
	i2c_write(address, acc, 2);

}

void i2c_write(uint8_t address, uint8_t* data, uint32_t length)
{
	/*//dzia�a przy skomentowanym inicjalizacja_I2C
	uint32_t dummy;

	while(I2C2->SR2 & I2C_SR2_BUSY)
	{
		if(error_check())
		{
			inicjalizacja_I2C();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_START;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if(error_check())
		{
			inicjalizacja_I2C();
			return;
		}
	}
	dummy = I2C2->SR1;
	I2C2->DR = address;
	while (!(I2C2->SR1 & I2C_SR1_ADDR))
	{
		if(error_check())
		{
			inicjalizacja_I2C();
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
				inicjalizacja_I2C();
				return;
			}

		}
		I2C2->DR = *data++;
	}

	while (!(I2C2->SR1 & I2C_SR1_TXE) || !(I2C2->SR1 & I2C_SR1_BTF))
	{
		if(error_check())
		{
			inicjalizacja_I2C();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_STOP;*/
}

uint8_t i2c_read2(uint8_t adres, uint8_t reg_adres)
{
	uint8_t dane[1];
	i2c_read(adres, reg_adres, dane, 1);
	return dane[0];
}

void i2c_read( uint8_t adres, uint8_t reg_adres, uint8_t * dane, uint8_t len )
{


}
