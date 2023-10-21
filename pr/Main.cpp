#define F_CPU 8000000L
// Подключение семисегментных индикаторов к AVR. Динамическая индикация. Опрос матричной клавиатуры.  Использование таймера.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char key;

// Функция для инициализации порта DDRC
void initKeypad() {
	DDRC = 0b0000111; // Настройка нижних 4 бит порта как выходы и верхних 4 бит как входы
	PORTC =0b1111111; // Подключение подтягивающих резисторов к верхним 4 битам
}

// Функция для опроса клавиатуры
char readKeypad() {
	PORTC |= 0b1111111; // Установим нижние 3 бита в высокий уровень и верхние 4 в режим чтения
	char col, row;
	for (col = 0; col < 3; col++) {
		// Устанавливаем нижние 3 бита поочередно в 0
		PORTC &= ~(1 << col);
		for (row = 3; row < 7; row++) {
			if (!(PINC & (1 << row))) {
				// Клавиша нажата
				// Вернем символ в зависимости от положения клавиши
				if (col == 0 && row == 3) return '1';
				if (col == 1 && row == 3) return '2';
				if (col == 2 && row == 3) return '3';
				
				
				if (col == 0 && row == 4) return '4';
				if (col == 1 && row == 4) return '5';
				if (col == 2 && row == 4) return '6';
				
				if (col == 0 && row == 5) return '7';
				if (col == 1 && row == 5) return '8';
				if (col == 2 && row == 5) return '9';
				
				if (col == 1 && row == 6) return '0';
				
			}
		}
		PORTC |= 0b1111111; // Установим нижние 3 бита в высокий уровень и верхние 4 в режим чтения
		
	}
	return 0; // Ни одна клавиша не нажата
}

//---------------------------0-----1-----2-----3-----4-----5-----6-----7-----8------9
unsigned char SEGMENTE[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

unsigned char segCounter = 0;
unsigned int display = 0;


void initTimer() {
	// Настройка таймера 0 для прерываний каждые 100 миллисекунд
	TCCR0A = 0x00;
	TCCR0B = (1 << CS01) |(1 << CS00); // Предделитель 64
	TIMSK0 = (1 << TOIE0); // Разрешить прерывание по переполнению таймера
}


ISR(TIMER0_OVF_vect) {
	key = readKeypad();
	
	if (key != 0) {
		// Обработка нажатой клавиши
		
		display = key - '0';
	}
		
	
	
	PORTD = 0xFF; // Гасим все разряды
	PORTB = (1 << segCounter); // Выбираем следующий разряд
	
	switch (segCounter)
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
	if (segCounter++ > 2) segCounter = 0;
	

	
}



// Главная функция
int main(void)
{
	DDRB = 0xFF; // Порт B - выход
	PORTB = 0x00;
	DDRD = 0xFF; // Порт D - выход
	PORTD = 0x00;
	
	initKeypad();
	
	initTimer();
	sei(); // Разрешить глобальные прерывания


    
		
	while (1)
	{
			
		
			
	}
    
	return 0;
	
}







