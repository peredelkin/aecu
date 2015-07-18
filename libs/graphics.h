/**
 * @file graphics.h
 * Библиотека для работы с графикой.
 */

#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "errors.h"
#include "defs.h"

//! Расположение байта в буфере.
//! Горизонтальное, LSB к MSB справа налево.
#define GRAPHICS_BYTE_ALIGN_HORIZONTAL 0
//! Вертикальное, LSB к MSB сверху вниз.
#define GRAPHICS_BYTE_ALIGN_VERTICAL   1
//! Тип расположения байт в буфере.
typedef uint8_t graphics_byte_align_t;

//! Тип размера.
typedef size_t graphics_size_t;
//! Тип позиции.
typedef int16_t graphics_pos_t; //int16_t

//! Максимальный размер растра.
#define GRAPHICS_MAX_SIZE 128

/**
 * Структура точки.
 */
typedef struct _GPoint{
    graphics_pos_t x;
    graphics_pos_t y;
}graphics_point_t;

//! Значения пиксела.
//! Пиксел не горит.
#define PIXEL_OFF       0
//! Пиксел горит.
#define PIXEL_ON        1
//! Тип значения пиксела.
typedef uint8_t pixel_value_t;

//! Режим отрисовки.
//! Установка значения.
#define PAINT_MODE_SET  0
//! Логическое исключающее ИЛИ.
#define PAINT_MODE_XOR  1
//! Тип режима отрисовки.
typedef uint8_t paint_mode_t;

//! Режим заполнения.
//! Без заполнения.
#define FILL_MODE_NONE  0
//! Заливка.
#define FILL_MODE_SOLID 1
//! Тип заполнения.
typedef uint8_t fill_mode_t;


/**
 * Структура растра.
 */
typedef struct _Graphics{
    uint8_t* buffer;
    graphics_size_t width;
    graphics_size_t height;
    size_t buffer_size;
    graphics_byte_align_t byte_align;
    graphics_point_t dirty_from;
    graphics_point_t dirty_to;
    pixel_value_t pixel_value;
    paint_mode_t paint_mode;
    fill_mode_t fill_mode;
}graphics_t;

/**
 * Инициализирует растр.
 * @param graphics Растр.
 * @param buffer Буфер под растр.
 * @param width Ширина в пикселах.
 * @param height Высота в пикселах.
 * @param byte_align Расположение байта в буфере.
 * @return Код ошибки.
 */
extern err_t graphics_init(graphics_t* graphics, uint8_t* buffer,
                           graphics_size_t width, graphics_size_t height,
                           graphics_byte_align_t byte_align);

/**
 * Очищает растр.
 * @param graphics Растр.
 */
extern void graphics_clear(graphics_t* graphics);

/**
 * Получает буфер растра.
 * @param graphics Растр.
 * @return Буфер растра.
 */
static ALWAYS_INLINE uint8_t* graphics_buffer(graphics_t* graphics)
{
    return graphics->buffer;
}

/**
 * Получает размер буфера растра.
 * @param graphics Растр.
 * @return Размер буфера растра.
 */
static ALWAYS_INLINE size_t graphics_buffer_size(graphics_t* graphics)
{
    return graphics->buffer_size;
}

/**
 * Получает ширину растра.
 * @param graphics Растр.
 * @return Ширину растра.
 */
static ALWAYS_INLINE graphics_size_t graphics_width(graphics_t* graphics)
{
    return graphics->width;
}

/**
 * Получает высоту растра.
 * @param graphics Растр.
 * @return Высоту растра.
 */
static ALWAYS_INLINE graphics_size_t graphics_height(graphics_t* graphics)
{
    return graphics->height;
}

/**
 * Получает ширину растра в байтах.
 * @param graphics Растр.
 * @return Ширину растра в байтах.
 */
extern graphics_size_t graphics_width_bytes(graphics_t* graphics);

/**
 * Получает высоту растра в байтах.
 * @param graphics Растр.
 * @return Высоту растра в байтах.
 */
extern graphics_size_t graphics_height_bytes(graphics_t* graphics);

/**
 * Получает значение рисуемых пикселов.
 * @param graphics Растр.
 * @return Значение пикселов.
 */
static ALWAYS_INLINE pixel_value_t graphics_pixel_value(graphics_t* graphics)
{
    return graphics->pixel_value;
}

/**
 * Устанавливает значение рисуемых пикселов.
 * @param graphics Растр.
 * @param value Значение пикселов.
 */
extern void graphics_set_pixel_value(graphics_t* graphics, pixel_value_t value);

/**
 * Получает режим отрисовки.
 * @param graphics Растр.
 * @return Режим отрисовки.
 */
