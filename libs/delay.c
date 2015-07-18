#include "delay.h"

#ifdef F_CPU
    #define DELAY_MS_MAX (262 / (F_CPU / 1000000))
    #define DELAY_US_MAX (768 / (F_CPU / 1000000))
#else
    #warning F_CPU is not defined, using default values
    #define DELAY_MS_MAX (12)
    #define DELAY_US_MAX (48)
#endif

void delay_ms16(uint16_t msec)
{
    while(msec > DELAY_MS_MAX){
        _delay_ms(DELAY_MS_MAX);
        msec -= DELAY_MS_MAX;
    }
    while(msec > 0){
        _delay_ms(1);
        msec --;
    }
}

void delay_ms8(uint8_t msec)
{
    while(msec > DELAY_MS_MAX){
        _delay_ms(DELAY_MS_MAX);
        msec -= DELAY_MS_MAX;
    }
    while(msec > 0){
        _delay_ms(1);
        msec --;
    }
}

void delay_us16(uint16_t usec)
{
    while(usec > DELAY_US_MAX){
        _delay_us(DELAY_US_MAX);
        usec -= DELAY_US_MAX;
    }
    while(usec > 0){
        _delay_us(1);
        usec --;
    }
}

void delay_us8(uint8_t usec)
{
    while(usec > DELAY_US_MAX){
        _delay_us(DELAY_US_MAX);
        usec -= DELAY_US_MAX;
    }
    while(usec > 0){
        _delay_us(1);
        usec --;
    }
}
