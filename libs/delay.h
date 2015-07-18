/**
 * @file delay.h
 * Задержки с переменным временем.
 */

#ifndef DELAY_H
#define	DELAY_H

#include <stdint.h>
#include <util/delay.h>

/**
 * Выполняет задержку выполнения, 16 бит.
 * @param msec время в мс.
 */
extern void delay_ms16(uint16_t msec);

/**
 * Выполняет задержку выполнения, 8 бит.
 * @param msec время в мс.
 */
extern void delay_ms8(uint8_t msec);

/**
 * Выполняет задержку выполнения, 16 бит.
 * @param msec время в нс.
 */
extern void delay_us16(uint16_t usec);

/**
 * Выполняет задержку выполнения, 8 бит.
 * @param msec время в нс.
 */
extern void delay_us8(uint8_t usec);

#endif	/* DELAY_H */
