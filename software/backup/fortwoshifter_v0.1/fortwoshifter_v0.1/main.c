/*
 * fortwoshifter_v0.1.c
 *
 * Created: 19.12.2022 20:37:47
 * Author : Dude
 */ 
// ��������� �� ������ ���������:
// PD0=������
// PD1=��������
// PD2=�����
// PD3=-
// PD4=+
// PD5=����\������

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
unsigned long A=0; //��� �������� ����������� ������� ������������� �� ���� 
unsigned long i=0; //��� �������� ���������� ������ ����
unsigned char eep=0; //��� �������� ����� ������ � ���
unsigned char eepl=0; //��� �������� ����� ������ � ���
unsigned char Amode; //������� ����� ����/������
unsigned char A1; //��� �������� ����������� ��������� ������ ���� ��� ���������

void port_ini(void)          //������������� ������
{
		DDRD=0xFF;
		PORTD=0xFD;
		DDRC=0x00;
		PORTC=0xFF;
}

void gear_state(unsigned char gs)   //��������� ���
{
	switch(gs)
	{
		case 1: PORTD=0xFE;   break;            //������
		case 2: PORTD=0xFD;   break;             //��������
		case 3: PORTD=0xFB;   break;             //�����
		case 4: PORTD=0xF3;   break;             //-
		case 5: PORTD=0xEB;   break;             //+
		case 6: PORTD=0xDB;   break;             //����/������
	}
}



int main(void)
{
	port_ini();
	Amode=EEPROM_read(1);
	A1=1;
 
    while (1) 	
    { 
		//�����
		if (Lpsh==0) // ������ �������������� �������
		{
					if (PINC&0x04) //������ ������������� ��������� ������� � ������
					{
						if (bs_Rpsh==0) //��������� ������ �������� ��������� ������
						{
							if (!(PINC&0x01)) //�������� �������� ��������� ������
							{
								Rpsh=1;
								gear_state(3);
								_delay_ms(10);
								if (PINC&0x01) //���� ������ ������ ��������� �� ������������
								{
									bs_Rpsh=1;
									if ((Amode==1)&(A1==1)) //��������� ������ ���� ���� �� ��� ������� �� ���������� � �������� ����������� ���������
									{										
										for (i=0; i<=10000; i++)//������� ���������� ������ ����
										{
										gear_state(6);
										}
										A1=0;
									}
								}
							}
						}
						else // ����� ��������� ������ 0
						{
							if (!(PINC&0x01)) //��������� ������
							{
								A++; //������� ��������� ������� ������������� �� ����
								_delay_ms(10);
								if ((PINC&0x01)&(A<40))// �������� �������
								{
									gear_state(2);
									Rpsh=0;
									bs_Rpsh=0; //��������� ������
									A=0;
								}
								if ((PINC&0x01)&(A>=40)) //������� �������
								{
									for (i=0; i<=10000; i++)//������� ���������� ������ ����
									{
										gear_state(6);
									}
									A=0;
									if (Amode==0)
									{
										EEPROM_write(1, 1); //������ ���� ������
										Amode=1;
									}
									else
									{
										EEPROM_write(1, 0); //������ ���� ������
										Amode=0;
									}
								}
							}
						}
					}
		}
		//_______//������
		if ((PINC&0x01)&(Rpsh==0)) //������ ������������� ��������� ������� � ������
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
		//______������ ������������
		if (bs_Rpsh==1) //�������� �����
		{
			if ((!(PINC&0x02))&(bs_Lpl==0)) //�������� ������� � ������ �������������� ��������� + � -
			{
				gear_state(5);
				bs_Rpl=1;
				if (eep<1) //������� ����� ������
				{
					EEPROM_write(1, 0); //����������� ������ ������� ������
// 					gear_state(1); //��������� ������
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
			//�������� ����
			if ((!(PINC&0x08))&(bs_Rpl==0)) //�������� ������� � ������ �������������� ��������� + � -
			{
				gear_state(4);
				bs_Lpl=1;
				if (eepl<1) //������� ����� ������
				{
					EEPROM_write(1, 0); //����������� ������ ������� ������
// 					gear_state(1); //��������� ������
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

