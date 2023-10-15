#define F_CPU 8000000L
 #include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>


void port_ini(void){
	 PORTC=0b00000010; //  1 пин на выход
	  DDRC=0b00000010; // выход высокого уровня на одном пине
	  
	 PORTB=0x00; // 
	 DDRB=0b00100000; 
	 
	
}


void impulse()
{
	PORTB |= 1<<5;				// импульс вкл (пин 5 в высокий уровень)
	_delay_ms(600);     // длительность импульса
	PORTB &= ~(1<<5);			// импульс выкл  (пин 5 в низкий уровень)
}

int main(void)
{
    /* Replace with your application code */
	port_ini();
    while (1) 
    {
		if( !(PINC & (1 << 1)) ) //если на первом пине низкий уровень (кнопка прижала контакт к земле) делаем импульс
			impulse();
			
    }
}
