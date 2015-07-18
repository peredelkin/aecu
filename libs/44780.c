#include "44780.h"
#include "lcd0108.h"

//Функция управления портом данных

static void lcd_port_io(uint8_t io, uint8_t mask) {
    if (io) LCD_DDR |= mask;
    else {
        LCD_PORT &= ~mask;
        LCD_DDR &= ~mask;
    }
}

/**
 * Функция тактирования
 * @param lcd
 * @param state
 */
static void lcd_e_pin(lcd44780_t* lcd, uint8_t state) {
    if (state) pin_on(&lcd->e_p);
    else pin_off(&lcd->e_p);
    LCD_DELAY;
}

/**
 * Ожидание дисплея
 */
static void lcd_wait_busy(lcd44780_t* lcd) {
    lcd_port_io(0, 0xFF);
    pin_off(&lcd->rs_p);
    pin_on(&lcd->rw_p);
    while (1) {
        lcd_e_pin(lcd, 1);
        if (!(LCD_PIN & 0x80)) break;
        lcd_e_pin(lcd, 0);
    }
    lcd_e_pin(lcd, 0);
}

/**
 * Передача команд/данных дисплею.
 * @param data Данные.
 */
static void lcd_write_cmd_data(lcd44780_t* lcd, uint8_t rs, uint8_t data) {
    lcd_wait_busy(lcd);
    if (rs) pin_on(&lcd->rs_p); //данные
    else pin_off(&lcd->rs_p); //команды
    pin_off(&lcd->rw_p);
    lcd_port_io(1, 0xFF);
    LCD_PORT = data;
    lcd_e_pin(lcd, 1);
    lcd_e_pin(lcd, 0);
}

/**
 * Очистка дисплея.
 */
static void lcd_clear(lcd44780_t* lcd) {
    lcd_write_cmd_data(lcd, 0, 0x01);
}

/**
 * Возврат курсора.
 */
static void lcd_return_home(lcd44780_t* lcd) {
    lcd_write_cmd_data(lcd, 0, 0x02);
}

/**
 * Настройка автомотического сдвига.
 * @param id Сдвиг вправо,иначе влево.
 * @param s Сдвиг при чтении включен.
 */
void lcd_entry_mode_set(lcd44780_t* lcd, uint8_t id, uint8_t s) {
    lcd_write_cmd_data(lcd, 0, (1 << 2) | (id << 1) | (s << 0));
}

/**
 * Параметры курсора и работы дисплея.
 * @param d Дисплей включен,если установлено.
 * @param c Отображение курсора,если установлено.
 * @param b Мигание курсора,если установлено.
 */
void lcd_on_off_control(lcd44780_t* lcd, uint8_t d, uint8_t c, uint8_t b) {
    lcd_write_cmd_data(lcd, 0, (1 << 3) | (d << 2) | (c << 1) | (b << 0));
}

/**
 * Ручной сдвиг дисплея или курсора.
 * @param sc Сдвиг дисплея,если установлено,иначе курсор.
 * @param rl Сдвиг вправо,если установлено,иначе влево.
 */
static void lcd_cursor_or_display_shift(lcd44780_t* lcd, uint8_t sc, uint8_t rl) {
    lcd_write_cmd_data(lcd, 0, (1 << 4) | (sc << 3) | (rl << 2));
}

/**
 * Настройка шины данных дисплея,шрифта и количества строк.
 * @param dl 8 бит,если установлено,иначе 4 бита(прием/передача в два прохода).
 * @param n 2 строки,если установлено,иначе одна строка.
 * @param f 5x10,если установлено,иначе 5х8.
 */
void lcd_function_set(lcd44780_t* lcd, uint8_t dl, uint8_t n, uint8_t f) {
    lcd_write_cmd_data(lcd, 0, (1 << 5) | (dl << 4) | (n << 3) | (f << 2));
}

/**
 * Передача адреса CGRAM.
 * @param addr Адрес.
 */
static void lcd_set_cgram(lcd44780_t* lcd, uint8_t addr) {
    lcd_write_cmd_data(lcd, 0, (1 << 6) | addr);
}

/**
 * Передача адреса DDRAM.
 * @param addr Адрес.
 */
static void lcd_set_ddram(lcd44780_t* lcd, uint8_t addr) {
    lcd_write_cmd_data(lcd, 0, (1 << 7) | addr);
}

/**
 * Инициализация портов E,RW,RS микроконтроллера.
 */
void lcd_init(lcd44780_t* lcd) {
    pin_out(&lcd->e_p);
    pin_out(&lcd->rs_p);
    pin_out(&lcd->rw_p);
}

/**
 * Передача массива данных.
 * @param data Массив.
 * @param size Размер массива.
 */
static void lcd_write(lcd44780_t* lcd, uint8_t* data, uint8_t size) {
    uint8_t i = 0;
    for (; i < size; i++) lcd_write_cmd_data(lcd, 1, data[i]);
}

/**
 * Передача строки.
 * @param str Строка.
 */
void lcd_puts(lcd44780_t* lcd, uint8_t* str) {
    uint16_t i = 0;
    while (str[i] != 0) {
        lcd_write_cmd_data(lcd, 1, str[i]);
        i++;
    }
}
