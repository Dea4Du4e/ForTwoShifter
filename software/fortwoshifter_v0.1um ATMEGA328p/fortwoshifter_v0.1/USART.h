/*
 * USART.h
 *
 * Created: 13.12.2022 21:14:18
 *  Author: Dude
 */ 


#ifndef USART_H_
#define USART_H_

#include "core.h"
void USART_Init(unsigned int speed);
void USART_Transmit(unsigned char data);


#endif /* USART_H_ */