/* 
 * File:   44780.h
 * Author: root
 *
 * Created on 16 ноября 2014 г., 15:30
 */

#ifndef LCD44780_H
#define	LCD44780_H

#include "ports.h"
#include <util/delay.h>
// D0..7
#define LCD_PORT PORTA
#define LCD_DDR  DDRA
#define LCD_PIN  PINA

// E,RS,RW pins
typedef struct {
    pin_t e_p;
    pin_t rs_p;
    pin_t rw_p;
} lcd44780_t;

#define LCD_DELAY       _delay_us(1)

extern void lcd_init(lcd44780_t* lcd);
extern void lcd_entry_mode_set(lcd44780_t* lcd,uint8_t id,uint8_t s);
extern void lcd_on_off_control(lcd44780_t* lcd,uint8_t d,uint8_t c,uint8_t b);
extern void lcd_function_set(lcd44780_t* lcd,uint8_t dl,uint8_t n,uint8_t f);
extern void lcd_puts(lcd44780_t* lcd,uint8_t* str);

#endif	/* 44780_H */

/*
#include <avr/io.h>
#include "libs/44780.h"

lcd44780_t lcd = {
    make_pin(B,2),
    make_pin(B,0),
    make_pin(B,1)
};

int main()
{
    lcd_init(&lcd);
    lcd_entry_mode_set(&lcd,1,0);
    lcd_on_off_control(&lcd,1,1,0);
    lcd_function_set(&lcd,1,1,1);
    
    lcd_puts(&lcd,"Hello!");
}
 */