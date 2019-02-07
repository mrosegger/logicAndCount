/*
 * LogicAndCount.cpp
 *
 * Created: 05/02/2019 12:01:01
 * Author : mrosegger
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
int counter;


int main(void)
{
    //  Config Timer:
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536) 1 sec = 15624; 0,5s = 7812
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
	
	sei();
	
	unsigned const char BUTTON_MASK =	(1<<PINC0)|(1<<PIND1)|(1<<PIND2);
	
	/* Replace with your application code */
    while (1) 
    {
		unsigned char pins = PINC & BUTTON_MASK;
		pins =~ pins;
		pins = (BUTTON_MASK&pins);
		int bt0 = 0;
		int bt1 = 0;
		int bt2 = 0;
		
		if (PINC &= (1<<PINC0))
		{
			bt0 = 1;
		}
		if (PINC &= (1<<PINC1))
		{
			bt1 = 1;
		}
		if (PINC &= (1<<PINC1))
		{
			bt2 = 1;
		}
		
		unsigned char logicOr = bt0|bt1|bt2;
		unsigned char logicAnd = bt0&bt1&bt2;
		unsigned char logicXor = (bt0&~bt1&~bt2) | (~bt0&bt1&~bt2) | (~bt0&~bt1&bt2);
		
		PORTD |= (logicOr<<PIND5);
		PORTD |= (logicAnd<<PIND6);
		PORTD |= (logicXor<<PIND7);
		
    }	
}

ISR(TIMER1_COMPA_vect)
{
	counter++;
	PORTD = counter<<2;
	if (counter == 7)
	{
		counter = 0;
	}
}

