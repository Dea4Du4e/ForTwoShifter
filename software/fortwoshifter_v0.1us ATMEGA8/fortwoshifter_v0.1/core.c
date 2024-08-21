/*
 * fortwoshifter_v0.1.us.c
 *
 * Created: 19.12.2022 20:37:47
 * Author : Dude
 */ 
// пояснение по работе программы:
// PD0=реверс
// PD1=нейтраль
// PD2=драйв
// PD3=-
// PD4=+
// PD5=авто\ручной

// PC0=Rup
// PC1=Rdn
// PC2=Ldn
// PC3=Lup

#include <avr/io.h>
#include "core.h"


void port_ini(void)          //инициализация портов
{
		DDRC=0xFF;
		PORTC=0x0A;
		//DDRC=0x00;
		//PORTC=0xFF;
}



void gear_state(unsigned char gs)   //состояния кпп
{
	switch(gs)
	{
		case 1: PORTC=0x09;   break;            //реверс
		case 2: PORTC=0x05;   break;             //нейтраль
		case 3: PORTC=0x06;   break;             //драйв
		case 4: PORTC=0x0C;   break;             //-
		case 5: PORTC=0x03;   break;             //+
		case 6: PORTC=0x19;   break;             //авто/ручной
	}
}

ISR(USART_RXC_vect)

{
	int R;
	unsigned long U;
	unsigned long D;
	unsigned long Amode;
	R = UDR;
				if(R=='R')
				{
					gear_state(1);
				}
				if (R=='N')
				{
					gear_state(2);
				}
				if (R=='D')
				{
					gear_state(3);
				}
				if (R=='A')
				{
					for (Amode=0; Amode<100000; Amode++)
					{
						gear_state(6);
					}
					gear_state(3);
				}
				if (R=='-')
				{
					for (D=0; D<100000; D++)
					{
						gear_state(4);
					}
					gear_state(3);
				}
				if (R=='+')
				{
					for (U=0; U<100000; U++)
					{
						gear_state(5);
					}
					gear_state(3);
				}
}

int main(void)
{
	
	port_ini();

	USART_Init(8);
	USART_Transmit('8');
	
	sei();
    while (1) 	
    { 
    }
}

