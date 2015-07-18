/* 
 * File:   one_wire.h
 * Author: ruslan
 *
 * Created on 11 октября 2014 г., 14:39
 */

#ifndef ONE_WIRE_H
#define	ONE_WIRE_H

#include "ports.h"
#include <util/delay.h>

typedef struct {
    pin_t one_wire_in;
    pin_t one_wire_out;
} one_wire_t;

#endif	/* ONE_WIRE_H */