#include "I2C.h"
#include "dane.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_I2C()
{
	/*konfiguracja gpio
	 *
	 */
	GPIO_InitTypeDef  *i2c_gpio = malloc (sizeof (GPIO_InitTypeDef));
	i2c_gpio->GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	i2c_gpio->GPIO_Speed = GPIO_Speed_50MHz;
	i2c_gpio->GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, i2c_gpio);
	free(i2c_gpio);

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
	I2C_GenerateSTART(I2C2,ENABLE);
	//while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C2,0x30,I2C_Direction_Transmitter);
	//while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	I2C_SendData(I2C2,0x28);
	I2C_GenerateSTART(I2C2,ENABLE);
	I2C_SendData(I2C2,0x31);
	dane_czujniki.akcel.x_l=I2C_ReceiveData(I2C2);
	I2C_GenerateSTOP(I2C2,ENABLE);
}
