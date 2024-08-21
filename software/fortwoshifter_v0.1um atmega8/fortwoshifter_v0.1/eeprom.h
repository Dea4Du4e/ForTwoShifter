/*
 * eeprom.h
 *
 * Created: 25.12.2022 13:41:50
 *  Author: Dude
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "core.h"
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);

unsigned char EEPROM_read(unsigned int uiAddress);



#endif /* EEPROM_H_ */