/**
 * @file font.h
 * Библиотека для работы со шрифтами в виде битовых карт.
 */
#ifndef FONT_H
#define	FONT_H

#include <stddef.h>
#include <stdint.h>
#include "errors.h"
#include "graphics.h"

/**
 * Структура битовой карты шрифта.
 */
typedef struct _FontBitmap {
    //! Число символов в битовой карте.
    //uint8_t symbols_count;
    //! Код первого символа в битовой карте.
    uint8_t first_symbol;
    //! Кол последнего символа в битовой карте.
    uint8_t last_symbol;
    //! Ширина битовой карты в пикселах.
    graphics_size_t data_width;
    /**
     * Данные символов - битовая карта.
     * Каждый символ должен быть выровнен по границе байта.
     */
    const uint8_t* symbols_data;
}font_bitmap_t;

/**
 * Структура шрифта.
 */
typedef struct _Font {
    //! Ширина символа.
    uint8_t symbol_width;
    //! Высота символа.
    uint8_t symbol_height;
    //!Расстояние межде символами по горизонтали.
    uint8_t hspace;
    //!Расстояние межде символами по вертикали.
    uint8_t vspace;
    //! Расположение байт символа.
    graphics_byte_align_t byte_align;
    //! Число битовых карт.
    uint8_t bitmaps_count;
    /**
     * Битовые карты.
     * Должны быть упорядочены по кодам символов.
     */
    font_bitmap_t* bitmaps;
    //! Число байт на символ по горизонтали.
    uint8_t symbol_width_in_bytes;
}font_t;


//! Размер табуляции.
#define FONT_TAB_SIZE 4



/**
 * Инициализирует шрифт.
 * @param font Шрифт.
 * @param bitmaps Битовые карты.
 * Должны быть упорядочены по кодам символов.
 * @param bitmaps_count Число битовых карт.
 * @param hspace Расстояние межде символами по горизонтали.
 * @param vspace Расстояние межде символами по вертикали.
 */
extern void font_init(font_t* font, font_bitmap_t* bitmaps, uint8_t bitmaps_count,
                      graphics_byte_align_t byte_align,
                      uint8_t symbol_width, uint8_t symbol_height,
                      uint8_t hspace, uint8_t vspace);

/**
 * Отрисовывает символ на растре.
 * @param font Шрифт.
 * @param graphics Растр.
 * @param x Абсцисса.
 * @param y Ордината.
 * @param c Символ.
 */
extern void font_putc(font_t* font, graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, char c);

/**
 * Отрисовывает строку символов на растре.
 * @param font Шрифт.
 * @param graphics Растр.
 * @param x Абсцисса.
 * @param y Ордината.
 * @param c Символ.
 */
extern void font_puts(font_t* font, graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, const char* s);

#endif	/* FONT_H */

