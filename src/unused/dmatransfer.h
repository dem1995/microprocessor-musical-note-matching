/*
 * DMAtransfer.h
 *
 *  Created on: May 1, 2019
 *      Author: kort8201
 */

#ifndef DMATRANSFER_H_
#define DMATRANSFER_H_

#include "src/utils/pin.h"

#define PCONP (*(volatile int*)0x400FC0C4)
#define PINSEL1 (*(volatile int*)0x4002C004)
#define PINMODE1 (*(volatile int*)0x4002C044)
#define DACCTRL (*(volatile int*)0x4008C004) //DAC control register. Used to request data transfer using DMA
#define DACR (*(volatile unsigned int *)0x4008C000)
#define DMACConfig (*(volatile unsigned int *)0x50004030) //Used to activate DMA controller
#define DMACIntTCClear (*(volatile unsigned int *)0x50004008) //Used to clear Terminal Interrupts
#define DMACIntErrClear (*(volatile unsigned int *)0x50004010)//Used to clear Error based Interrupts
#define DMACC0SrcAddr (*(volatile unsigned int *)0x50004100)//Used to indicate to DMA controller the location in memory to read from
#define DMACC0DestAddr (*(volatile unsigned int *)0x50004104)//Used to indicate to DMA controller the location in the DAC subsystem being written to
#define DMACC0LLI (*(volatile unsigned int *)0x50004108)//Used to indicate to DMA controller the location of next linked list item used.
                                                        //In this case, no other items are used per transaction, so register will
#define DMACC0Control (*(volatile unsigned int *)0x5000410C)//Used to indicate the transfer size of the upcoming transaction (in shorts)
#define DMACC0Config (*(volatile unsigned int *)0x50004110)//Used to indicate the transfer size of the upcoming transaction (in shorts)

int DMAtransfer(int inputInts[], int numInputs)
{
	//Setup for DAC
	PINSEL1 |= 1 << 21;                 //Select appropriate mode for output pin for DAC out
	PINSEL1 &= ~(1<<20);

	//Setup for DMA
	PCONP |= 1 << 29;                   //Power the DMA controller
	DMACConfig |= 1;                    // Enable the DMA controller

	//DMA Channel 0 Initiation
	DMACIntTCClear |= 1;                //Clear Interrupts on Channel 0
	DMACIntErrClear |= 1;
	DMACC0SrcAddr = (int)inputInts;         //Write memory location of data to be transferred as SOURCE location
	DMACC0DestAddr = 0x4008C000;        //Write DACR as location to be transferred to aka DESTINATION location
	DMACC0LLI = 0;                      //Point next linked list item address to 0

	//DMA Channel 0 Control
	DMACC0Control |= numInputs;          //Sets the number of shorts to be transferred.  4 bytes per int.
	DMACC0Control &= ~(0b111<<12);      //Sets source burst size to 1. Burst is number of transfer per transaction.
	DMACC0Control &= ~(0b111<<15);      //Sets destination burst size to 1. Burst is number of transfer per transaction.
	DMACC0Control |= (1<<19);            //Sets source transfer width to 32 bits (0b010 for 32 bits)
	DMACC0Control |= (1<<22);            //Sets destination transfer width to 32 bits (0b010 for 32 bits)
	DMACC0Control |= 1<<26;             //Enables source address incrementation
	DMACC0Control &= ~(1<<27);          //Disables destination address incrementation

	DMACC0Config;

	//DMA Channel 0 Configuration
	DMACC0Config = 1<<15;               //Masks TC Interrupt
	DMACC0Config |= (7<<6)|(1<<11);      //Sets DAC peripheral as transfer destination (7), and transfer type as memory to peripheral (0b001)
	DMACC0Config |= 1;                  //Enables Channel 0. Channel is now active.

	//Request a DMA transfer
	DACCTRL |= 1 <<3;                   //Enable DMA Burst Requests for DAC to DMA

	while((DMACC0Config&1)==1)
	{
		DMACC0Config;
		DACR;
	}

	int output = DACR>>6;
	output = output&0b1111111111;
	return output;
}

#endif /* DMATRANSFER_H_ */
