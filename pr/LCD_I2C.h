/*
 * lcd_i2c.c
 *
 * Created: 04/09/2017 07:37:47 p. m.
 * Author : falco
 */ 

#include <avr/io.h>
//Yo asigne el pin 0x02 como el En de la LCD
//y el pin 0x01 como el pin RS de la LCD
//Los pines de los datos los mбs altos del modulo a los de la lcd P4-D4,P5-D5,P6-D6,P7-D7
//R/W va a tierra

//-----Proto-Type-------//
void lcd_init();			//----LCD Initializing Function
void toggle();				//----Latching function of LCD
void lcd_cmd_hf(char v1);   //----Function to send half byte command to LCD
void lcd_cmd(char v2);		//----Function to send Command to LCD
void lcd_dwr(char v3);		//----Function to send data to LCD
void lcd_msg(char *c);		//----Function to Send String to LCD
void delay(int ms);			//----Delay function
void lcd_lef_sh();			//----Left Shifting Function
void lcd_rig_sh();			//----Right Shifting Function
//----------------------//

void toggle()
{
	TWDR |= 0x02;					//---PIN En de la LCD en = 1;  -----Latching data in to LCD data register using High to Low signal (подаём высокий уровень на pin En, для фиксация данных в жидкокристаллическом регистре данных с использованием сигнала от высокого к низкому)
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Enable I2C and clear interrupt- Esta linea y la siguiente simepre van despues de querer mandar un coamndo por TDWR (Включение I2C и очищение прерываний - есть линия связи и время, необходимое для выполнения запроса мандата команды для TDWR)
	while  (!(TWCR &(1<<TWINT)));	//---Simepre poner despues de la linea anterior al mandar datos por TWDR (Всегда проверяется на успешность после предыдущей строки при отправке данных через TWDR)
	delay(1);
	TWDR &= ~0x02;					//---PIN del Enable de la LCD en = 0; (Ставим назкий уровень на pin En дисплея)
	TWCR = (1<<TWINT) | (1<<TWEN);	//---Enable I2C and clear interrupt (Включение I2C и очищение прерываний )
	while  (!(TWCR &(1<<TWINT)));   // Всегда проверяется на успешность после предыдущей строки при отправке данных через TWDR
}

void lcd_cmd_hf(char v1)
{
	TWDR &=~0x01;					//PIN RS de la LCD rs = 0; ----Selecting register as Command register (устанавливаем pin Rs дисплея в низкий уровень)
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt  (Включение I2C и очищение прерываний )
	while  (!(TWCR &(1<<TWINT)));   // проверка успешности
	
	TWDR &= 0x0F;					//----clearing the Higher 4 bits (очистка старших 4 бит) 0F=00001111
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt  (Включение I2C и очищение прерываний )
	while  (!(TWCR &(1<<TWINT)));   // проверка успешности
	
	TWDR |= (v1 & 0xF0);			//----Masking higher 4 bits and sending to LCD (Маскирование более старших 4 бит и отправка на ЖК-дисплей) f0=11110000
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt (Включение I2C и очищение прерываний )
	while  (!(TWCR &(1<<TWINT)));   // проверка успешности
	toggle();
}

void lcd_cmd(char v2)
{
	TWDR&=~0x01;					//rs = 0; ----Selecting register as command register
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;                   //----clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (v2 & 0xF0);			//----Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	toggle();
	
	TWDR &= 0x0F;                    //----clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= ((v2 & 0x0F)<<4);		//----Masking lower 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}

void lcd_dwr(char v3)
{
	TWDR|=0x01;						//rs = 1; ----Selecting register as command register
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR &= 0x0F;				    //----clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= (v3 & 0xF0);			//----Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	toggle();
	
	TWDR &= 0x0F;					//----clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	TWDR |= ((v3 & 0x0F)<<4);		//----Masking lower 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	//Enable I2C and clear interrupt
	while  (!(TWCR &(1<<TWINT)));
	toggle();
}

void lcd_init()
{
	lcd_cmd_hf(0x30);       //-----Sequence for initializing LCD
	lcd_cmd_hf(0x30);       //-----     "            "              "               "
	lcd_cmd_hf(0x20);       //-----     "            "              "               "
	lcd_cmd(0x28);          //-----Selecting 16 x 2 LCD in 4Bit mode
	lcd_cmd(0x0C);          //-----Display ON Cursor OFF
	lcd_cmd(0x01);          //-----Clear display
	lcd_cmd(0x06);          //-----Cursor Auto Increment
	lcd_cmd(0x80);          //-----1st line 1st location of LCD
}

void delay(int ms)
{
	int i,j;
	for(i=0;i<=ms;i++)
	for(j=0;j<=120;j++);
}

void lcd_msg(char *c)
{
	while(*c != 0)      //----Wait till all String are passed to LCD
	lcd_dwr(*c++);		//----Send the String to LCD
}

void lcd_rig_sh()
{
	lcd_cmd(0x1C);      //----Command for right Shift
	delay(400);
}

void lcd_lef_sh()
{
	lcd_cmd(0x18);      //----Command for Left Shift
	delay(200);
}
