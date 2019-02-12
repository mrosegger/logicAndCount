/*
 * LogicAndCount.cpp
 *
 * Created: 05/02/2019 12:01:01
 * Author : mrosegger
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
volatile int counter;


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
	
	DDRD = 255;
	
	/* Replace with your application code */
    while (1) 
    {
		//continue;
		int bt0 = 1;
		int bt1 = 1;
		int bt2 = 1;
		
		//Read in input
		if (PINC &= (1<<PINC0))
		{
			bt0 = 0;
		}
		if (PINC &= (1<<PINC1))
		{
			bt1 = 0;
		}
		if (PINC &= (1<<PINC2))
		{
			bt2 = 0;
		}
		
		//Logic
		unsigned char logicOr = bt0|bt1|bt2;
		unsigned char logicAnd = bt0&bt1&bt2;
		unsigned char logicXor = (bt2^bt0^bt1);
		
		//Logic Output
		PORTD &= ((1<<PORTD2) |(1<<PORTD3) |(1<<PORTD4));
		PORTD |= (logicOr<<PORTD5);
		PORTD |= (logicAnd<<PORTD6);
		PORTD |= (logicXor<<PORTD7);
		
    }	
}
	
//Timer Interrupt 
ISR(TIMER1_COMPA_vect)
{
	//Counter Output
	PORTD &= ((1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7));
	PORTD |= counter<<2;
	//Counter Reset
	if (counter++ == 7)
	{
		counter = 0;
	}
}

