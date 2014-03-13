#include "I2C.h"
#include "dane.h"

extern volatile daneTypeDef dane_czujniki;

void inicjalizacja_I2C()
{
	I2C2->CR1
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
