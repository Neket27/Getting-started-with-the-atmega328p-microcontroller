/* Heder file for I2C */

#include <avr/io.h>

#define  F_CPU 8000000UL

void i2c_init(){
	TWBR = 0x62;		// �������� �������� ������
	TWCR = (1<<TWEN);	//Enable I2C
	TWSR = 0x00;		//Prescaler set to 1 

}
		//Start condition
void i2c_start(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//start condition (��������� ���������)
	while (!(TWCR & (1<<TWINT)));				//check for start condition (�������� ��������� �������)

}
		//I2C stop condition
void i2c_write(char x){				//Cpn esta funcion se escribe en el bus de TWDR ( ������� ���������� ������ �� ���� TWDR)
	TWDR = x;						//Move value to I2C (���������� �������� �� ����)
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt (��������� I2C � �������� ����������)
	while  (!(TWCR &(1<<TWINT)));   //�������, ��� ������ ������ �������
}

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	//Enable I2C and clear interrupt (��������� I2C � �������� ����������)
	while (!(TWCR & (1<<TWINT)));	//Read successful with all data received in TWDR (������ ��������� ������� �� ����� �������, ����������� � TWDR)
	return TWDR;
}