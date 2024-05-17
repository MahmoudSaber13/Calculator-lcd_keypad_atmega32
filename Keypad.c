/*
 * Keypad.c
 *
 * Created: 1/7/2024 3:23:10 PM
 *  Author: mahmo
 */ 

// Keypad.c

#include <avr/io.h>

// macros
#define setbit(reg,bit) reg|=(1<<bit)		// set bit 1
#define clearbit(reg,bit) reg&=~(1<<bit)	// clear bit 0
#define toggelbit(reg,bit) reg^=(1<<bit)	// toggle
#define readbit(reg,bit) ((reg>>bit)&1)		// input read

#include "Keypad.h"

void keypad_init(void)   // 1st step
{
	// 8 bits p0 --> p7
	// 8 pins 4 / 4   4 rows --> power loop --> output   4 columns --> input
	// 4 rows p0 p1 p2 p3 output power
	// columns p4 p5 p6 p7 input reading pin
	
	////////////////// output pins /////////////////
	setbit(keypad_out_dir, 0);  //DDRA
	setbit(keypad_out_dir, 1);
	setbit(keypad_out_dir, 2);
	setbit(keypad_out_dir, 3);
	
	/////////////////// Input Pins ///////////////////
	clearbit(keypad_in_dir, 4);
	clearbit(keypad_in_dir, 5);
	clearbit(keypad_in_dir, 6);
	clearbit(keypad_in_dir, 7);
	/////////////////// internal pull up resistor for input pins /////////
	//input power
	
	setbit(keypad_in_port,4); // porta= 0000100 -- > pull up input & port
	setbit(keypad_in_port,5);
	setbit(keypad_in_port,6);
	setbit(keypad_in_port,7); // noise
}

void switch_output(int x)
{
	// power loop function for rows
	
	switch(x)
	{
		// output r0 r1 r2 r3 high to low
		case 0:
		clearbit(keypad_out_port, 0); //r0 low
		setbit(keypad_out_port, 1);
		setbit(keypad_out_port, 2);
		setbit(keypad_out_port, 3);
		break;
		case 1:
		clearbit(keypad_out_port,1); // r1 low
		setbit(keypad_out_port, 0);
		setbit(keypad_out_port, 2);
		setbit(keypad_out_port, 3);
		break;
		case 2:
		clearbit(keypad_out_port,2); // r2 low
		setbit(keypad_out_port, 1);
		setbit(keypad_out_port, 0);
		setbit(keypad_out_port, 3);
		break;
		case 3:
		clearbit(keypad_out_port,3); // r3 low
		setbit(keypad_out_port, 1);
		setbit(keypad_out_port, 2);
		setbit(keypad_out_port, 0);
		break;
		default:
		break;
	}
}

char get_key_value(void)
{
	char key= notpressed; //255
	
	while(key == notpressed) // key= '1' break condition
	{
		switch_output(0);
		if(readbit(keypad_in_pin, 4)==0) // pull up power --> low
		{
			key= '7'; // char
			while(readbit(keypad_in_pin,4)==0); // skip loop infinity
			// there is no pressing anymore
		}
		else if (readbit(keypad_in_pin,5)==0)
		{
			key= '8';
			while(readbit(keypad_in_pin,5)==0);
		}
		else if (readbit(keypad_in_pin,6)==0)
		{
			key= '9';
			while(readbit(keypad_in_pin,6)==0);
		}
		else if (readbit(keypad_in_pin,7)==0)
		{
			key= '/';
			while(readbit(keypad_in_pin,7)==0);
		}
		
		switch_output(1);
		if(readbit(keypad_in_pin,4)==0)
		{
			key= '4';
			while(readbit(keypad_in_pin,4)==0);
		}
		else if(readbit(keypad_in_pin,5)==0)
		{
			key= '5';
			while(readbit(keypad_in_pin,5)==0);
		}
		else if(readbit(keypad_in_pin,6)==0)
		{
			key= '6';
			while(readbit(keypad_in_pin,6)==0);
		}
		else if(readbit(keypad_in_pin,7)==0)
		{
			key= '*';
			while(readbit(keypad_in_pin,7)==0);
		}
		
		switch_output(2);
		if(readbit(keypad_in_pin,4)==0)
		{
			key= '1';
			while(readbit(keypad_in_pin,4)==0);
		}
		else if(readbit(keypad_in_pin,5)==0)
		{
			key= '2';
			while(readbit(keypad_in_pin,5)==0);
		}
		else if(readbit(keypad_in_pin,6)==0)
		{
			key= '3';
			while(readbit(keypad_in_pin,6)==0);
		}
		else if(readbit(keypad_in_pin,7)==0)
		{
			key= '-';
			while(readbit(keypad_in_pin,7)==0);
		}
		
		switch_output(3);
		if(readbit(keypad_in_pin,4)==0)
		{
			key= 'C';
			while(readbit(keypad_in_pin,4)==0);
		}
		else if(readbit(keypad_in_pin,5)==0)
		{
			key= '0';
			while(readbit(keypad_in_pin,5)==0);
		}
		else if(readbit(keypad_in_pin,6)==0)
		{
			key= '=';
			while(readbit(keypad_in_pin,6)==0);
		}
		else if(readbit(keypad_in_pin,7)==0)
		{
			key= '+';
			while(readbit(keypad_in_pin,7)==0);
		}
	}
	return key;
}