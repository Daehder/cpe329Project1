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

// Define statements
#define F_CPU 16000000UL	// set CPU speed to 16MHz
#define std_delay 50		// set standard delay time to 50us
#define debounce 250		// set debounce time to 50ms
#define func_set 0x38		// 8bit command for function set
#define disp_ctrl 0x0F		// 8bit command for display control
#define disp_clear 0x01		// 8bit command for display clear
#define entry_mode 0x06		// 8bit command for enable entry
#define set_line1 0x80		// address for start of line 2
#define set_line2 0xC0		// address for start of line 1
#define right_button 0x01	// return type for right button
#define left_button 0x02	// return type for left button
#define pin13 0x20			// pin13 LED port

// Included libraries
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "StringLib/StringLib.h"

// Function prototypes
void lcd_write_cmd(char command);
void lcd_write_char(char character);
void lcd_initialize();
void write_custom_char(uint8_t loc, unsigned char *p);
void lcd_print_string(char string[]);
void delay_sec(uint16_t ms);
void clear_display(); 
uint8_t check_buttons();
void ui_text(char text_string[], uint16_t freq_mHz, uint16_t duty);
void ui_heart(uint8_t loc, uint16_t duty);
void led_driver_pin13(uint16_t freq_mHz, uint16_t duty);


/////////////////////////////MAIN FUNCTION//////////////////////////////////////

int main(void)
{
	UCSR0B = 0;                   // Allows atmel to R/W to pins 0 and 1 
	DDRB = 0b00100111;            // Pin11-12 = In; E, RW, RS, PIN13 = Out 
	DDRD = 0b11111111;            // DB(7-0) = output 
	
	PORTB |= 0b00011000;          // Set internal pull-ups @ pin11/12 

	// custom characters
	unsigned char small_heart[8] = {0x00, 0x00, 0x00, 0x00, 0x0E, 0x04, 0x00, 0x00};
	unsigned char medium_heart[8] = {0x00, 0x00, 0x00, 0x0A, 0x1F, 0xE, 0x04, 0x00};
	unsigned char large_heart[8] = {0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00};

	// Initialize the LCD 
	lcd_initialize();
	
	// Write custom characters to LCD
	write_custom_char(0, small_heart);
	write_custom_char(1, medium_heart);
	write_custom_char(2, large_heart);

	// The following text comes from Dr. Suess's "The Grinch" and is not 
	// an original work of the authors of this program. 
	while(1){
		ui_text("And what happened, then?", 0, 0);
		ui_text("Well, ", 0, 25);
		ui_text("in Whoville they say", 0, 50);
		ui_text("- that the Grinch's small", 0, 75);
		ui_text("heart grew three sizes that day.", 0, 100);
		ui_heart(0x00, 20);		// small heart w/ duty cycle ~20%
		ui_heart(0x01, 50);		// medium heart /w duty cycle ~50%
		ui_heart(0x02, 100);	// large heart w/ duty cycle ~100%
		ui_text("And then - the true meaning of", 0, 100);
		ui_text("Christmas came through,", 0, 100);
		ui_text("and the Grinch found the", 0, 100);
		ui_text("strength of *ten* Grinches,", 0, 100);
		ui_text("plus two!", 0, 100);
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

// Initializes the LCD 
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

// prints custom character p[8] to CGRAM at address loc
void write_custom_char(uint8_t loc, unsigned char *p){
	if(loc<8){
		lcd_write_cmd(0x40 + (loc*8));
		for(uint8_t i=0; i<8; i++)
			lcd_write_char(p[i]);
	}
	lcd_write_cmd(0x80);
}

//  toggles pins and runs delays for have LCD character writing  
void lcd_write_char(char character){
	PORTB &= 0b11111000;    // E = 0
	PORTD = character;		// send character
	PORTB |= 0b00000101;	// E = 1; RS = 1
	PORTB &= 0b11111000;	// E = 0; RS = 0
	_delay_us(std_delay);	// delay
}

// Checks which (if any) buttons are pressed and
// returns the constant associated with said button
// If no button is pressed, 0 is returned
uint8_t check_buttons(){
	// If pin12 (left) button is pressed 
	if(~PINB & 0b00010000){
		return left_button;
	}
	// If pin11 (right) button is pressed
	else if(~PINB & 0b00001000){
		return right_button;
	}
	// If no button is pressed
	else
		return 0;
}

// Prints string of characters to LCD 
void lcd_print_string(char string[]){
	char *line1, *line2, *extra;
	line1 = getLine(string, &extra);
   
	// Print top line 
	for(int i=0; i<strlen(line1); i++){
		lcd_write_char(line1[i]);
		_delay_us(std_delay);
	}
	
	// Set cursor to beginning of line 2
	lcd_write_cmd(set_line2);
   string = extra;
   line2 = getLine(string, &extra);
	
	// Print bottom line 
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

// delay in ms function that can use a variables 
void delay_ms(uint8_t ms){
	while(ms>0){
		_delay_ms(1);
		ms--;
	}
}

// sets pin13 LED to a specific frequency and duty cylce (brightness)
void led_driver_pin13(uint16_t freq_mHz, uint16_t duty){
	// either LED is always on freq = 60Hz or it is specified
	double period_ms;
	if(freq_mHz == 0)
		period_ms = 16;	// 60Hz
	else
		period_ms = 1000000/freq_mHz;
		
	uint16_t tON = period_ms*duty/100;
	uint16_t tOFF = period_ms - tON;
	
	PORTB |= 0b00100000;	// turn on led
	delay_ms(tON);
	PORTB &= 0b11011111;	// turn off led
	delay_ms(tOFF);
}

// runs standard ASCII strings and LED effects for the story
void ui_text(char text_string[], uint16_t freq_mHz, uint16_t duty){
	lcd_print_string(text_string);			
	// run led at specific frequency and duty cycle until button is pressed
	while(!check_buttons())
		led_driver_pin13(freq_mHz, duty);	
	clear_display();						
	_delay_ms(debounce);				
}

// similar to ui_text except uses custom characters instead of ASCII
void ui_heart(uint8_t loc, uint16_t duty){
	lcd_write_cmd(set_line1+7);		// set heart location to the middle of line 1
	lcd_write_char(loc);
	// run led at specific duty cycle (brightness) until button is pressed
	while(!check_buttons())
		led_driver_pin13(0, duty);
	clear_display();
	_delay_ms(debounce);
}