/* Heder file for I2C */

#include <avr/io.h>

#define  F_CPU 8000000UL

void i2c_init(){
	TWBR = 0x62;		// Скорость передачи данных
	TWCR = (1<<TWEN);	//Enable I2C
	TWSR = 0x00;		//Prescaler set to 1 

}
		//Start condition
void i2c_start(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//start condition (начальное состояние)
	while (!(TWCR & (1<<TWINT)));				//check for start condition (проверка состояния запуска)

}
		//I2C stop condition
void i2c_write(char x){				//Cpn esta funcion se escribe en el bus de TWDR ( функция записывает данные на шину TWDR)
	TWDR = x;						//Move value to I2C (перемещаем значение на шину)
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt (Включение I2C и очищение прерываний)
	while  (!(TWCR &(1<<TWINT)));   //прверка, что запись прошла успешна
}

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	//Enable I2C and clear interrupt (Включение I2C и очищение прерываний)
	while (!(TWCR & (1<<TWINT)));	//Read successful with all data received in TWDR (Чтение завершено успешно со всеми данными, полученными в TWDR)
	return TWDR;
}