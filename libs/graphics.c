#include "graphics.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "bits.h"
#include "utils.h"


//! Максимальные значения.
#define GRAPHICS_BYTE_ALIGN_MAX    GRAPHICS_BYTE_ALIGN_VERTICAL
#define PIXEL_VALUE_MAX            PIXEL_ON
#define PAINT_MODE_MAX             PAINT_MODE_XOR
#define FILL_MODE_MAX              FILL_MODE_SOLID



err_t graphics_init(graphics_t* graphics, uint8_t* buffer,
                    graphics_size_t width, graphics_size_t height,
                    graphics_byte_align_t byte_align)
{
    if(buffer == NULL) return E_NULL_POINTER;
    if(width > GRAPHICS_MAX_SIZE || height > GRAPHICS_MAX_SIZE) return E_INVALID_VALUE;
    if(byte_align > GRAPHICS_BYTE_ALIGN_MAX) return E_INVALID_VALUE;
    
    graphics->buffer = buffer;
    graphics->buffer_size = width * height / 8;
    graphics->byte_align = byte_align;
    graphics->height = height;
    graphics->width = width;
    
    graphics_dirty_reset(graphics);
    
    return E_NO_ERROR;
}

void graphics_clear(graphics_t* graphics)
{
    graphics_dirty_reset(graphics);
    memset(graphics->buffer, 0x0, graphics->buffer_size);
}

graphics_size_t graphics_width_bytes(graphics_t* graphics)
{
    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        return graphics->width;
    }
    return graphics->width >> 3;
}

graphics_size_t graphics_height_bytes(graphics_t* graphics)
{
    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        return graphics->height >> 3;
    }
    return graphics->height;
}

void graphics_set_pixel_value(graphics_t* graphics, pixel_value_t value)
{
    if(value > PIXEL_VALUE_MAX) return;
    
    graphics->pixel_value = value;
}

void graphics_set_paint_mode(graphics_t* graphics, paint_mode_t mode)
{
    if(mode > PAINT_MODE_MAX) return;
    
    graphics->paint_mode = mode;
}

void graphics_set_fill_mode(graphics_t* graphics, fill_mode_t mode)
{
    if(mode > FILL_MODE_MAX) return;
    
    graphics->fill_mode = mode;
}

static bool graphics_get_pixel_address(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, graphics_size_t* byte_n, uint8_t* bit_n)
{
    if(byte_n == NULL || bit_n == NULL) return false;
    if(x < 0 || x >= graphics->width) return false;
    if(y < 0 || y >= graphics->height) return false;
    
    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        if(byte_n) *byte_n = ((size_t)graphics->width * (y >> 3)) + x;
        if(bit_n)  *bit_n = (y & 0x7);
    }else{
        if(byte_n) *byte_n = ((size_t)graphics->width * y + x) >> 3;
        if(bit_n)  *bit_n = (x & 0x7);
    }
    
    return true;
}

static void graphics_update_dirty(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y)
{
    if(graphics->dirty_from.x > x) graphics->dirty_from.x = x;
    if(graphics->dirty_from.y > y) graphics->dirty_from.y = y;
    if(graphics->dirty_to.x < x) graphics->dirty_to.x = x;
    if(graphics->dirty_to.y < y) graphics->dirty_to.y = y;
}

void graphics_dirty_reset(graphics_t* graphics)
{
    graphics->dirty_from.x = GRAPHICS_MAX_SIZE;
    graphics->dirty_from.y = GRAPHICS_MAX_SIZE;
    graphics->dirty_to.x = 0;
    graphics->dirty_to.y = 0;
}

bool graphics_dirty(graphics_t* graphics)
{
    return (graphics->dirty_from.x < graphics->dirty_to.x) ||
           (graphics->dirty_from.y < graphics->dirty_to.y);
}

extern graphics_size_t graphics_dirty_from_byte(graphics_t* graphics)
{
    graphics_size_t res = 0;
    uint8_t tmp;
    graphics_get_pixel_address(graphics, graphics->dirty_from.x, graphics->dirty_from.y, &res, &tmp);
    return res;
}

extern graphics_size_t graphics_dirty_to_byte(graphics_t* graphics)
{
    graphics_size_t res = 0;
    uint8_t tmp;
    graphics_get_pixel_address(graphics, graphics->dirty_to.x, graphics->dirty_to.y, &res, &tmp);
    return res;
}

void graphics_set_pixel(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y)
{
    graphics_size_t byte_n;
    uint8_t bit_n;
    
    if(!graphics_get_pixel_address(graphics, x, y,&byte_n, &bit_n)) return;
    
    switch(graphics->paint_mode){
        case PAINT_MODE_SET:
            BIT_SET(graphics->buffer[byte_n], bit_n, graphics->pixel_value);
            break;
        case PAINT_MODE_XOR:
            graphics->buffer[byte_n] ^= BIT_BYVAL(bit_n, graphics->pixel_value);
            break;
    }
    
    graphics_update_dirty(graphics, x, y);
}

