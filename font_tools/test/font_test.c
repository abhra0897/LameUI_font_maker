//Make it so we don't need to include any other C files in our build.
#define CNFG_IMPLEMENTATION

#include "font_test.h"
#include "rawdraw_sf.h"

#define HOR_RES		480
#define VERT_RES	640

#define Y_OFFSET	80
#define X_OFFSET	1

void HandleKey( int keycode, int bDown ) { }
void HandleButton( int x, int y, int button, int bDown ) { }
void HandleMotion( int x, int y, int mask ) { }
void HandleDestroy() { }
void set_pixel_area (uint16_t p_x, uint16_t p_y, uint16_t p_width, uint16_t p_height, uint16_t p_color);
void lui_gfx_draw_string_advanced(const char* str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fore_color, uint16_t bg_color, uint8_t is_bg, const lui_font_t* font);
void lui_gfx_draw_string_simple(const char* str, uint16_t x, uint16_t y, uint16_t fore_color, const lui_font_t* font);
void _lui_gfx_render_char_glyph(uint16_t x, uint16_t y, uint16_t fore_color, uint16_t bg_color, uint8_t is_bg, const _lui_glyph_t* glyph, const lui_font_t* font);
void lui_gfx_get_string_dimension(const char* str, const lui_font_t* font, uint16_t max_w, uint16_t* str_dim);


void set_pixel_area (uint16_t p_x, uint16_t p_y, uint16_t p_width, uint16_t p_height, uint16_t p_color)
{
	// Seperating RGB565 color
	uint8_t red_5 = (p_color >> 8) & 0xF8;
	uint8_t green_6 = (p_color >> 3) & 0xFC;
	uint8_t blue_5 = (p_color << 3);
	uint32_t color = ((uint32_t)red_5 << 24) | ((uint32_t)green_6 << 16) | ((uint32_t)blue_5 << 8) | (uint32_t)0xFF;
	CNFGColor( color ); 
	CNFGTackRectangle( p_x + X_OFFSET, p_y + Y_OFFSET, p_x + p_width + X_OFFSET, p_y + p_height + Y_OFFSET); 	
}



void lui_gfx_draw_string_advanced(const char* str, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fore_color, uint16_t bg_color, uint8_t is_bg, const lui_font_t* font)
{
	uint16_t x_temp = x;
	uint16_t y_temp = y;
    const _lui_glyph_t *glyph;
	
	// Scan chars one by one from the string
	//char
	for (uint16_t char_cnt = 0; *str != '\0'; char_cnt++)
	{
		if (*str == '\n')
		{
			x_temp = x;					//go to first col
			y_temp += (font->bitmap->size_y);	//go to next row (row height = height of space)
		}
		else
		{
			// Find the glyph for the char from the font
			int i = 0;
            while (font->glyphs[i].character != *str)
            {
                ++i;
            }
            glyph = &(font->glyphs[i]);

			// check if not enough space available at the right side
			if (x_temp + glyph->width > x + w)
			{
				x_temp = x;					//go to first col
				y_temp += font->bitmap->size_y;	//go to next row

				// check if not enough space available at the bottom
				if(y_temp + font->bitmap->size_y > y + h)
					return;
			}

			_lui_gfx_render_char_glyph(x_temp, y_temp, fore_color, 0, 0, glyph, font);

			x_temp += glyph->width;		//next char position
		}

        str++;
	}
}

void lui_gfx_draw_string_simple(const char* str, uint16_t x, uint16_t y, uint16_t fore_color, const lui_font_t* font)
{
	lui_gfx_draw_string_advanced(str, x, y, 0, 0, fore_color, 0, 0, font);
}


/*
 * Draws a character glyph in left-to-right order
 * Monochrome fonts generated with lcd-image-converter software are supported only
 * Font must be generated by scanning from left to right
 *
 * Returns the last written pixel's X position
 */
