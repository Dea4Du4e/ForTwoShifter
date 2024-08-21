/*
 * USART.c
 *
 * Created: 13.12.2022 21:14:43
 *  Author: Dude
 */ 
#include "USART.h"
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// void USART_Init (long USART_BAUDRATE)
// {
// 	UCSRB |= (1 << RXEN) | (1 << TXEN);	/* Turn on transmission and reception */
// 	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit char size */
// 	UBRRL = BAUD_PRESCALE;			/* Load lower 8-bits of the baud rate */
// 	UBRRH = (BAUD_PRESCALE >> 8);		/* Load upper 8-bits*/
// 	//DDRB=0xFF;
// 	//PORTB=0x00;
// }

void USART_Init (unsigned int speed) //инициализация модуля USART
{
	UBRRH = (unsigned char)(speed>>8);
	UBRRL = (unsigned char)speed;
	UCSRB = (1<<RXEN)|(1<<TXEN); //включаем передачу и приём по USART
	UCSRB|= (1<<RXCIE); //разрешаем прерывание при передаче
	UCSRA|=(1<<U2X); // удвоенная скорост передачи данных
	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); //обращаемся именно к регистру UCSRC(URSEL=1), асинхронный режим (UMSEL=0), без контроля чётности (UPM1=0 и UPM0=0),
	//1 стоп-бит (USBS=0), 8-бит посылка (UCSZ1=1 UCSZ0=1) UCSRC |= (1<<UPM1); чётность

}
void USART_Transmit(unsigned char data) //функция отправки данных
{

	while(!(UCSRA&(1<<UDRE))); //ожидание опустошения буфера передачи
	UDR=data; //начало передачи данных
	_delay_us(1000);

	
}