static ALWAYS_INLINE paint_mode_t graphics_paint_mode(graphics_t* graphics)
{
    return graphics->paint_mode;
}

/**
 * Устанавливает режим отрисовки.
 * @param graphics Растр.
 * @param mode Режим отрисовки.
 */
extern void graphics_set_paint_mode(graphics_t* graphics, paint_mode_t mode);

/**
 * Получает режим заполнения.
 * @param graphics Растр.
 * @return Режим заполнения.
 */
static ALWAYS_INLINE fill_mode_t graphics_fill_mode(graphics_t* graphics)
{
    return graphics->fill_mode;
}

/**
 * Устанавливает режим заполнения.
 * @param graphics Растр.
 * @param mode Режим заполнения.
 */
extern void graphics_set_fill_mode(graphics_t* graphics, fill_mode_t mode);

/**
 * Сбрасывает флаг наличия грязных данных.
 * @param graphics Растр.
 */
extern void graphics_dirty_reset(graphics_t* graphics);

/**
 * Получает флаг наличия грязных данных.
 * @param graphics Растр.
 * @return Флаг наличия грязных данных.
 */
extern bool graphics_dirty(graphics_t* graphics);
/**
 * Получает первый грязный байт.
 * @param graphics Растр.
 * @return Первый грязный байт.
 */
extern graphics_size_t graphics_dirty_from_byte(graphics_t* graphics);

/**
 * Получает последний грязный байт.
 * @param graphics Растр.
 * @return Последний грязный байт.
 */
extern graphics_size_t graphics_dirty_to_byte(graphics_t* graphics);

/**
 * Устанавливает значение пиксела.
 * @param graphics Растр.
 * @param x Абсцисса.
 * @param y Ордината.
 * @param value Значение пиксела.
 */
extern void graphics_set_pixel(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y);

/**
 * Получает значение пиксела.
 * @param graphics Растр.
 * @param x Абсцисса.
 * @param y Ордината.
 * @return  Значение пиксела.
 */
extern pixel_value_t graphics_get_pixel(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y);

/**
 * Рисует горизонтальную линию.
 * @param graphics Растр.
 * @param x Абсцисса.
 * @param y_from Начальная ордината.
 * @param y_to Конечная ордината.
 */
extern void graphics_hline(graphics_t* graphics, graphics_pos_t y, graphics_pos_t x_from, graphics_pos_t x_to);

/**
 * Рисует вертикальную линию.
 * @param graphics Растр.
 * @param y Ордината.
 * @param x_from Начальная абсцисса.
 * @param x_to Конечная абсциса.
 */
extern void graphics_vline(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y_from, graphics_pos_t y_to);

/**
 * Рисует линию.
 * @param graphics Растр.
 * @param x_from Начальная абсцисса.
 * @param y_from Начальная ордината.
 * @param x_to Конечная абсцисса.
 * @param y_to Конечная ордината.
 */
extern void graphics_line(graphics_t* graphics,
                          graphics_pos_t x_from, graphics_pos_t y_from,
                          graphics_pos_t x_to, graphics_pos_t y_to);

/**
 * Рисует круг.
 * @param graphics Растр.
 * @param center_x Абсцисса центра.
 * @param center_y Ордината центра.
 * @param radius Радиус.
 */
extern void graphics_circle(graphics_t* graphics,
                            graphics_pos_t center_x, graphics_pos_t center_y,
                            graphics_pos_t radius);

/**
 * Рисует прямоугольник.
 * @param graphics Растр.
 * @param x_from Начальная абсцисса.
 * @param y_from Начальная ордината.
 * @param x_to Конечная абсцисса.
 * @param y_to Конечная ордината.
 */
extern void graphics_square(graphics_t* graphics,
                            graphics_pos_t x_from, graphics_pos_t y_from,
                            graphics_pos_t x_to, graphics_pos_t y_to);

/**
 * Копирует изображение из памяти программ.
 * @param graphics Растр.
 * @param x Абсцисса места копирования.
 * @param y Ордината места копирования.
 * @param pgm_image Адрес изображения.
 * @param byte_align Расположение байта.
 * @param width Ширина изображения.
 * @param height Высота изображения.
 * @param img_data_width Ширина блока данных изображения.
 */
extern void graphics_copy_image_pgm(graphics_t* graphics,
                                    graphics_pos_t x, graphics_pos_t y,
                                    const uint8_t* pgm_image, graphics_byte_align_t byte_align,
                                    graphics_size_t width, graphics_size_t height,
                                    graphics_size_t img_data_width);

#endif	/* GRAPHICS_H */