/**
 * Устанавливает значение 8 пикселов.
 * @param graphics Растр.
 * @param byte_n Номер байта.
 * @param byte Значение 8 пикселов.
 */
void graphics_set_pixel_byte(graphics_t* graphics, graphics_size_t byte_n, uint8_t byte)
{
    if(byte_n >= graphics->buffer_size) return;
    
    switch(graphics->paint_mode){
        case PAINT_MODE_SET:
            graphics->buffer[byte_n] = byte;
            break;
        case PAINT_MODE_XOR:
            graphics->buffer[byte_n] ^= byte;
            break;
    }
}

pixel_value_t graphics_get_pixel(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y)
{
    graphics_size_t byte_n;
    uint8_t bit_n;
    
    if(!graphics_get_pixel_address(graphics, x, y,&byte_n, &bit_n)) return PIXEL_OFF;
    
    return BIT_VALUE(graphics->buffer[byte_n], bit_n);
}

/**
 * Получает флаг возможности скопировать сразу 8 пикселов.
 * @param graphics Растр.
 * @param byte_align Размещение 8 пикселов.
 * @param x Абсцисса растра.
 * @param y Ордината растра.
 * @param end Конечный номер пиксела НЕ включительно.
 * @param byte_n Возвращаемый номер байта в буфере растра.
 * @return Флаг возможности скопировать сразу 8 пикселов.
 */
static bool graphics_can_store_pixels_byte(graphics_t* graphics,
                                graphics_byte_align_t byte_align,
                                graphics_pos_t x, graphics_pos_t y, graphics_pos_t end,
                                graphics_size_t* byte_n)
{
    if(graphics->byte_align != byte_align) return false;
    
    graphics_pos_t i;
    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        i = y;
    }else{
        i = x;
    }
    
    if(end - i < 8) return false;
    
    uint8_t bit_n;
    if(!graphics_get_pixel_address(graphics, x, y, byte_n, &bit_n)) return false;
    
    if(bit_n != 0) return false;
    
    return true;
}

void graphics_hline(graphics_t* graphics, graphics_pos_t y, graphics_pos_t x_from, graphics_pos_t x_to)
{
    if(y < 0 || y >= (graphics_pos_t)graphics->height) return;
    
    graphics_size_t n;
    
    if(x_to < x_from){
        SWAP(x_to, x_from, n);
    }
    
    if(x_to < 0) return;
    if(x_from < 0) x_from = 0;
    if(x_to >= graphics->width){
        x_to = graphics->width;
    }else{
        x_to ++;
    }

    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_HORIZONTAL){
    
        uint8_t pixel_value = graphics->pixel_value ? 0xff : 0x0;
    
        for(;x_from < x_to;){
            if(graphics_can_store_pixels_byte(graphics, GRAPHICS_BYTE_ALIGN_HORIZONTAL, x_from, y, x_to, &n)){
                graphics_set_pixel_byte(graphics, n, pixel_value);
                x_from += 8;
            }else{
                graphics_set_pixel(graphics, x_from, y);
                x_from ++;
            }
        }
    }else{
        for(;x_from < x_to;){
            graphics_set_pixel(graphics, x_from, y);
            x_from ++;
        }
    }
}

void graphics_vline(graphics_t* graphics, graphics_pos_t x, graphics_pos_t y_from, graphics_pos_t y_to)
{
    if(x < 0 || x >= (graphics_pos_t)graphics->width) return;
    
    graphics_size_t n;
    
    if(y_to < y_from){
        SWAP(y_to, y_from, n);
    }
    
    if(y_to < 0) return;
    if(y_from < 0) y_from = 0;
    if(y_to >= graphics->height){
        y_to = graphics->height;
    }else{
        y_to ++;
    }
    
    if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
    
        uint8_t pixel_value = graphics->pixel_value ? 0xff : 0x0;
        
        for(;y_from < y_to;){
            if(graphics_can_store_pixels_byte(graphics, GRAPHICS_BYTE_ALIGN_VERTICAL, x, y_from, y_to, &n)){
                graphics_set_pixel_byte(graphics, n, pixel_value);
                y_from += 8;
            }else{
                graphics_set_pixel(graphics, x, y_from);
                y_from ++;
            }
        }
    }else{
        for(;y_from < y_to;){
            graphics_set_pixel(graphics, x, y_from);
            y_from ++;
        }
    }
}

