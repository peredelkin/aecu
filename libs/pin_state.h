/* 
 * File:   ports_state.h
 * Author: ruslan
 *
 * Created on 25 Сентябрь 2014 г., 9:30
 */

#ifndef PORTS_STATE_H
#define	PORTS_STATE_H

#include "ports.h"
#include <util/delay.h>

extern unsigned char pin_state(pin_t* pin);
extern unsigned char button_state(pin_t* pin,uint8_t state,uint8_t state_2);

#endif	/* PORTS_STATE_H */

