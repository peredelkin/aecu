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

void coil_call_event_once(coil_ch_act_t* coil_ch) {
    tmr16_event_call(&coil_ch->ch);
    tmr16_event_set(&coil_ch->ch, NULL);
    tmr16_int_disable(&coil_ch->ch);
}

void coil_event_set(coil_ch_act_t* ch_head, void (*timer_event) (),uint16_t ocr) {
    while((ch_head->next) && (ch_head->ch.event)) ch_head = ch_head->next;
    tmr16_write_cr(&ch_head->ch, ocr);
    tmr16_event_set(&ch_head->ch, timer_event);
    tmr16_int_enable(&ch_head->ch);
}