void graphics_line(graphics_t* graphics,
                          graphics_pos_t x_from, graphics_pos_t y_from,
                          graphics_pos_t x_to, graphics_pos_t y_to)
{
    graphics_pos_t delta_x;
    graphics_pos_t delta_y;
    
    graphics_pos_t err;
    
    int8_t dx, dy;
    
    graphics_pos_t i;
    
    bool x_as_y = false;
    
    delta_x = x_to - x_from; delta_x = ABS(delta_x);
    delta_y = y_to - y_from; delta_y = ABS(delta_y);
    
    if(delta_y > delta_x) x_as_y = true;
    
    if(x_to >= x_from){
        dx = 1;
    }else{
        dx = -1;
    }
    
    if(y_to >= y_from){
        dy = 1;
    }else{
        dy = -1;
    }
    
    err = 0;
    i = 0;
    
    if(x_as_y){
    
        for(; i <= delta_y; i ++){
            
            graphics_set_pixel(graphics, x_from, y_from);
            
            err += delta_x;
            
            if((err + err) >= delta_y){
                err -= delta_y;
                x_from += dx;
            }
            y_from += dy;
        }
        
    }else{
    
        for(; i <= delta_x; i ++){
            
            graphics_set_pixel(graphics, x_from, y_from);
            
            err += delta_y;
            
            if((err + err) >= delta_x){
                err -= delta_x;
                y_from += dy;
            }
            x_from += dx;
        }
    
    }
}

/**
 * Заполняет строку рисуемой фигуры.
 * @param graphics Растр.
 * @param first_i Первая строка/столбец.
 * @param i Текущая строка/солбец.
 * @param j_from Начальная абсцисса/ордината.
 * @param j_to Конечная абсцисса/ордината.
 */
static void graphics_fill(graphics_t* graphics, graphics_pos_t first_i, graphics_pos_t i, graphics_pos_t j_from, graphics_pos_t j_to)
{
    switch(graphics->fill_mode){
        default:
        case FILL_MODE_NONE:
            break;
        case FILL_MODE_SOLID:
            if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
                graphics_vline(graphics, i, j_from, j_to);
            }else{
                graphics_hline(graphics, i, j_from, j_to);
            }
            break;
    }
}

void graphics_circle(graphics_t* graphics,
                     graphics_pos_t center_x, graphics_pos_t center_y,
                     graphics_pos_t radius)
{
    if(center_x + radius < 0) return;
    if(center_x - radius >= (graphics_pos_t)graphics->width) return;
    if(center_y + radius < 0) return;
    if(center_y - radius >= (graphics_pos_t)graphics->height) return;
    
    graphics_pos_t x = 0;
    graphics_pos_t y = radius;
    graphics_pos_t delta = 1 - (radius + radius);
    graphics_pos_t err;
    graphics_pos_t old_y = y;
    graphics_pos_t old_x = x;
   
    for(;y >= 0;){
        /*
        if(x >= 0 && y >= 0) graphics_set_pixel(graphics, center_x + x, center_y + y);//(0,  r)-( r, 0)
        if(x >= 0 && y >= 1) graphics_set_pixel(graphics, center_x + x, center_y - y);//(0, -r)-( r, 0)
        if(x >= 1 && y >= 0) graphics_set_pixel(graphics, center_x - x, center_y + y);//(0,  r)-(-r, 0)
        if(x >= 1 && y >= 1) graphics_set_pixel(graphics, center_x - x, center_y - y);//(0, -r)-(-r, 0)
        */
       
        graphics_set_pixel(graphics, center_x + x, center_y + y);
        if(y != 0){
            graphics_set_pixel(graphics, center_x + x, center_y - y);
            
            // Заливка.
            if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
                if(x == 0 || old_x != x){
                    graphics_fill(graphics, center_x - radius, center_x - old_x, center_y - old_y + 1, center_y + old_y - 1);
                    if(x != 0){
                        graphics_fill(graphics, center_x - radius, center_x + old_x, center_y - old_y + 1, center_y + old_y - 1);
                    }
                }
            }
        }
        if(x != 0){
            graphics_set_pixel(graphics, center_x - x, center_y + y);
            if(y != 0){
                graphics_set_pixel(graphics, center_x - x, center_y - y);
            }
            // Заливка.
            if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_HORIZONTAL){
                if(old_y != y){
                    graphics_fill(graphics, center_y - radius, center_y - y, center_x - x + 1, center_x + x - 1);
                    if(y != 0){
                        graphics_fill(graphics, center_y - radius, center_y + y, center_x - x + 1, center_x + x - 1);
                    }
                }
            }
        }
       
        old_y = y;
        old_x = x;
        
        err = (delta + y + delta + y) - 1;
        if(delta < 0 && err <= 0){
            x ++;
            delta += (x + x) + 1;
            continue;
        }
        err = (delta + delta - x - x) - 1;
        if(delta > 0 && err > 0){
            y --;
            delta += 1 - (y + y);
            continue;
        }
        x ++;
        delta += (x - y + x - y + 4);
        y --;
    }
}

