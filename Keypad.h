/*
 * Keypad.h
 *
 * Created: 1/7/2024 3:23:32 PM
 *  Author: mahmo
 */ 

#ifndef KEYPAD_H_    //multi calling for header
#define KEYPAD_H_


// header file .h not .c
// compiler header +++ c fíle
// c-file compilation process --> assembly --> machine lang

/*
// #include <stdio.h>
//

int main()
{
	printf("fbckjip");
}
*/

//linker --> header stdio.h printf --> main.c --> complete compilation

/*
functions --> function body
          --> prototype     ==> header function
		  proto in header file keypad.h
		  function body c file keypad.c
		  
		  #define x 0
		  #define s 0
		  #define a 0
		  #define z 0
header file

keypad.h

keypad.c --> calling header keypad.h
 
 main.c
 
 compiler main function 95% right
 
 */

//macro
#define keypad_out_dir  DDRA
#define keypad_out_port PORTA
#define keypad_in_dir   DDRA
#define keypad_in_pin   PINA
#define keypad_in_port  PORTA

void keypad_init(void);   //proto
char get_key_value(void);

#define notpressed 0xff //255 0b11111111

#endif /* KEYPAD_H_ */

