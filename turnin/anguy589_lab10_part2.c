/*	Author: anguy589
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	demo link: https://drive.google.com/file/d/1Ifu_nv2gOr0oHdUGIWITawHIHqjKBMVJ/view?usp=sharing
 */
#include <avr/io.h>
#include <keypad.h>
#include <timer.h>
#include <bit.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char x = 0x00;
unsigned char tempB = 0x00;
unsigned char lock = 0x00;

unsigned char masterKey[5] = { 0x01, 0x02, 0x03, 0x04, 0x05};

enum sm1states {input}; 
enum sm2states {locked, enable, unlocked, c1, c2, c3, c4, c5};


int keyTick(int state) {
	x = GetKeypadKey();

	switch(state) {
		case input:
			break;
		default: 
			break;
	
	}

	switch(state) {
		case input:
	        	switch(x) {
        	        	case '\0': tempB = 0x1F; break;
                		case '1': tempB = 0x01; break;
                		case '2': tempB = 0x02; break;
               			case '3': tempB = 0x03; break;
                		case '4': tempB = 0x04; break;
                		case '5': tempB = 0x05; break;
                		case '6': tempB = 0x06; break;
                		case '7': tempB = 0x07; break;
                		case '8': tempB = 0x08; break;
                		case '9': tempB = 0x09; break;
                		case 'A': tempB = 0x0A; break;
                		case 'B': tempB = 0x0B; break;
                		case 'C': tempB = 0x0C; break;
                		case 'D': tempB = 0x0D; break;
                		case '*': tempB = 0x0E; break;
                		case '0': tempB = 0x00; break;
                		case '#': tempB = 0x0F; break;
                		default: tempB = 0x1B; break;
        		}
			break;
		default:
			break;
	}

	return state;

}

int doorTick(int state) {
	lock = ~PINB & 0x80;

	switch(state) {
		case locked:
			if(tempB == 0x0F) {
				state = enable;
			}
			break;
		case enable:
			state = c1;
			break;
		case c1:
			if(tempB == 0x01) {state = c2;}
			break;
		case c2:
			if(tempB == 0x02) {state = c3;}
			break;
		case c3:
			if(tempB == 0x03) {state = c4;}
			break;
		case c4:
			if(tempB == 0x04) {state = c5;}
			break;
		case c5:
			if(tempB == 0x05) {state = unlocked;}
			break;
		case unlocked:	
			if(lock) {
				state = locked;
			}
			break;
	}

	switch(state) {
		case locked:
			PORTB = 0x00;
			PORTA = 0x00;
			break;
		case enable:
			PORTA = 0x0F;
			break;
		case c1:
			PORTA = tempB;
			break;
		case c2:
			PORTA = tempB;
			break;
		case c3:
			PORTA = tempB;
			break;
		case c4:
			PORTA = tempB;
			break;
		case c5:
			PORTA = tempB;
			break;
		case unlocked:
			PORTB = 0x01;
			break;
		default:
			break;
	
	}

	return state;
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x0F; PORTB = 0xF0;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRA = 0xFF; PORTA = 0x00;
	
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks= sizeof(tasks)/sizeof(task*);
	
	const char start = 0;

	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keyTick;

	task2.state = start;
	task2.period = 500;
	task2.elapsedTime = task1.period;
	task2.TickFct = &doorTick;

	TimerSet(50);
	TimerOn();



	unsigned short i;
    /* Insert your solution below */
    while (1) {
	
    	for(i = 0; i < numTasks; i++) {
		if(tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
			
		}
		tasks[i]->elapsedTime += 50;
	}

	while(!TimerFlag);
	TimerFlag = 0;
	
       
    }

    return 1;

}
