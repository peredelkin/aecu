#include "ext_int.h"

void ext_int_init(void) {
                                //Bits 7..0 – ISC31, ISC30 - ISC00, ISC00: External Interrupt 3 - 0 Sense Control Bits
    EICRA |=(0<<ISC31)|
            (0<<ISC30)|
            (0<<ISC21)|
            (0<<ISC20)|
            (0<<ISC11)|
            (0<<ISC10)|
            (0<<ISC01)|
            (0<<ISC00);
}
