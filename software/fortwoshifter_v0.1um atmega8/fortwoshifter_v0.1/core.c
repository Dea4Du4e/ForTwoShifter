/*
 * fortwoshifter_v0.1.um.c
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
#include "core.h"
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
unsigned char A1;  //��� �������� ����������� ��������� ������ ���� ��� ���������
unsigned char Rpsh1; //��� �������� ����������� ��������� ������
unsigned char Lpsh1; //��� �������� ����������� ��������� �������
unsigned char N; //��� �������� ����������� ��������� ��������
unsigned char up1; //��� �������� ����������� ��������� ������������ �����
unsigned char dn1; //��� �������� ����������� ��������� ������������ ����

void port_ini(void)          //������������� ������
{
		//DDRD=0xFF;
		//PORTD=0xFD;
		DDRC=0x00;
		PORTC=0xFF;
		
}

void gear_state(unsigned char gs)   //��������� ���
{
	switch(gs)
	{
		case 1: USART_Transmit('R');   break;            //������
		case 2: USART_Transmit('N');   break;             //��������
		case 3: USART_Transmit('D');   break;             //�����
		case 4: USART_Transmit('-');   break;             //-
		case 5: USART_Transmit('+');   break;             //+
		case 6: USART_Transmit('A');   break;             //����/������
	}
}



int main(void)
{
	
	port_ini();
	Amode=EEPROM_read(1);
	A1=EEPROM_read(1);
	
	USART_Init(12);
	//�������� ��� ���������
	USART_Transmit('N');

    //sei();
    while (1) 	
    { 
		//�����
		if (Lpsh==0) //������ ������������� ��������� ������� � ������
		{
					if (PINC&0x04) // ������ �������������� �������
					{
						if (bs_Rpsh==0) //��������� ������ �������� ��������� ������
						{
							if (!(PINC&0x01)) //�������� �������� ��������� ������
							{
								Rpsh=1;
								if (Rpsh1==0)
								{
									gear_state(3); //�������� �������
									Rpsh1=1;
								}
								_delay_ms(10);
								if (PINC&0x01) //���� ������ ������ ��������� �� ������������
								{
									Rpsh1=0;
									bs_Rpsh=1;
									if ((Amode==1)&(A1==1)) //��������� ������ ���� ���� �� ��� ������� �� ���������� � �������� ����������� ���������
									{										
										//for (i=0; i<=10000; i++)//������� ���������� ������ ����
										//{
										gear_state(6);
										//}
										A1=0;
									}
								}
							}
						}
						else // ��� ��������� ������ ==1
						{
							if (!(PINC&0x01)) //��������� ������
							{
								A++; //������� ��������� ������� ������������� �� ����
								_delay_ms(10);
								if ((PINC&0x01)&&(A<40))// �������� �������
								{
									gear_state(2);
									Rpsh=0;
									bs_Rpsh=0; //��������� ������
									A=0;
								}
								if ((PINC&0x01)&&(A>=40)) //������� �������
								{
									for (i=0; i<1; i++)//������� ���������� ������ ����
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
		if ((PINC&0x01)&&(Rpsh==0)) //������ ������������� ��������� ������� � ������
		{
					if (bs_Lpsh==0)
					{
						if (!(PINC&0x04))
						{
							_delay_ms(10);
							Lpsh=1;
							if (Lpsh1==0)//������� ���������� ������ ������
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
		//______������ ������������
		if (bs_Rpsh==1) //�������� �����
		{
			if ((!(PINC&0x02))&(bs_Lpl==0)) //�������� ������� � ������ �������������� ��������� + � -
			{
				_delay_ms(10);
				if(up1==0)
				{
					gear_state(5);
					up1=1;
				}
				bs_Rpl=1;
				if (eep<1) //������� ����� ������
				{
					EEPROM_write(1, 0); //����������� ������ ������� ������
// 					gear_state(1); //��������� ������
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
			//�������� ����
			if ((!(PINC&0x08))&(bs_Rpl==0)) //�������� ������� � ������ �������������� ��������� + � -
			{
				_delay_ms(10);
				if (dn1==0)
				{
					gear_state(4);
					dn1=1;
				}

				bs_Lpl=1;
				if (eepl<1) //������� ����� ������
				{
					EEPROM_write(1, 0); //����������� ������ ������� ������
// 					gear_state(1); //��������� ������
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

