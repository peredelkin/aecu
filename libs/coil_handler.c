#include "coil_handler.h"

void coil_act_insert_next(coil_act_t* head, coil_act_t* tail) {
    tail->next = head->next;
    tail->prev = head;
    head->next = tail;
    if (tail->next) tail->next->prev = tail;
}

void coil_act_insert_prev(coil_act_t* head, coil_act_t* tail) {
    tail->prev = head->prev;
    tail->next = head;
    head->prev = tail;
    if (tail->prev) tail->prev->next = tail;
}

coil_act_t* coil_act_cut(coil_act_t* head) {
    coil_act_t* cut = head;
    if (cut->next) head->next->prev = head->prev;
    if (cut->prev) head->prev->next = head->next;
    return cut;
}

void coil_act_sorting_insert(coil_act_t* head, coil_act_t* tail) {
    if (tail->angle < head->angle) {
        while ((head->prev) && (tail->angle < head->prev->angle)) {
            head = head->prev;
        }
        coil_act_insert_prev(head, tail);
    } else {
        while ((head->next) && (tail->angle > head->next->angle)) {
            head = head->next;
        }
        coil_act_insert_next(head, tail);
    }
}

void coil_act_insertion_sort(coil_act_t* head) {
    coil_act_t* cut;
    uint32_t z = 0;
    while (head->next) {
        if (head->next->angle < head->angle) {
            z++;
            cut = coil_act_cut(head->next);
            while ((head->prev) && (cut->angle < head->prev->angle)) {
                head = head->prev;
            }
            coil_act_insert_prev(head, cut);
        } else {
            head = head->next;
        }
    }
}

void coil_act_sort_selected(coil_act_t* head) {
    coil_act_t* cut;
    if ((head->prev) && (head->prev->angle > head->angle)) {
        cut = coil_act_cut(head);
        while ((head->prev) && (head->prev->angle > cut->angle)) {
            head = head->prev;
        }
        coil_act_insert_prev(head, cut);
    } else {
        if ((head->next) && (head->next->angle < head->angle)) {
            cut = coil_act_cut(head);
            while ((head->next) && (head->next->angle < cut->angle)) {
                head = head->next;
            }
            coil_act_insert_next(head, cut);
        }
    }
}

void coil_act_main_handler(coil_act_t** coil, coil_ch_act_t* timer,uint16_t angle_counter,uint16_t capture) {
    while ((*coil)->tooth_angle == angle_counter) {
        if ((*coil)->action_angle == 0) (*coil)->action();
        else {
            while ((timer->next) && (timer->ch.event)) timer = timer->next ;
            tmr16_write_cr(&timer->ch, (capture * ((*coil)->action_angle)) / 6);
            tmr16_event_set(&timer->ch, (*coil)->action);
            tmr16_int_enable(&timer->ch);
        }
        if ((*coil)->angle != (*coil)->angle_buffer) {
            (*coil)->angle = (*coil)->angle_buffer;
            (*coil)->action_angle = (*coil)->angle % 6;
            (*coil)->tooth_angle = (*coil)->angle - (*coil)->action_angle;
            coil_act_sort_selected((*coil));
        }
        if((*coil)->next) *coil = (*coil)->next;
        else while((*coil)->prev) *coil = coil->prev; //костыль "перемотка"
    }
}