void graphics_square(graphics_t* graphics,
                     graphics_pos_t x_from, graphics_pos_t y_from,
                     graphics_pos_t x_to, graphics_pos_t y_to)
{
    graphics_pos_t tmp;
    
    if(x_from > x_to) SWAP(x_from, x_to, tmp);
    if(y_from > y_to) SWAP(y_from, y_to, tmp);
    
    if(x_to < 0 || y_to < 0) return;
    if(x_from >= (graphics_pos_t)graphics->width || y_from >= (graphics_pos_t)graphics->height) return;
    
    graphics_hline(graphics, y_from, x_from, x_to);
    
    if(y_from != y_to){
        
        graphics_hline(graphics, y_to,   x_from, x_to);

        if(y_to - y_from > 1){
            y_to --; y_from ++;

            graphics_vline(graphics, x_from, y_from, y_to);
            graphics_vline(graphics, x_to,   y_from, y_to);

            if(graphics->fill_mode != FILL_MODE_NONE && (x_to - x_from > 1)){
                
                x_from ++;
                x_to --;
                
                if(graphics->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){

                    tmp = x_from;

                    while(x_from <= x_to){
                        graphics_fill(graphics, tmp, x_from, y_from, y_to);
                        x_from ++;
                    }
                }else{

                    tmp = y_from;

                    while(y_from <= y_to){
                        graphics_fill(graphics, tmp, y_from, x_from, x_to);
                        y_from ++;
                    }
                }
            }
        }
    }
}

void graphics_copy_image_pgm(graphics_t* graphics,
                             graphics_pos_t x, graphics_pos_t y,
                             const uint8_t* pgm_image, graphics_byte_align_t byte_align,
                             graphics_size_t width, graphics_size_t height, graphics_size_t img_data_width)
{
    if(x >= (graphics_pos_t)graphics->width || y >= (graphics_pos_t)graphics->height) return;
    //if(x + width <= 0 || y + height <= 0) return;
    
    graphics_pos_t x_end = x + width;
    graphics_pos_t y_end = y + height;
    
    if(x_end > graphics->width) x_end = graphics->width;
    if(y_end > graphics->height) y_end = graphics->height;
    
    if(x_end < 0) return;
    if(y_end < 0) return;
    
    if(byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        if(x < 0){
            pgm_image += -x;
            x = 0;
        }
        if(y < 0){
            pgm_image += -y / 8 * width;
            y %= 8;
        }
    }else{//horizontal
        if(y < 0){
            pgm_image += -y * width;
            y = 0;
        }
        if(x < 0){
            pgm_image += -x / 8;
            x %= 8;
        }
    }
    
    graphics_update_dirty(graphics, x, y);
    graphics_update_dirty(graphics, x_end - 1, y_end - 1);
    
    graphics_pos_t i;
    
    uint8_t bit_i;
    graphics_size_t img_i;
    uint8_t img_byte;
    
    graphics_size_t pixel_byte_n;
    
    if(byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        for(; y < y_end; y += 8){
            img_i = 0;
            for(i = x; i < x_end; i ++){

                img_byte = pgm_read_byte(&pgm_image[img_i]); img_i ++;

                if(graphics_can_store_pixels_byte(graphics, byte_align, i, y, y_end, &pixel_byte_n)){
                    graphics_set_pixel_byte(graphics, pixel_byte_n, img_byte);
                }else{
                    for(bit_i = 0; bit_i < 8; bit_i ++){
                        if(img_byte & 0x1) graphics_set_pixel(graphics, i, y + bit_i);
                        if(y + bit_i >= y_end) break;
                        img_byte >>= 1;
                    }
                }
            }
            pgm_image += img_data_width;
        }
    }else{//horizontal
        for(; y < y_end; y ++){
            img_i = 0;
            for(i = x; i < x_end; i += 8){

                img_byte = pgm_read_byte(&pgm_image[img_i]); img_i ++;
                
                if(graphics_can_store_pixels_byte(graphics, byte_align, i, y, x_end, &pixel_byte_n)){
                    graphics_set_pixel_byte(graphics, pixel_byte_n, img_byte);
                }else{
                    for(bit_i = 0; bit_i < 8; bit_i ++){
                        if(img_byte & 0x1) graphics_set_pixel(graphics, i + bit_i, y);
                        if(i + bit_i >= x_end) break;
                        img_byte >>= 1;
                    }
                }
            }
            pgm_image += img_data_width;
        }
    }
}
