#include "naglowki_include.h"

#ifndef _SENSORY_H_
#define _SENSORY_H_

void inicjalizacja_akcelerometr();
void i2c_read( uint8_t adres, uint8_t reg_adres, uint8_t * dane, uint8_t len );
uint8_t i2c_read2(uint8_t adres, uint8_t reg_adres);
void i2c_write(uint8_t address, uint8_t* data, uint32_t length);
void i2c_write2(uint8_t address, uint8_t reg, uint8_t data);
int error_check();

#endif
