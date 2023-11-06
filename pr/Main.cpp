 
#define  F_CPU 8000000L
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "DHT11.h"
#include "i2c.h"
#include "LCD_I2C.h"


char buf[16];
uint8_t hum, temp=0; // переменные для влажности и температуры


int main(void)
{
	i2c_init();
	i2c_start();
	i2c_write(0X70);
	lcd_init();
	lcd_cmd(0x80);
	lcd_msg("Neket");
	lcd_msg(buf);
	
		while (1)
		{
			dht_read(&hum, &temp); // опроса датчика

			lcd_cmd(0xC0);
			sprintf(buf, "%dC %dRH;\r\n", temp, hum);
			lcd_msg(buf);		

		}

	return 0;

	}



