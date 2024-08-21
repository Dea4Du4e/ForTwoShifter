/*
 * USART.c
 *
 * Created: 13.12.2022 21:14:43
 *  Author: Dude
 */ 
#include "USART.h"
void USART_Init (unsigned int speed) //������������� ������ USART
{
	UBRRH = (unsigned char)(speed>>8);
	UBRRL = (unsigned char)speed;
	UCSRB = (1<<RXEN)|(1<<TXEN); //�������� �������� � ���� �� USART
	UCSRB|= (1<<RXCIE); //��������� ���������� ��� ��������
	UCSRA|=(1<<U2X); // ��������� ������� ��������� ������
	UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); //���������� ������ � �������� UCSRC(URSEL=1), ����������� ����� (UMSEL=0), ��� �������� �������� (UPM1=0 � UPM0=0),
	//1 ����-��� (USBS=0), 8-��� ������� (UCSZ1=1 UCSZ0=1) UCSRC |= (1<<UPM1); ��������
	DDRA=0x01;
	PORTA=0x00;
}
void USART_Transmit(unsigned char data) //������� �������� ������
{
	PORTA=0x01;
	while(!(UCSRA&(1<<UDRE))); //�������� ����������� ������ �����
	UDR=data; //������ �������� ������
	PORTA=0x00;
}