#include "ports.h"
#include <stdint.h>

//установка бита регистра (&регистр,бит)

inline void bit_on(volatile uint8_t *reg, unsigned char bit) {
    *reg |= (1 << bit);
}
//сброс бита регистра (&регистр,бит)

inline void bit_off(volatile uint8_t *reg, unsigned char bit) {
    *reg &= ~(1 << bit);
}
//инвертирование бита регистра (&регистр,бит)

inline void bit_toggle(volatile uint8_t *reg, unsigned char bit) {
    *reg ^= (1 << bit);
}
//инвертирование пина

inline void pin_toggle(pin_t* pin) {
    *pin->port ^= pin->pin_mask;
}

uint8_t port_mask(uint8_t size, uint8_t shift) {
    if ((shift + size) > 8) return 0;
    uint8_t mask = 0;
    uint8_t pin = shift;
    while (pin < size + shift) {
        mask |= (1 << pin);
        pin++;
    }
    return mask;
}