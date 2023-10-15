#define F_CPU 8000000L
// Подключение семисегментных индикаторов к AVR. Динамическая индикация
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//---------------------------0-----1-----2-----3-----4-----5-----6-----7-----8------9
unsigned char SEGMENTE[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

unsigned char segcounter = 0;
unsigned int display = 0;

// Обработчик прерывания по переполнению таймера 2
ISR(TIMER2_OVF_vect)
{
	PORTD = 0xFF; // Гасим все разряды
	PORTB = (1 << segcounter); // Выбираем следующий разряд
	
	switch (segcounter)
	{
	case 0:
		PORTD = ~(SEGMENTE[display % 10000 / 1000]); // Раскладываем число на разряды
		break;
	case 1:
		PORTD = ~(SEGMENTE[display % 1000 / 100]);
		break;
	case 2:
		PORTD = ~(SEGMENTE[display % 100 / 10]);
		break;
	case 3:
		PORTD = ~(SEGMENTE[display % 10]);
		break;
	}
	if (segcounter++ > 2) segcounter = 0;
}
// Главная функция
int main(void)
{
	DDRB = 0xFF; // Порт B - выход
	PORTB = 0x00;
	DDRD = 0xFF; // Порт D - выход
	PORTD = 0x00;
	
	TCCR2B |= (1 << CS21); // Предделитель на 8
	TIMSK2 |= (1 << TOIE2); // Разрешение прерывания по таймеру 2
	
	sei(); // Глобально разрешаем прерывания
	
	while (1)
	{
		display++; // Увеличиваем счет от 0 до 9999
		if (display > 9999) display = 0;
		_delay_ms(10); // Задержка
	}
}