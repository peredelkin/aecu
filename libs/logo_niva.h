#ifndef LOGO_NIVA
#define LOGO_NIVA

#include <avr/pgmspace.h>

#define LOGO_NIVA_WIDTH 128
#define LOGO_NIVA_HEIGHT 64

const uint8_t logo_niva_data[] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0xF0, 0xF0, 
0xF0, 0xE8, 0xE8, 0xE8, 0xE8, 0xF8, 0xF8, 0xF8, 
0xF8, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 0xF4, 
0xF4, 0xF4, 0xF4, 0xF4, 0xE4, 0xE4, 0xE4, 0xE4, 
0xE4, 0xE4, 0xE4, 0x64, 0x04, 0x84, 0x64, 0xA4, 
0xA4, 0xA4, 0xA4, 0xA4, 0xA8, 0xA8, 0xA8, 0xA8, 
0xE8, 0xE8, 0x68, 0x68, 0x68, 0x48, 0x48, 0x48, 
0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 
0x50, 0x90, 0x90, 0x90, 0xA0, 0xA0, 0xA0, 0xA0, 
0xA0, 0xA0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x80, 0xE0, 0xF0, 0xF8, 0x7E, 
0x1F, 0x0F, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 
0x01, 0x01, 0x01, 0x01, 0x01, 0xC1, 0xFF, 0xFF, 
0xFF, 0x03, 0x81, 0xE1, 0xF1, 0xFB, 0xFB, 0xFB, 
0xFB, 0xF3, 0xE3, 0x83, 0x1F, 0x03, 0x03, 0xFF, 
0x7F, 0x0F, 0xC3, 0xB8, 0x86, 0x81, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0F, 0xFC, 0x00, 0x3F, 0xC1, 0x01, 0x01, 
0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x02, 
0x03, 0x03, 0x7E, 0x80, 0x0E, 0xF3, 0x02, 0x02, 
0x02, 0x0E, 0x39, 0xC3, 0x03, 0x0C, 0x70, 0xC0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 
0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 
0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x1C, 
0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1B, 0x0A, 0x0A, 
0x0A, 0x0A, 0x0C, 0x0C, 0x0C, 0x04, 0x85, 0x85, 
0x45, 0x45, 0x45, 0x55, 0x55, 0x4F, 0x37, 0x37, 
0x3F, 0x3E, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 
0x2F, 0x2F, 0x2F, 0x2F, 0x2A, 0x36, 0x0C, 0x03, 
0x1C, 0x3F, 0xCA, 0x1F, 0x0F, 0x0F, 0x0F, 0x0B, 
0x08, 0x08, 0x0C, 0x08, 0x0E, 0x0E, 0x0E, 0x08, 
0x08, 0x08, 0x0F, 0x0F, 0x00, 0x0F, 0xF8, 0x08, 
0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x10, 0x18, 
0x10, 0x10, 0x18, 0x1F, 0x00, 0x03, 0xFC, 0x10, 
0x10, 0x10, 0x10, 0x11, 0x17, 0x18, 0x00, 0x01, 
0x06, 0x38, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFE, 0xFF, 
0x7F, 0x7B, 0xFF, 0xFF, 0xE7, 0xE1, 0xE1, 0xE1, 
0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 
0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 0xF1, 
0xF1, 0xF1, 0xE1, 0xE1, 0xE1, 0xEF, 0xEF, 0x6F, 
0x6F, 0x6F, 0xEB, 0xEF, 0xEF, 0xFF, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x84, 0x84, 0xC4, 0xC4, 0xC4, 0xC2, 0xC2, 0x82, 
0x82, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
0x02, 0x02, 0xFF, 0x02, 0x02, 0x02, 0x02, 0x02, 
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
0x02, 0x02, 0x02, 0x03, 0x03, 0x02, 0x07, 0xFA, 
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x85, 0x7E, 
0x04, 0x04, 0x04, 0x84, 0x84, 0x04, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x07, 0xFC, 0x00, 0x00, 0x00, 
0x00, 0xC0, 0xC0, 0x80, 0x80, 0xFF, 0x9D, 0xF0, 
0xF1, 0xF1, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0x3F, 0xFF, 0xBF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xBF, 0xBF, 
0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xB8, 0xF1, 
0xF0, 0xF0, 0xF8, 0xFF, 0xDF, 0xFF, 0xC0, 0xC0, 
0x80, 0x00, 0x80, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFE, 0xF8, 0xC0, 0x60, 0xE0, 0xE0, 
0xE0, 0xE0, 0xFF, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xFC, 0xE3, 
0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 
0xE0, 0xE0, 0xE0, 0x60, 0x18, 0xC6, 0xF1, 0xFC, 
0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 
0xF8, 0x00, 0xF0, 0xCE, 0xC3, 0xC0, 0xC0, 0x00, 
0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xF0, 0xF0, 0xF0, 
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xEF, 0x3F, 0x2F, 0x0F, 0x43, 0xD3, 
0x13, 0x01, 0x13, 0x87, 0x07, 0x1F, 0xFB, 0xDF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 
0x03, 0xC3, 0x93, 0xB7, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x07, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xED, 0xD8, 0xA0, 0xA2, 0x33, 
0x23, 0x31, 0x31, 0x90, 0x98, 0xCC, 0xE7, 0xF8, 
0xFF, 0xFF, 0xBF, 0xBF, 0xBF, 0xBF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xBF, 0x87, 0x83, 0x83, 0x81, 0x81, 0x81, 
0x81, 0x80, 0x81, 0x81, 0x83, 0x83, 0x83, 0x83, 
0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 
0x81, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFE, 0xDD, 0xC9, 0xC0, 0xEC, 0xF7, 0x3F, 0x07, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 
0x0C, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F, 
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 
0x1F, 0x1F, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 
0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 
0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 
0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
#endif /* LOGO_NIVA */