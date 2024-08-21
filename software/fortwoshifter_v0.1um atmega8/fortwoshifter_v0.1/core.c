/*
 * fortwoshifter_v0.1.um.c
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
unsigned char bs_Rpsh;
unsigned char bs_Lpsh;
unsigned char bs_Lpl;
unsigned char bs_Rpl;
unsigned char Rpsh=0;
unsigned char Lpsh=0;
unsigned long A=0; //для счётчика удерживания правого переключателя от себя 
unsigned long i=0; //для счётчика активности кнопки авто
unsigned char eep=0; //для счётчика одной записи в еер
unsigned char eepl=0; //для счётчика одной записи в еер
unsigned char Amode; //текущий режим авто/ручной
unsigned char A1;  //для счётчика однократной активации режима авто при включении
unsigned char Rpsh1; //для счётчика однократной активации драйва
unsigned char Lpsh1; //для счётчика однократной активации реверса
unsigned char N; //для счётчика однократной активации нейтрали
unsigned char up1; //для счётчика однократной активации переключения вверх
unsigned char dn1; //для счётчика однократной активации переключения вниз

void port_ini(void)          //инициализация портов
{
		//DDRD=0xFF;
		//PORTD=0xFD;
		DDRC=0x00;
		PORTC=0xFF;
		
}

void gear_state(unsigned char gs)   //состояния кпп
{
	switch(gs)
	{
		case 1: USART_Transmit('R');   break;            //реверс
		case 2: USART_Transmit('N');   break;             //нейтраль
		case 3: USART_Transmit('D');   break;             //драйв
		case 4: USART_Transmit('-');   break;             //-
		case 5: USART_Transmit('+');   break;             //+
		case 6: USART_Transmit('A');   break;             //авто/ручной
	}
}



int main(void)
{
	
	port_ini();
	Amode=EEPROM_read(1);
	A1=EEPROM_read(1);
	
	USART_Init(12);
	//Передаем при включении
	USART_Transmit('N');

    //sei();
    while (1) 	
    { 
		//драйв
		if (Lpsh==0) //запрет одновременной активации реверса и драйва
		{
					if (PINC&0x04) // запрет одновременного нажатия
					{
						if (bs_Rpsh==0) //включение драйва проверка состояния кнопки
						{
							if (!(PINC&0x01)) //проверка текущего состояния кнопки
							{
								Rpsh=1;
								if (Rpsh1==0)
								{
									gear_state(3); //передача команды
									Rpsh1=1;
								}
								_delay_ms(10);
								if (PINC&0x01) //пока кнопка зажата состояние не переключится
								{
									Rpsh1=0;
									bs_Rpsh=1;
									if ((Amode==1)&(A1==1)) //активация режима авто если он был активен до выключения и проверка однократной активации
									{										
										//for (i=0; i<=10000; i++)//счётчик активности кнопки авто
										//{
										gear_state(6);
										//}
										A1=0;
									}
								}
							}
						}
						else // при состоянии кнопки ==1
						{
							if (!(PINC&0x01)) //проверить кнопку
							{
								A++; //счётчик удержания правого переключателя от себя
								_delay_ms(10);
								if ((PINC&0x01)&&(A<40))// короткое нажатие
								{
									gear_state(2);
									Rpsh=0;
									bs_Rpsh=0; //состояние драйва
									A=0;
								}
								if ((PINC&0x01)&&(A>=40)) //длинное нажатие
								{
									for (i=0; i<1; i++)//счётчик активности кнопки авто
									{
										gear_state(6);
									}
									A=0;
									if (Amode==0)
									{
										EEPROM_write(1, 1); //запись авто режима
										Amode=1;
									}
									else
									{
										EEPROM_write(1, 0); //запись авто режима
										Amode=0;
									}
								}
							}
						}
					}
		}
		//_______//реверс
		if ((PINC&0x01)&&(Rpsh==0)) //запрет одновременной активации реверса и драйва
		{
					if (bs_Lpsh==0)
					{
						if (!(PINC&0x04))
						{
							_delay_ms(10);
							Lpsh=1;
							if (Lpsh1==0)//счётчик активности кнопки реверс
							{
								gear_state(1);
								Lpsh1=1;
							}
							if (PINC&0x04)
							{
								bs_Lpsh=1;
								Lpsh1=0;
							}
						}
					}
					else
					{
						if (!(PINC&0x04))
						{
							_delay_ms(10);
							Lpsh=0;
							if (N==0)
							{
								gear_state(2);
								N=1;
							}
							if (PINC&0x04)
							{
								bs_Lpsh=0;
								N=0;
							}
						}
					}
		}
		//______ручное переключение
		if (bs_Rpsh==1) //передача вверх
		{
			if ((!(PINC&0x02))&(bs_Lpl==0)) //проверка нажатия и запрет одновременного включения + м -
			{
				_delay_ms(10);
				if(up1==0)
				{
					gear_state(5);
					up1=1;
				}
				bs_Rpl=1;
				if (eep<1) //счётчик одной записи
				{
					EEPROM_write(1, 0); //однократная запись ручного режима
// 					gear_state(1); //индикатор записи
// 					_delay_ms(500); 
					eep=1;
					Amode=0;
				}
				if(PINC&0x02)
				{
					up1=0;
				}
			}
			else
			{
				//gear_state(3);
				bs_Rpl=0;
				eep=0;
			}
			//передача вниз
			if ((!(PINC&0x08))&(bs_Rpl==0)) //проверка нажатия и запрет одновременного включения + м -
			{
				_delay_ms(10);
				if (dn1==0)
				{
					gear_state(4);
					dn1=1;
				}

				bs_Lpl=1;
				if (eepl<1) //счётчик одной записи
				{
					EEPROM_write(1, 0); //однократная запись ручного режима
// 					gear_state(1); //индикатор записи
// 					_delay_ms(500); 
					eepl=1;
					Amode=0;
				}
				if (PINC&0x08)
				{
					dn1=0;
				}
			}
			else
			{
				//gear_state(3);
				bs_Lpl=0;
				eepl=0;
			}
		}
    }
}

