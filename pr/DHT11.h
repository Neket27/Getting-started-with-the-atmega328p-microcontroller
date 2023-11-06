#pragma once
#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>


#define DHT_DDR		DDRD
#define DHT_PIN		PIND
#define DHT_PORT	PORTD
#define DHT			PD2


#ifndef DHT11_H_
#define DHT11_H_

// функция передачи условия "старт" на линию и проверка ответов от датчика.
uint8_t dht_start(void);
// получение байта от датчика
uint8_t dht_byte(void);
// общая функция опроса датчика
uint8_t dht_read(uint8_t *hum, uint8_t* temp);

#endif /* DHT11_H_ */