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

void odczyt_I2C(uint8_t adres, uint8_t rejestr)
{
	/*while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, 0x30, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, 0x28);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2C2, ENABLE); // Send START condition
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2, 0x30, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
	dane_czujniki.akcel.x_l = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2, DISABLE);



	I2C_GenerateSTOP(I2C2, ENABLE);*/

	uint32_t dummy;

	while(I2C2->SR2 & I2C_SR2_BUSY)
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}

	I2C2->CR1 |= I2C_CR1_START;
	while( !( I2C2->SR1 & I2C_SR1_SB ))
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->DR = adres;
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}
	dummy = I2C2->SR2;
	while( !( I2C2->SR1 & I2C_SR1_TXE ))
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->DR = reg_adres;
	while( !( I2C2->SR1 & I2C_SR1_BTF ))
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->CR1 |= I2C_CR1_START;
	while( !( I2C2->SR1 & I2C_SR1_SB ))
	{
		if(error_check())
		{
			//i2c_config();
			return;
		}
	}
	I2C2->DR = adres | 0x01;
	while( !( I2C2->SR1 & I2C_SR1_ADDR ))
	{
		if(error_check())
		{
			//i2c_config();
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
			   //i2c_config();
			   return;
		   }
		}
		*( dane++ ) = I2C2->DR;

		len--;
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}
