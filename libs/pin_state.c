#include "pin_state.h"

unsigned char pin_state(pin_t* pin) {
    uint8_t result=0;
    if((pin->bit_state & pin->pin_mask)!=0) {
        if((*pin->pin & pin->pin_mask)!=0) result=1; // 1
        else result=3; // 1->0
    }
    else {
        if((*pin->pin & pin->pin_mask)!=0) result=2; //0->1
        else result=0; // 0
    }
    pin->bit_state=*pin->pin;
    return result;
}

unsigned char button_state(pin_t* pin, uint8_t state, uint8_t state_2) {
    if (pin_state(pin) == state) {
        //_delay_us(0.1);
        if (pin_state(pin) == state_2) return 1;
    } else return 0;
}
