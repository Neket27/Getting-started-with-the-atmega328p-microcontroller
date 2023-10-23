#define F_CPU 8000000L
// Подключение семисегментных индикаторов к AVR. Динамическая индикация. Опрос матричной клавиатуры.  Использование таймера.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


unsigned int pin7=0;
bool pushButton = false;


void initDisplay(){
	DDRB = 0xFF; // Порт B - выход
	PORTB = 0b10000000;
	DDRD = 0xFF; // Порт D - выход
	PORTD = 0x00;
}

void initTimer() {
	// Настройка таймера 1 для прерываний каждые ___ миллисекунд
	
	  // инициализация Timer1
	cli(); // отключить глобальные прерывания
	TCCR1A = 0; // установить регистры в 0
	TCCR1B = 0; 

	OCR1A = 8000; // установка регистра совпадения
	TCCR1B |= (1 << WGM12); // включение в CTC режим

	// Установка битов CS10 на коэффициент деления: 1 - нет деления
	TCCR1B |= (1 << CS21);

	TIMSK1 |= (1 << OCIE1A); // включение прерываний по совпадению
	sei(); // включить глобальные прерывания
}

void initButton()
{
	DDRC = 0x00;// инициализируем порт на вход
	PORTC = 0xFF; // включаем подтягивающие резистры (на пинах будет высокий уровень, кнопка будет притягивать к земле и таким оброзом фиксироваться нажатие)
}



//---------------------------0-----1-----2-----3-----4-----5-----6-----7-----8------9-----10----11----12----13---14----15
unsigned char SEGMENTE[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0xF7, 0xFC, 0x39, 0x5E, 0x76, 0x71  };
unsigned int display = 0;

unsigned char segCounter = 0;

struct segment
{
	int number;
	bool saveNumber = false;
	
};

segment display_1;
segment display_2;
segment display_3;
segment display_4;


void outNumberOnDisplay(int display )
{
	
		
	PORTD = 0xFF; // Гасим все разряды
	PORTB = (1 << segCounter); // Выбираем следующий разряд. Запись (1 << segCounter) устанавливает нужный порт в 1, а все остальные в 0
	//PORTB &= ~(1 << PB0) | ~(1 << PB1) | ~(1 << PB2) | ~(1 << PB3) | ~(1 << PB4) | ~(1 << PB5) | ~(1 << PB6) | ~(1<<PB7);
	PORTB |=  pin7 << PB7;
	
	switch (segCounter)
	{
	case 0:
		PORTD = ~(SEGMENTE[display % 16666 / 1666]); // Раскладываем число на разряды
		break;
	case 1:
		PORTD = ~(SEGMENTE[display % 1666 / 166]);
		break;
	case 2:
		PORTD = PORTD = ~(SEGMENTE[display % 166 / 16]);
		break;
	case 3:
		PORTD = ~(SEGMENTE[display % 16]);
		break;
	}
	//PORTB &= ~(1 << segCounter);
	if (segCounter++ > 2) segCounter = 0;
			
		
}




ISR(TIMER1_COMPA_vect)
{
	if (pushButton) {
		display++;
		//if (display == 9999)
		//	pushButton = false;
			
	}
	//display = 65535;
	
	outNumberOnDisplay(display);
	

			
}



// Главная функция
int main(void)
{
	initDisplay();
	initTimer();
	
	bool resetDisplay = false;
	
	
	while (1)
	{		
		
		if (!(PINC & (1 << PC1))) //если на первом пине низкий уровень (кнопка прижала контакт к земле) выаолняем условие
		{
			pushButton = !pushButton;
			if (pushButton == false && resetDisplay == true) {
				_delay_ms(5000);
				display = 0;
			}
				
			resetDisplay = true;
			_delay_ms(300);
			
		}
		
	
		
	  
	
	}
	return 0;
}







