#include "two_wire.h"

/**
 * Установка занчений скорости и прескалера.
 * Инициализация TWI.
 */
void twi_init(twi_t* two_wire) {
    TWBR = TWBR_VALUE;
    TWSR = TWPS_VALUE;
    TWCR = (1 << TWEN);
    two_wire->twi_busy_flag = false;
    two_wire->callback = NULL;
}

/**
 * Отправка команды START и установка флага занятости TWI.
 */
void twi_cmd_start(twi_t* two_wire) {
    two_wire->twi_busy_flag = true;
    TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

/**
 * Отправка команды STOP и сброс флага занятости TWI.
 */
void twi_cmd_stop(twi_t *two_wire) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (1 << TWIE);
    two_wire->twi_busy_flag = false;
    if (two_wire->callback) two_wire->callback();
}

/**
 * Отправка команды ACK для приема следующего байта.
 */
void twi_cmd_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
}

/**
 * Отпрака команды NACK или TX для приема последнего байта или отправки байта.
 */
void twi_cmd_nack_tx(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
}

/**
 * Обработчик прерывания TWI с приемом и передачей.
 */
void twi_handler(twi_t *two_wire) {
    switch (TW_STATUS) {
        case TW_START:
            two_wire->twi_sla_byte_n = 0;
            TWDR = (two_wire->twi_sla_addr << 1); //TW_WRITE
            twi_cmd_nack_tx();
            break;
        case TW_MT_SLA_ACK:
            TWDR = two_wire->twi_sla_mem_addr;
            twi_cmd_nack_tx();
            break;
        case TW_MT_DATA_ACK:
            if (two_wire->twi_io) {
                twi_cmd_start(two_wire);
            } else {
                if (two_wire->twi_sla_byte_n < two_wire->twi_size) {
                    TWDR = two_wire->twi_sla_data[two_wire->twi_sla_byte_n];
                    two_wire->twi_sla_byte_n++;
                    twi_cmd_nack_tx();
                } else {
                    twi_cmd_stop(two_wire);
                }
            }
            break;
        case TW_REP_START:
            TWDR = (two_wire->twi_sla_addr << 1) | TW_READ;
            twi_cmd_nack_tx();
            break;
        case TW_MR_SLA_ACK:
            twi_cmd_ack();
            break;
        case TW_MR_DATA_ACK:
            two_wire->twi_sla_data[two_wire->twi_sla_byte_n] = TWDR;
            two_wire->twi_sla_byte_n++;
            if (two_wire->twi_sla_byte_n < two_wire->twi_size - 1) twi_cmd_ack();
            else twi_cmd_nack_tx();
            break;
        case TW_MR_DATA_NACK:
            two_wire->twi_sla_data[two_wire->twi_sla_byte_n] = TWDR;
            twi_cmd_stop(two_wire);
            break;
        default:
            twi_cmd_stop(two_wire);
            break;
    }
}

void twi_wait(twi_t* two_wire) {
    while (two_wire->twi_busy_flag);
}

/**
 * Callback по завершении записи или чтения
 * @param two_wire
 * TWI порт
 * @param callback
 * Вызываемая функция
 */
void twi_set_callback(twi_t* two_wire, twi_callback_t callback) {
    two_wire->callback = callback;
}

/**
 * Функция передачи TWI.
 * @param addr
 * Адрес устройства.
 * @param mem_addr
 * Адрес в памяти устройства.
 * @param data
 * Передаваемые данные.
 * @param size
 * Количество байт данных.
 */
void twi_tx(twi_t* two_wire, uint8_t addr, uint8_t mem_addr, uint8_t *data, uint8_t size) {
    if (!two_wire->twi_busy_flag) {
        two_wire->twi_io = 0;
        two_wire->twi_sla_addr = addr;
        two_wire->twi_sla_mem_addr = mem_addr;
        two_wire->twi_sla_data = data;
        two_wire->twi_size = size;
        twi_cmd_start(two_wire);
    }
}

/**
 * Функция приема TWI.
 * @param addr
 * Адрес устройства.
 * @param mem_addr
 * Адрес в памяти устройства.
 * @param data
 * Принимаемые данные.
 * @param size
 * Количество байт данных.
 */
void twi_rx(twi_t* two_wire, uint8_t addr, uint8_t mem_addr, uint8_t *data, uint8_t size) {
    if (!two_wire->twi_busy_flag) {
        two_wire->twi_io = 1;
        two_wire->twi_sla_addr = addr;
        two_wire->twi_sla_mem_addr = mem_addr;
        two_wire->twi_sla_data = data;
        two_wire->twi_size = size;
        twi_cmd_start(two_wire);
    }
}
