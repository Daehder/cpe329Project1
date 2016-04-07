/*
 * LCD_project1.c
 *
 * Created: 4/1/2016 9:23:51 PM
 * Author : Mytch Johnson
 */ 

#define F_CPU 16000000UL
#define func_set 0x38
#define disp_ctrl 0x0F
#define disp_clear 0x01
#define entry_mode 0x06
#define set_line2 0xC0
#define set_line1 0x02
#define shift_right 0x05
#define shift_left 0x07
#define inc_right 0x06
#define inc_left 0x04

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void lcd_write_cmd(char command);
void lcd_write_char(char character);
void lcd_initialize();
void lcd_print_string(char string[]);

int main(void)
{
	UCSR0B = 0;				// allows atmel to R/W to pins 0 and 1
	DDRB = 0b00100111;		// E, RW, RS, PIN13 = output
	DDRD = 0b11111111;		// DB(7-0) = output
	
	//initialize LCD
	lcd_initialize();
	
	// print to the LCD
	lcd_print_string("dickbutt 0123456789");
	
	return 1;
}

// runs pins and delays to send LCD a command 
void lcd_write_cmd(char command){
	PORTB = 0b00000000;	    // E, RW, RS = 0,0,0
	_delay_us(1);			// delay
	PORTD = command;		// set command
	_delay_us(1);			// delay
	PORTB = 0b00000100;		// E, RW, RS = 1,0,0
	_delay_us(1);			// delay 
	PORTB = 0b00000000;		// E, RW, RS = 0,0,0
	_delay_us(50);
}

//  runs pins and delays to have LCD print characters 
void lcd_write_char(char character){
	PORTB = 0b00000000;	    // E, RW, RS = 0,0,0
	_delay_us(1);			// delay
	PORTD = character;		// send character
	_delay_us(1);			// delay
	PORTB = 0b00000101;		// E, RW, RS = 1,0,1
	_delay_us(1);			// delay
	PORTB = 0b00000000;		// E, RW, RS = 0,0,0
	_delay_us(50);
}

// runs LCD initialization code
void lcd_initialize(){
	_delay_ms(50);			// power ON delay > 30ms
	
	// run setup
	lcd_write_cmd(func_set);		// run function set
	_delay_us(80);					// delay > 4.5ms
	lcd_write_cmd(disp_ctrl);		// run display set (off)
	_delay_us(80);					// delay > 50us
	lcd_write_cmd(disp_clear);		// run clear display
	_delay_ms(3);					// delay > 3ms
	lcd_write_cmd(entry_mode);		// set entry mode
	_delay_us(80);					// delay > 50us
}

// prints string of characters to LCD
void lcd_print_string(char string[]){
	for(int i=0; i<strlen(string); i++){
		if(i == 16){
			lcd_write_cmd(set_line2);
		}
		lcd_write_char(string[i]);
		_delay_us(100);
	}
}
