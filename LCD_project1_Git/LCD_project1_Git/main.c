/*
 * CPE 329 Project 1 "Hello World"
 * LCD Display Interface using GPIOs
 *
 * Created: 4/1/2016 9:23:51 PM
 * Author : Mytch Johnson and Erik Miller
 * Class  : CPE 329 w/ John Oliver
 * Quarter: Spring 2016
 */ 

//////////////////////////////PREPROCESSING/////////////////////////////////////

#define F_CPU 16000000UL	// set CPU speed to 16MHz
#define std_delay 50		// set standard delay time to 50us
#define debounce 250		// set debounce time to 50ms
#define func_set 0x38		// 8bit command for function set
#define disp_ctrl 0x0F		// 8bit command for display control
#define disp_clear 0x01		// 8bit command for display clear
#define entry_mode 0x06		// 8bit command for enable entry
#define set_line1 0x02		// address for start of line 2
#define set_line2 0xC0		// address for start of line 1
#define right_button 0x01	// return type for right button
#define left_button 0x02	// return type for left button
#define pin13 0x20			// pin13 LED port

// Included libraries
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
void delay_sec(uint16_t ms);
void clear_display(); 
uint8_t check_buttons();
void ui_text(char text_string[], uint16_t freq_mHz, uint16_t duty);
void led_driver_pin13(uint16_t freq_mHz, uint16_t duty);


/////////////////////////////MAIN FUNCTION//////////////////////////////////////

int main(void)
{
	UCSR0B = 0;				// allows atmel to R/W to pins 0 and 1
	DDRB = 0b00100111;		// Pin11-12 = Input; E, RW, RS, PIN13 = output
	DDRD = 0b11111111;		// DB(7-0) = output	
	
	PORTB |= 0b00011000;	// set internal pull-ups @ pin11/12
	
	//initialize LCD
	lcd_initialize();
	
	while(1){
		ui_text("And what happened, then?", 0, 5);
		ui_text("Well, ", 0, 10);
		ui_text("in Whoville they say", 0, 30);
		ui_text("- that the Grinch's small", 0, 30);
		ui_text("heart grew three sizes that day.", 0, 60);
		ui_text("And then - the true meaning of", 0, 60);
		ui_text("Christmas came through,", 0, 80);
		ui_text("and the Grinch found the", 0, 100);
		ui_text("strength of *ten* Grinches,", 0, 100);
		ui_text("plus two!", 1000, 50);
		ui_text("Merry Holidays! Replay?", 1000, 50);
		
	}
	
	return 1;
}

//////////////////////////////HELPER FUNCTIONS//////////////////////////////////

// toggles pins and runs delays to send LCD a command 
void lcd_write_cmd(char command){
	PORTB &= 0b11111000;    // E = 0
	PORTD = command;		// set command
	PORTB |= 0b00000100;	// E = 0
	PORTB &= 0b11111000;	// E = 0
	_delay_us(std_delay);	// delay
}

// runs LCD initialization code
void lcd_initialize(){
	_delay_ms(35);					// power ON delay > 30ms
	lcd_write_cmd(func_set);		// run function set
	_delay_us(std_delay);			// delay > 40us
	lcd_write_cmd(disp_ctrl);		// run display set 
	_delay_us(std_delay);			// delay > 40us
	lcd_write_cmd(disp_clear);		// run clear display
	_delay_ms(2);					// delay > 1.5 ms
	lcd_write_cmd(entry_mode);		// set entry mode
	_delay_us(std_delay);			// delay > 40us
}

//  toggles pins and runs delays for have LCD character writing  
void lcd_write_char(char character){
	PORTB &= 0b11111000;    // E = 0
	PORTD = character;		// send character
	PORTB |= 0b00000101;	// E = 1; RS = 1
	PORTB &= 0b11111000;	// E = 0; RS = 0
	_delay_us(std_delay);	// delay
}

// returns which button is press or returns 0 if not pressed
uint8_t check_buttons(){
	// if pin12 (left) button is pressed
	if(~PINB & 0b00010000){
		return left_button;
	}
	// if pin11 (right) button is pressed
	else if(~PINB & 0b00001000){
		return right_button;
	}
	// if no button is pressed
	else
		return 0;
}

// prints string of characters to LCD
void lcd_print_string(char string[]){	
	// variables to check if there is LCD line overflow
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

// clear the display and set cursor to top left
void clear_display(){
	lcd_write_cmd(disp_clear);
	_delay_ms(2);
}

void delay_ms(uint8_t ms){
	while(ms>0){
		_delay_ms(1);
		ms--;
	}
}

void led_driver_pin13(uint16_t freq_mHz, uint16_t duty){
	double period_ms;
	if(freq_mHz == 0)
		period_ms = 16;
	else
		period_ms = 1000000/freq_mHz;
		
	uint16_t tON = period_ms*duty/100;
	uint16_t tOFF = period_ms - tON;
	
	PORTB |= 0b00100000;	// turn on led
	delay_ms(tON);
	PORTB &= 0b11011111;	// turn off led
	delay_ms(tOFF);
}

void ui_text(char text_string[], uint16_t freq_mHz, uint16_t duty){
	lcd_print_string(text_string);
	while(!check_buttons()){
		led_driver_pin13(freq_mHz, duty);
	}
	clear_display();
	_delay_ms(debounce);
}