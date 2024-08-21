/*
 * USART.c
 *
 * Created: 13.12.2022 21:14:43
 *  Author: Dude
 */ 
#include "USART.h"
void USART_Init (unsigned int speed) //инициализация модуля USART
{
	UBRRH = (unsigned char)(speed>>8);
	UBRRL = (unsigned char)speed;
	UCSRB = (1<<RXEN)|(1<<TXEN); //включаем передачу и приём по USART
	UCSRB|= (1<<RXCIE); //разрешаем прерывание при передаче
	UCSRA|=(1<<U2X); // удвоенная скорост ьпередачи данных
	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); //обращаемся именно к регистру UCSRC(URSEL=1), асинхронный режим (UMSEL=0), без контроля чётности (UPM1=0 и UPM0=0),
	//1 стоп-бит (USBS=0), 8-бит посылка (UCSZ1=1 UCSZ0=1) UCSRC |= (1<<UPM1); чётность
	DDRA=0x01;
	PORTA=0x00;
}
void USART_Transmit(unsigned char data) //функция отправки данных
{
	PORTA=0x01;
	while(!(UCSRA&(1<<UDRE))); //ожидание опустошения буфера приёма
	UDR=data; //начало передачи данных
	PORTA=0x00;
}