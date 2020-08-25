/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "keypad.h"
#endif

unsigned char x;
static unsigned char count;

typedef struct task {
    signed char state;
    unsigned long int period;
    unsigned long int elapsedTime;
    int (*TickFct)(int);
} task;

enum State{on} state;

int tick() {
    x=GetKeypadKey();
    return state;
}

enum State2{locked,sequence,hold,unlocked} state2;

int tick2() {
    static unsigned char lock=0x00;
    unsigned char button=(~PINB&0x80)>>7;
    switch (state2) {
	case locked:
	    if (x=='#') {
		state2=hold;
		count=0;
	    }
	    break;
	case sequence:
	    if (count==1) {
		if (x=='\0') {
		    state2=sequence;
		}
		else if (x=='1') {
		    state2=hold;
		}
		else {
		    state2=locked;
		}
	    }
	    else if (count==2) {
                if (x=='\0') {
                    state2=sequence;
                }
                else if (x=='2') {
                    state2=hold;
                }
                else {
                    state2=locked;
                }
            }
	    else if (count==3) {
                if (x=='\0') {
                    state2=sequence;
                }
                else if (x=='3') {
                    state2=hold;
                }
                else {
                    state2=locked;
                }
            }
	    else if (count==4) {
                if (x=='\0') {
                    state2=sequence;
                }
                else if (x=='4') {
                    state2=hold;
                }
                else {
                    state2=locked;
                }
            }
	    else if (count==5) {
                if (x=='\0') {
                    state2=sequence;
                }
                else if (x=='5') {
                    state2=hold;
                }
                else {
                    state2=locked;
                }
            }
	    break;
	case hold:
	    if (count==0) {
		if (x=='#') {
		    state2=hold;
		}
		else if (x=='\0') {
		    state2=sequence;
		    count=1;
		}
	    }
	    else if (count==1) {
                if (x=='1') {
                    state2=hold;
                }
                else if (x=='\0') {
                    state2=sequence;
                    count=2;
                }
            }
	    else if (count==2) {
                if (x=='2') {
                    state2=hold;
                }
                else if (x=='\0') {
                    state2=sequence;
                    count=3;
                }
	    }
	    else if (count==3) {
                if (x=='3') {
                    state2=hold;
                }
                else if (x=='\0') {
                    state2=sequence;
                    count=4;
                }
            }
	    else if (count==4) {
                if (x=='4') {
                    state2=hold;
                }
                else if (x=='\0') {
                    state2=sequence;
                    count=5;
                }
            }
	    else if (count==5) {
                if (x=='5') {
                    state2=hold;
                }
                else if (x=='\0') {
		    lock=0x01;
                    state2=unlocked;
                }
            }
	    break;
	case unlocked:
	    if (button) {
		lock=0x00;
		state=locked;
	    }
	    break;
	default:
	    state2=locked;
    }
    PORTB=lock;
    return state2;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB=0x7F;PORTB=0x80;
    DDRC=0xF0;PORTC=0x0F;
    /* Insert your solution below */
    const unsigned char SIZE=2;
    const unsigned char PERIOD=1;
    task tasks[SIZE];

    tasks[0].state=on;
    tasks[0].period=1;
    tasks[0].elapsedTime=tasks[0].period;
    tasks[0].TickFct=&tick;

    tasks[1].state=locked;
    tasks[1].period=1;
    tasks[1].elapsedTime=tasks[1].period;
    tasks[1].TickFct=&tick2;

    TimerSet(PERIOD);
    TimerOn();
    while (1) {
	for (unsigned short i=0;i<SIZE;i++) {
	    if (tasks[i].elapsedTime==tasks[i].period) {
		tasks[i].state=tasks[i].TickFct(tasks[i].state);
		tasks[i].elapsedTime=0;
	    }
	    tasks[i].elapsedTime+=PERIOD;
	}
	switch(x) {
	    case '\0': PORTB=0x00; break;
	    case '1': PORTB=0x80; break;
	    case '2': PORTB=0x80; break;
	    case '3': PORTB=0x80; break;
            case '4': PORTB=0x80; break;
	    case '5': PORTB=0x80; break;
            case '6': PORTB=0x80; break;
            case '7': PORTB=0x80; break;
            case '8': PORTB=0x80; break;
            case '9': PORTB=0x80; break;
	    case 'A': PORTB=0x80; break;
            case 'B': PORTB=0x80; break;
            case 'C': PORTB=0x80; break;
            case 'D': PORTB=0x80; break;
            case '*': PORTB=0x80; break;
            case '0': PORTB=0x80; break;
            case '#': PORTB=0x80; break;
	    default: PORTB=0x1B;
	}
	while(!TimerFlag) {}
        TimerFlag=0;
    }
    return 1;
}
