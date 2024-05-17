/*
 * LCD_Keypad_training.c
 *
 * Created: 1/7/2024 1:39:51 PM
 * Author : mahmo
 */ 

#define F_CPU 1000000ul
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#include "Keypad.h"

/* pin configuration
control pis
data pins
8bit
3 control pins port d
8 data pins    port c
*/

// macros control pins  hardware config
#define RS 1
#define RW 2
#define EN 3

// direction registers
#define LCD_DATA_DIR DDRC
#define LCD_control_DIR DDRD

//port registers data
#define LCD_DATA_PORT PORTC
#define LCD_control_PORT PORTD

// macros
#define setbit(reg,bit) reg|=(1<<bit)		// set bit 1
#define clearbit(reg,bit) reg&=~(1<<bit)	// clear bit 0
#define toggelbit(reg,bit) reg^=(1<<bit)	// toggle
#define readbit(reg,bit) ((reg>>bit)&1)		// input read


#define clear 0x01
// command function
// function input

void LCD_send_data_4bit(unsigned char data)
{
	LCD_DATA_PORT = (LCD_DATA_PORT &0x0f) |(data & 0xf0);//send high nibble
	setbit(LCD_control_PORT,RS);
	clearbit(LCD_control_PORT,RW);
	setbit(LCD_control_PORT,EN);
	_delay_us(1);
	clearbit(LCD_control_PORT,EN);
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT &0x0f) |(data<<4);//send low nibble
	setbit(LCD_control_PORT,EN);
	_delay_us(1);
	clearbit(LCD_control_PORT,EN);
	_delay_ms(1);
	
}
void LCD_send_command_4bit(unsigned char cmd)
{
	LCD_DATA_PORT = (LCD_DATA_PORT &0x0f) |(cmd & 0xf0);//send high nibble
	clearbit(LCD_control_PORT,RS);
	clearbit(LCD_control_PORT,RW);
	setbit(LCD_control_PORT,EN);
	_delay_us(1);
	clearbit(LCD_control_PORT,EN);
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT &0x0f) |(cmd<<4);//send low nibble
	setbit(LCD_control_PORT,EN);
	_delay_us(1);
	clearbit(LCD_control_PORT,EN);
	_delay_ms(1);
}

void LCD_INIT_4bit(void)
{
	LCD_DATA_DIR|= 0xf0;
	setbit(LCD_control_DIR,EN);
	setbit(LCD_control_DIR,RS);
	setbit(LCD_control_DIR,RW);
	LCD_send_command_4bit(0x02);// return home
	LCD_send_command_4bit(0x28);// 2 line  and 4 bit mode
	LCD_send_command_4bit(0x0c);// display on
	LCD_send_command_4bit(0x06);// auto increment
	LCD_send_command_4bit(0x01);// clear screen
	_delay_ms(20);
	
	
}
void LCD_send_string_4bit( char* str)
{
	int count =0;
	while(str[count]!=0)
	{
		LCD_send_data_4bit(str[count]);
		count++;
	}
}

void intToStr(int n, char* str)
{
	int sign=n;
	int i=0;
	if(n<0)
	{
		n=-n;
	}
	while(n>0)
	{
		str[i++]= n%10 +'0';
		n= n/10;
		//i++;
	}
	if(sign<0)
	{
		str[i++]= '-';
	}
	str[i]= '\0';
	
	for(int j=0, k=i-1; j<k; j++,k--)
	{
		char temp= str[j];
		str[j]= str[k];
		str[k]= temp;
	}
	LCD_send_string_4bit(str);
}


int main()
{
	LCD_INIT_4bit();
	keypad_init();
	char resultStr[5];
	char Key;
	int num1=0, num2=0, result=0 ;
	char oper=0;
	int isfirstnumber= 1;
	
	LCD_send_string_4bit("Beginning...");
	_delay_ms(2000);
	LCD_send_command_4bit(0x01);
	
	while(1)
	{
		
		Key= get_key_value();
		
		if(Key >= '0' && Key <= '9')
		{
			LCD_send_data_4bit(Key);
			int digit= Key - '0';
			
			if(isfirstnumber)
			{
				num1= num1 *10 + digit;
			}
			else
			{
				num2= num2*10 + digit;
			}
		}
		else if(Key== '+' || Key== '-' || Key== '*' || Key== '/')
		{
			LCD_send_data_4bit(Key);
			oper= Key;
			isfirstnumber= 0;
		}
		
		else if(Key== '=')
		{
			LCD_send_data_4bit(Key);
			switch (oper)
			{
				case '+':
				  result= (num1) + (num2);
				  break;
				case '-':
				  result= (num1) - (num2);
				  break;
				case '*':
				  result= (num1) * (num2);
				  break;
				 case '/':
				   result= (num1) / (num2);
				   break;
				 default:
				   break;
			}
			
			intToStr(result, resultStr);
			//LCD_send_string_4bit(resultStr);
			
		}
		else if(Key== 'C')
		{
			LCD_send_command_4bit(0x01);
			num1 = 0; // Reset num1 to 0
			num2 = 0; // Reset num2 to 0
			result = 0; // Reset result to 0
			isfirstnumber = 1; // Reset isfirstnumber to 1
			continue;
		}
		
	}
}




















/*
int main() {
	LCD_INIT_4bit();  // Initialize LCD
	keypad_init();    // Initialize keypad

	char key;
	int number1 = 0, number2 = 0, result = 0;
	char operator = 0;
	bool isFirstNumber = true;

	while (1) {
		key = get_key_value();

		if (key >= '0' && key <= '9') 
		{ // If key is a digit
			LCD_send_data_4bit(key); // Display pressed key
			int digit = key - '0';
			if (isFirstNumber) 
			{
				number1 = number1 * 10 + digit; // Construct first number
			} 
			else 
			{
				number2 = number2 * 10 + digit; // Construct second number
			}
		} 
		else if (key == '+' || key == '-' || key == '*' || key == '/') 
		{
			operator = key;
			isFirstNumber = false;
			LCD_send_data_4bit(key); // Display operator
		} 
		else if (key == '=') 
		{
			// Perform the calculation
			LCD_send_data_4bit(key); // Display operator
			switch (operator) 
			{
				case '+':
				result = number1 + number2;
				break;
				case '-':
				result = number1 - number2;
				break;
				case '*':
				result = number1 * number2;
				break;
				case '/':
				if (number2 != 0) 
				{
					result = number1 / number2;
				} 
				else 
				{
					// Handle division by zero
					// You can display an error or reset the calculation
					continue;
				}
				break;
			}

			// Convert result to string and display
			char resultStr[16];
			intToStr(result, resultStr);
			LCD_send_string_4bit(resultStr);

			// Reset for the next calculation
			//number1 = 0; number2 = 0; result = 0;
			//isFirstNumber = true;
		}
		else if((key == 'C'))
		{
			LCD_send_command_4bit(0x01);
		}
	}
}

*/