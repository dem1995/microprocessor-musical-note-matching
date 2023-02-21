/*
 * ps2_input.h
 *
 *  Created on: May 3, 2019
 *      Author: Ben Korty and DEMcKnight
 */

#ifndef PS2_INPUT_H_
#define PS2_INPUT_H_


//#include "timer.h"
#ifndef TIMER_H_
#include <iostream>
#include "timer.h"
#endif

#include "pin.h"

//Code for reading in PS/2 input

#define PINMODE0 (*(volatile int*)0x4002C040)


/**
 * Reads PS/2 input into the LPC1769 unit.
 */
int keyInput()
{
	PINMODE0 |= (1<<17)|(1<<19);
	int input =0;


	bool lastClk = true; //The state of the last PS/2 clock read (starts high)
	int ps2Bits = 0;	//The PS/2 read-in (we start with nothing)
	int numBits = 0; 	//The number of bits we have read into ps2Bits. We have no bits to start;
	pin ps2DataLine = pin(0, 8);	//the ps/2 data line pin
	pin ps2Clk = pin(0, 9);		//the ps/2 clock pin

	int ps2BitsFinal = 0;			//where to store a full read
	bool fullRead = false;

	pin one_pressed = pin (0,7);
	pin two_pressed = pin (0,0);
	pin three_pressed = pin(0,18);
	pin four_pressed = pin(0,15);
	pin five_pressed = pin(0,23);

	bool clockChanged = ((bool)ps2Clk.read()!=lastClk);
	if (clockChanged) //If the clock changed
	{
		while(numBits<11){
			lastClk = !lastClk; 	//the clock has changed
			if (!(bool)ps2Clk.read()) //if the clock is low, read in new bit into ps2Bits
			{
				ps2Bits = ps2Bits << 1;					//make new room for new bit
				ps2Bits |= ps2DataLine.read();	//insert new bit with bitwise or
				numBits++;						//we've read in one more bit, so increase this counter
			}

			if (numBits == 11)
			{
				fullRead = true;
				ps2BitsFinal = ps2Bits;			//Store fully read bits in ps2BitsFinal
				ps2Bits = 0; 					//Clear ps2Bits
				numBits = 0;					//Clear the number
				ps2BitsFinal = ps2BitsFinal >> 2;
				ps2BitsFinal = ps2BitsFinal & 0b11111111;
				return ps2BitsFinal;
			}
		}
	}
	return 0;
}

#endif

/* PS2_INPUT_H_ */
