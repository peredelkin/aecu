/* 
 * File:   ports.h
 * Author: ruslan
 *
 * Created on 24 Сентябрь 2014 г., 8:33
 */

#ifndef PORTS_H
#define	PORTS_H

#include <avr/io.h>
#include <stdint.h>

#define CONCAT(a, b) a ## b
#define GEN_PORT(P) CONCAT(PORT,P)
#define GEN_PIN(P) CONCAT(PIN,P)
#define GEN_DDR(P) CONCAT(DDR,P)

typedef struct {
    volatile uint8_t* port;     //регистр PORT
    volatile uint8_t* pin;      //регистр PIN
    volatile uint8_t* ddr;      //регистр DDR
    uint8_t pin_mask;           //маска
    uint8_t bit_state;          //состояние бита
} pin_t;

#define make_pin(P, N) {.port=&GEN_PORT(P), .pin=&GEN_PIN(P), .ddr=&GEN_DDR(P), .pin_mask = (1 << N)} //получение адресов регистров порта

//pin_t a1 = make_pin(A, 1); //PORTA.1 (пример)

inline void bit_on(volatile uint8_t *reg,unsigned char bit);
inline void bit_off(volatile uint8_t *reg,unsigned char bit);
inline void bit_toggle(volatile uint8_t *reg,unsigned char bit);
//включение пина

static inline __attribute__((always_inline))  void pin_on(pin_t* pin) {
    *pin->port |= pin->pin_mask;
}
//выключение пина

static inline __attribute__((always_inline))  void pin_off(pin_t* pin) {
    *pin->port &= ~pin->pin_mask;
}

//возврат значения пина

static inline __attribute__((always_inline)) uint8_t pin_val(pin_t* pin) {
    if (*pin->pin & pin->pin_mask) return 1;
    else return 0;
}

//настройка на выход

static inline __attribute__((always_inline)) void pin_out(pin_t* pin) {
    *pin->ddr |= pin->pin_mask;
    *pin->port &= ~pin->pin_mask;
}
//настройка на вход без pull up

static inline __attribute__((always_inline)) void pin_in(pin_t* pin) {
    *pin->ddr &= ~pin->pin_mask;
    *pin->port &= ~pin->pin_mask;
}
//настройка на вход с pull up

static inline __attribute__((always_inline)) void pin_in_pu(pin_t* pin) {
    *pin->ddr &= ~pin->pin_mask;
    *pin->port |= pin->pin_mask;
}

inline void pin_toggle(pin_t* pin);
inline uint8_t pin_val(pin_t* pin);

#endif	/* PORTS_H */