void _lui_gfx_render_char_glyph(uint16_t x, uint16_t y, uint16_t fore_color, uint16_t bg_color, uint8_t is_bg, const _lui_glyph_t* glyph, const lui_font_t* font)
{
	uint16_t width = 0;

	width = glyph->width;

	uint16_t temp_x = x;
	uint16_t temp_y = y;

	int index_offset = glyph->payload_index;//((height / 8) + (height % 8 ? 1 : 0)) * x_offset;
    int mask = 0x80;
    uint8_t bit_counter = 0;
    for (int w = 0; w < width; w++)
    {
        bit_counter = 0;
        for (int h = 0; h < font->bitmap->size_y; h++)
        {
            if (bit_counter >= 8)   
            {
                ++index_offset;
                bit_counter = 0;
            }
            int bit = mask & (font->bitmap->payload[index_offset] << bit_counter);
            if (bit)
            {
                set_pixel_area(temp_x, temp_y, 1, 1, fore_color);
                //printf("%d\t", 1);
            }
            else
            {
                if (is_bg)
                    set_pixel_area(temp_x, temp_y, 1, 1, bg_color);
                //printf("%d\t", 0);
            }
            ++bit_counter;
            ++temp_y;
        }
        ++index_offset;
        ++temp_x;
        temp_y = y;
    }
}

/*
 * Get the width and height of a string (in pixels).
 * Width: by adding up the width of each glyph (representing a character)
 * Height: Height of any glyph (representing a character)
 */
void lui_gfx_get_string_dimension(const char* str, const lui_font_t* font, uint16_t max_w, uint16_t* str_dim)
{
	str_dim[0] = 0;	// -> width
	str_dim[1] = 0;	// -> height

	uint8_t needs_wrap = 0;
	uint16_t temp_w = 0;
	uint16_t temp_w_highest = 0;
	// height is the height of space
	uint16_t temp_h = font->bitmap->size_y;

	// Scan chars one by one from the string
	for (uint16_t char_cnt = 0; *str != '\0'; char_cnt++)
	{
		if (*str == '\n')
		{
			temp_h += font->bitmap->size_y;
			temp_w = 0;
		}
		else
		{
            // Find the glyph for the char from the font
			int i = 0;
            while (font->glyphs[i].character != *str)
            {
                ++i;
            }
            const _lui_glyph_t* glyph = &(font->glyphs[i]);

            // Add width of glyphs
            if (temp_w + glyph->width > max_w)
            {
                temp_h += font->bitmap->size_y;
                temp_w = 0;
                needs_wrap = 1;
            }

            temp_w += glyph->width;
            temp_w_highest = temp_w_highest < temp_w ? temp_w : temp_w_highest;
		}

        str++;
	}
	str_dim[0] = needs_wrap ? max_w : temp_w_highest;
	str_dim[1] = temp_h; 

}


int main()
{
	CNFGSetup( "LameUI Sim", HOR_RES + X_OFFSET, VERT_RES + Y_OFFSET );
	CNFGBGColor = 0xffffffff; //Dark Black Background
	short w, h;
	CNFGClearFrame();
	CNFGHandleInput();
	CNFGGetDimensions( &w, &h );
    
    const lui_font_t* font1 =  &FONT_montserrat_regular_17;
    const lui_font_t* font2 =  &FONT_dejavu_sans_extralight_20;
    const lui_font_t* font3 =  &FONT_montserrat_regular_32;
	const lui_font_t* font4 =  &FONT_ubuntu_mono_regular_32;

    char* txt = "\x03\x04Hello! There \x05\x07\x1A";
    uint16_t str_w_h[2];
    lui_gfx_get_string_dimension(txt, font1, HOR_RES - X_OFFSET, str_w_h);
    lui_gfx_draw_string_advanced(txt, 10, 10, str_w_h[0], str_w_h[1], 0, 0xFF00, 1, font1);

    lui_gfx_get_string_dimension(txt, font2, HOR_RES - X_OFFSET, str_w_h);
    lui_gfx_draw_string_advanced(txt, 10, 40, str_w_h[0], str_w_h[1], 0, 0xFF00, 1, font2);

    lui_gfx_get_string_dimension(txt, font3, HOR_RES - X_OFFSET, str_w_h);
    lui_gfx_draw_string_advanced(txt, 10, 80, str_w_h[0], str_w_h[1], 0, 0xFF00, 1, font3);
	
	lui_gfx_get_string_dimension(txt, font4, HOR_RES - X_OFFSET, str_w_h);
    lui_gfx_draw_string_advanced(txt, 10, 120, str_w_h[0], str_w_h[1], 0, 0xFF00, 1, font4);
    CNFGSwapBuffers();
    while (1)
    {        
        CNFGSwapBuffers();
        sleep(2); //wait 15ms to save CPU
    }
} 