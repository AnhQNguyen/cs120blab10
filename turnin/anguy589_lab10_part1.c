/*	Author: anguy589
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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

enum sm1states {input}; 


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
			
			if(tempB != 0x1F) {
				PORTB = tempB | 0x80;
			}
			else {
				PORTB = 0x1F;
			}
			break;
		default:
			break;
	}

	return state;

}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks= sizeof(tasks)/sizeof(task*);
	
	const char start = 0;

	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keyTick;

	TimerSet(100);
	TimerOn();



	unsigned short i;
    /* Insert your solution below */
    while (1) {
	
    	for(i = 0; i < numTasks; i++) {
		if(tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
			
		}
		tasks[i]->elapsedTime += 100;
	}

	while(!TimerFlag);
	TimerFlag = 0;
	
       
    }

    return 1;

}
