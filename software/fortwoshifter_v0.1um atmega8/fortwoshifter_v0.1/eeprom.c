/*
 * eeprom.c
 *
 * Created: 25.12.2022 13:42:53
 *  Author: Dude
 */ 
#include "eeprom.h"
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)

{

	while(EECR & (1<<EEWE)) //���� ������������ ����� ��������� ��������� ��������� � �������

	{}

	EEAR = uiAddress; //������������� �����

	EEDR = ucData; //����� ������ � �������

	EECR |= (1<<EEMWE); //��������� ������

	EECR |= (1<<EEWE); //����� ���� � ������

}

unsigned char EEPROM_read(unsigned int uiAddress)

{

	while(EECR & (1<<EEWE))

	{} //���� ������������ ����� ��������� ��������� ��������� � �������

	EEAR = uiAddress; //������������� �����

	EECR |= (1<<EERE); //��������� �������� ���������� �� ������ � ������� ������

	return EEDR; //���������� ���������

}