/*
 * fortwoshifter_v0.1.c
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
#include "main.h"
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
unsigned char A1; //для счётчика однократной активации режима авто при включении

void port_ini(void)          //инициализация портов
{
		DDRD=0xFF;
		PORTD=0xFD;
		DDRC=0x00;
		PORTC=0xFF;
}

void gear_state(unsigned char gs)   //состояния кпп
{
	switch(gs)
	{
		case 1: PORTD=0xFE;   break;            //реверс
		case 2: PORTD=0xFD;   break;             //нейтраль
		case 3: PORTD=0xFB;   break;             //драйв
		case 4: PORTD=0xF3;   break;             //-
		case 5: PORTD=0xEB;   break;             //+
		case 6: PORTD=0xDB;   break;             //авто/ручной
	}
}



int main(void)
{
	port_ini();
	Amode=EEPROM_read(1);
	A1=1;
 
    while (1) 	
    { 
		//драйв
		if (Lpsh==0) // запрет одновременного нажатия
		{
					if (PINC&0x04) //запрет одновременной активации реверса и драйва
					{
						if (bs_Rpsh==0) //включение драйва проверка состояния кнопки
						{
							if (!(PINC&0x01)) //проверка текущего состояния кнопки
							{
								Rpsh=1;
								gear_state(3);
								_delay_ms(10);
								if (PINC&0x01) //пока кнопка зажата состояние не переключится
								{
									bs_Rpsh=1;
									if ((Amode==1)&(A1==1)) //активация режима авто если он был активен до выключения и проверка однократной активации
									{										
										for (i=0; i<=10000; i++)//счётчик активности кнопки авто
										{
										gear_state(6);
										}
										A1=0;
									}
								}
							}
						}
						else // иначе состояние кнопки 0
						{
							if (!(PINC&0x01)) //проверить кнопку
							{
								A++; //счётчик удержания правого переключателя от себя
								_delay_ms(10);
								if ((PINC&0x01)&(A<40))// короткое нажатие
								{
									gear_state(2);
									Rpsh=0;
									bs_Rpsh=0; //состояние драйва
									A=0;
								}
								if ((PINC&0x01)&(A>=40)) //длинное нажатие
								{
									for (i=0; i<=10000; i++)//счётчик активности кнопки авто
									{
										gear_state(6);
									}
									A=0;
									if (Amode==0)
									{
										EEPROM_write(1, 1); //хапись авто режима
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
		if ((PINC&0x01)&(Rpsh==0)) //запрет одновременной активации реверса и драйва
		{
					if (bs_Lpsh==0)
					{
						if (!(PINC&0x04))
						{
							Lpsh=1;
							gear_state(1);
							_delay_ms(10);
							if (PINC&0x04)
							{
								bs_Lpsh=1;
							}
						}
					}
					else
					{
						if (!(PINC&0x04))
						{
							Lpsh=0;
							gear_state(2);
							_delay_ms(10);
							if (PINC&0x04)
							{
								bs_Lpsh=0;
							}
						}
					}
		}
		//______ручное переключение
		if (bs_Rpsh==1) //передача вверх
		{
			if ((!(PINC&0x02))&(bs_Lpl==0)) //проверка нажатия и запрет одновременного включения + м -
			{
				gear_state(5);
				bs_Rpl=1;
				if (eep<1) //счётчик одной записи
				{
					EEPROM_write(1, 0); //однократная запись ручного режима
// 					gear_state(1); //индикатор записи
// 					_delay_ms(500); 
					eep=1;
					Amode=0;
				}
			}
			else
			{
				gear_state(3);
				bs_Rpl=0;
				eep=0;
			}
			//передача вниз
			if ((!(PINC&0x08))&(bs_Rpl==0)) //проверка нажатия и запрет одновременного включения + м -
			{
				gear_state(4);
				bs_Lpl=1;
				if (eepl<1) //счётчик одной записи
				{
					EEPROM_write(1, 0); //однократная запись ручного режима
// 					gear_state(1); //индикатор записи
// 					_delay_ms(500); 
					eepl=1;
					Amode=0;
				}
			}
			else
			{
				gear_state(3);
				bs_Lpl=0;
				eepl=0;
			}
		}
    }
}

