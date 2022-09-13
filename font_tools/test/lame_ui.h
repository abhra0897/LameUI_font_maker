#include <stdint.h>

#ifndef _INC_LAME_UI_H
#define _INC_LAME_UI_H
 
/* This is a generic bitmap */
typedef struct {
    const uint16_t size_x; //size is in pixels
    const uint16_t size_y;
    const uint8_t * const payload;
} lui_bitmap_t;
 
/* This is a font glyph description - for now it does not support kerning */
typedef struct {
    const char character; //ASCII code
    const uint8_t width;
    const uint16_t payload_index;
} _lui_glyph_t;
 
typedef struct {
    const lui_bitmap_t * const bitmap;
    const uint8_t glyph_count;
    const _lui_glyph_t *glyphs; //pointer to array of glypt_t elements
} lui_font_t; 

#endif /*_INC_LAME_UI_H*/