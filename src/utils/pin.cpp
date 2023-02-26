/*
 * Pin.cpp
 *
 *  Created on: Feb 6, 2019
 *      Author: David "Dawn" Estes McKnight
 */
#include "pin.h"

pin::pin()
{
	_port_number = 0;
	_bit_number = 0;
}

pin::pin(const pin& obj)
{
	_port_number = obj._port_number;
	_bit_number = obj._bit_number;
}

pin::pin(unsigned int port_number, unsigned int bit_number)
{
	_port_number = port_number;
	_bit_number = bit_number;
}

pin::pin(unsigned int port_number, unsigned int bit_number, bool value) : pin(port_number, bit_number)
{
	write(value);
}


void pin::operator=(bool value)
{
	write(value);
}

void pin::write(bool value)
{
	FIO[_port_number].FIODIR |= 1<<_bit_number;
	FIO[_port_number].FIOPIN &= ~(1<<_bit_number);
	FIO[_port_number].FIOPIN |= value<<_bit_number;
}

pin::operator bool()
{
	return read();
}

bool pin::read()
{
	FIO[_port_number].FIODIR &= ~(1<<_bit_number);
	return (FIO[_port_number].FIOPIN >> _bit_number) & 1;
}
