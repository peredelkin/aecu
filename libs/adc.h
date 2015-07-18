/* 
 * File:   adc.h
 * Author: ruslan
 *
 * Created on 25 Сентябрь 2014 г., 21:30
 */

#ifndef ADC_H
#define	ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define AREF 0x00 // AREF, Internal Vref turned off.
#define AVCC 0x40 // AVCC with external capacitor at AREF pin.
#define VREF 0xC0 // Internal 2.56V Voltage Reference with external capacitor at AREF pin.

uint8_t channel=0;

extern void adc_init(void);

#endif	/* ADC_H */

