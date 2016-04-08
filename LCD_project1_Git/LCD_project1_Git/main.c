/*
 * LCD_project1.c
 *
 * Created: 4/1/2016 9:23:51 PM
 * Author : Mytch Johnson
 */ 

#define F_CPU 16000000UL	// set CPU speed to 16MHz
#define std_delay 50		// set standard delay time to 50us
#define debounce 100		// set debounce time to 50ms
#define func_set 0x38		// 8bit command for function set
#define disp_ctrl 0x0F		// 8bit command for display control
#define disp_clear 0x01		// 8bit command for display clear
#define entry_mode 0x06		// 8bit command for enable entry
#define set_line1 0x02		// address for start of line 2
#define set_line2 0xC0		// address for start of line 1
#define right_button 0x01	// return type for right button
#define left_button 0x02	// return type for left button

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "StringLibTest/StringLib.h"

// function prototypes 
void lcd_write_cmd(char command);
void lcd_write_char(char character);
void lcd_initialize();
void lcd_print_string(char string[]);
void clear_display(); 
uint8_t check_buttons(); 

int main(void)
{
	UCSR0B = 0;				// allows atmel to R/W to pins 0 and 1
	DDRB = 0b00100111;		// Pin11-12 = Input; E, RW, RS, PIN13 = output
	DDRD = 0b11111111;		// DB(7-0) = output	
	
	PORTB |= 0b00011000;	// set internal pull-ups @ pin11/12
	
	//initialize LCD
	lcd_initialize();
	
	// print to the LCD
	lcd_print_string("Hello World!... Goodbye and I");
	
	while(1){
		// if a button is pressed
		if(check_buttons()){
			PORTB |= 0b00100000;	// turn on led
			clear_display();
			lcd_print_string("I like big buttsss and I can not lie!");
		}
		else
			PORTB &= 0b11011111;	// turn off led
	}
	
	return 1;
}

// runs pins and delays to send LCD a command 
void lcd_write_cmd(char command){
	PORTB &= 0b11111000;    // E, RW, RS = 0,0,0
	PORTD = command;		// set command
	PORTB |= 0b00000100;	// E, RW, RS = 1,0,0
	PORTB &= 0b11111000;	// E, RW, RS = 0,0,0
	_delay_us(std_delay);
}

// runs LCD initialization code
void lcd_initialize(){
	_delay_ms(35);					// power ON delay > 30ms
	lcd_write_cmd(func_set);		// run function set
	_delay_us(std_delay);			// delay > 50us
	lcd_write_cmd(disp_ctrl);		// run display set (off)
	_delay_us(std_delay);			// delay > 50us
	lcd_write_cmd(disp_clear);		// run clear display
	_delay_ms(2);					// delay > 1.5 ms
	lcd_write_cmd(entry_mode);		// set entry mode
	_delay_us(std_delay);			// delay > 50us
}

//  runs pins and delays to have LCD print characters 
void lcd_write_char(char character){
	PORTB &= 0b11111000;    // E, RW, RS = 0,0,0
	PORTD = character;		// send character
	PORTB |= 0b00000101;	// E, RW, RS = 1,0,1
	PORTB &= 0b11111000;	// E, RW, RS = 0,0,0
	_delay_us(std_delay);			// delay
}

// check which (if any) buttons are pressed
uint8_t check_buttons(){
	// if pin12 (left) button is pressed
	if(~PINB & 0b00010000){
		_delay_ms(debounce);
		return left_button;
	}
	// if pin11 (right) button is pressed
	else if(~PINB & 0b00001000){
		_delay_ms(debounce);
		return right_button;
	}
	// if no button is pressed
	else
		return 0;
}

// prints string of characters to LCD
void lcd_print_string(char string[]){
/*
   for(int x=0; x<strlen(string); x++){
	if(x==16){
		lcd_write_cmd(set_line2);
		_delay_us(std_delay);
	}
	lcd_write_char(string[x]);
	_delay_us(std_delay);
}
 */
	
	char *line1, *line2, *extra;
   line1 = getLine(string, &extra);
   
	// print top line
	for(int i=0; i<strlen(line1); i++){
		lcd_write_char(line1[i]);
		_delay_us(std_delay);
	}
	
	// set cursor to beginning of line 2
	lcd_write_cmd(set_line2);
   string = extra;
   line2 = getLine(string, &extra);
	
	// print bottom line 
	for(int j=0; j<strlen(line2); j++){
		lcd_write_char(line2[j]);
		_delay_us(std_delay);
	}

}

void clear_display(){
	lcd_write_cmd(disp_clear);
	_delay_ms(2);
}
