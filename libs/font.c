#include "font.h"


void font_init(font_t* font, font_bitmap_t* bitmaps, uint8_t bitmaps_count,
               graphics_byte_align_t byte_align,
               uint8_t symbol_width, uint8_t symbol_height,
               uint8_t hspace, uint8_t vspace)
{
    font->bitmaps = bitmaps;
    font->bitmaps_count = bitmaps_count;
    font->byte_align = byte_align;
    font->symbol_width = symbol_width;
    font->symbol_height = symbol_height;
    font->hspace = hspace;
    font->vspace = vspace;
        
    if(font->byte_align == GRAPHICS_BYTE_ALIGN_VERTICAL){
        font->symbol_width_in_bytes = font->symbol_width;
    }else{//horizontal
        font->symbol_width_in_bytes = (font->symbol_width + 7) >> 3;
    }
}

void font_putc(font_t* font, graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, char c)
{
    const uint8_t* symbol_data;
    
    uint8_t i;
    
    for(i = 0; i < font->bitmaps_count; i ++){
        if((unsigned char)c < font->bitmaps[i].first_symbol) break;
        if((unsigned char)c > font->bitmaps[i].last_symbol) continue;
        symbol_data = font->bitmaps[i].symbols_data + 
                ((unsigned char)c - font->bitmaps[i].first_symbol) * font->symbol_width_in_bytes;
        graphics_copy_image_pgm(graphics, x, y,
                symbol_data,
                font->byte_align,
                font->symbol_width,
                font->symbol_height,
                font->bitmaps[i].data_width);
        break;
    }
}

void font_puts(font_t* font, graphics_t* graphics, graphics_pos_t x, graphics_pos_t y, const char* s)
{
    if(x >= (graphics_pos_t)graphics_width(graphics) ||
            y >= (graphics_pos_t)graphics_height(graphics)) return;
    
    uint8_t tabs_count = 0;
    graphics_pos_t orig_x = x;
    while(*s){
        if(*s <= 0x1F) s++;
        switch(*s){
            case '\r':
                x = orig_x;
                s ++;
                break;
            case '\n':
                x = orig_x;
                y += font->symbol_height + font->vspace;
                s ++;
                break;
            case '\t':
                font_putc(font, graphics, x, y, 0x20);
                x += font->symbol_width + font->hspace;
                if(++ tabs_count == FONT_TAB_SIZE){
                    tabs_count = 0;
                    s ++;
                }
                break;
            default:
                font_putc(font, graphics, x, y, *s);
                x += font->symbol_width + font->hspace;
                s ++;
                break;
        }
        if(y >= (graphics_pos_t)graphics_width(graphics)) break;
    }
}
