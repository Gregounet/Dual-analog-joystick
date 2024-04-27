#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "sprites.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

#define NB_SPRITES 19
#define NB_COLORS 8

const uint8_t sprites_bytes[] =
    {
        0x81, 0x5A, 0x3C, 0x5A, 0xFF, 0x81, 0x42, 0x3C,                                                 // bytes 0x1960 <-> 0x196F	(cart, bank 3, page 6)
        0x81, 0x5A, 0x3C, 0x5A, 0xFF, 0xFF, 0x7E, 0x3C, 0x81, 0x5A, 0x3C, 0x5A, 0xFF, 0xE7, 0x42, 0x00, // bytes 0x1970 <-> 0x197F	(cart, bank 3, page 6)
        0x81, 0x5A, 0x3C, 0x5A, 0xFF, 0x00, 0x00, 0x00, 0x81, 0x5A, 0x3C, 0x18, 0x18, 0x00, 0x00, 0x00, // bytes 0x1980 <-> 0x198F	(cart, bank 3, page 6)
        0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x62, 0x24, 0x00, 0xE7, 0x24, 0x52, 0x92, 0x91, // bytes 0x1990 <-> 0x199F	(cart, bank 3, page 6)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x89, // bytes 0x19A0 <-> 0x19AF	(cart, bank 3, page 6)
        0x18, 0x3C, 0x7E, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0xBE, 0x6C, 0x78, 0xE0, 0x70, 0x3C, 0x1E, 0x00, // bytes 0x19B0 <-> 0x19BF	(cart, bank 3, page 6)
        0x7D, 0x36, 0x1E, 0x07, 0x0E, 0x3C, 0x78, 0x00, 0x81, 0xC3, 0xE7, 0xB7, 0xFE, 0x7C, 0x90, 0x00, // bytes 0x19C0 <-> 0x19CF	(cart, bank 3, page 6)
        0x90, 0x7C, 0xFE, 0xB7, 0xE7, 0xC3, 0x81, 0x00, 0x81, 0x5A, 0x3C, 0x5A, 0xFF, 0xBD, 0x42, 0x3C, // bytes 0x19D0 <-> 0x19DF	(cart, bank 3, page 6)
        0x3C, 0x7E, 0xDB, 0xDB, 0xFF, 0x7E, 0xDB, 0x89, 0xBD, 0x7E, 0xED, 0xED, 0xFF, 0x7E, 0xDB, 0x89, // bytes 0x19E0 <-> 0x19EF	(cart, bank 3, page 6)
        0xBD, 0x7E, 0xB7, 0xB7, 0xFF, 0x7E, 0xDB, 0x89, 0x3C, 0x7E, 0xFF, 0xDB, 0xFF, 0x7E, 0xDB, 0x89  // bytes 0x19F0 <-> 0x19FF	(cart, bank 3, page 6)
};

#define O2_COLOR_LIGHT_GREY 0xC618
#define O2_COLOR_VIOLET 0xC418
#define O2_COLOR_LIGHT_GREEN 0x87F0

const uint16_t light_colors[NB_COLORS] = {
    ST77XX_WHITE,
    ST77XX_BLUE,
    ST77XX_GREEN,
    O2_COLOR_LIGHT_GREEN,
    ST77XX_RED,
    O2_COLOR_VIOLET,
    ST77XX_ORANGE,
    O2_COLOR_LIGHT_GREY};

int show_sprite(uint8_t sprite_number, uint8_t sprite_x, uint8_t sprite_y, uint8_t sprite_color_index)
{
    if (sprite_number >= NB_SPRITES || sprite_x >= SCREEN_WIDTH || sprite_y >= SCREEN_HEIGHT || sprite_color_index >= NB_COLORS)
        return -1;

    uint16_t sprite_color = light_colors[sprite_color_index];

    for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
    {
        uint8_t sprite_data = sprites_bytes[sprite_number * 0x08 + sprite_row];
        uint8_t mask = 0x01;
        for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
        {
            if (sprite_data & mask)
                tft.fillRect((sprite_x + sprite_column)*2, (sprite_y + sprite_row)*2, 2, 2, sprite_color);
            mask <<= 1;
        }
    }
}

/*
sprites_uptodate = 1;
for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
{
    if (displayed_sprites[sprite_number].changed_displayed & 0x02) // Sprite data was changed
    {
        displayed_sprites[sprite_number].changed_displayed &= 0xFD; // Clear change flag

        //
        // "Erase" old sprite position
        //
        graphic_tft.fillRect(
            20 + displayed_sprites[sprite_number].previous_start_x,
            displayed_sprites[sprite_number].previous_start_y,
            16 * displayed_sprites[sprite_number].previous_size,
            16 * displayed_sprites[sprite_number].previous_size,
            background_color);
        //
        // Record new position as previous one
        //
        displayed_sprites[sprite_number].previous_start_x = displayed_sprites[sprite_number].start_x;
        displayed_sprites[sprite_number].previous_start_y = displayed_sprites[sprite_number].start_y;
        displayed_sprites[sprite_number].previous_size = displayed_sprites[sprite_number].size;

#if defined(DEBUG_SERIAL)
        Serial.print("show_sprites() - sprite numéro ");
        Serial.println(sprite_number);
#endif
        for (uint8_t sprite_row = 0; sprite_row < 8; sprite_row++)
        {
            uint8_t sprite_data = intel8245_ram[0x80 + sprite_number * 0x08 + sprite_row];
#if defined(DEBUG_SERIAL)
            Serial.print("show_sprites() - affichage de la ligne ");
            Serial.print(sprite_row);
            Serial.print(" - data : 0x");
            Serial.println(sprite_data, HEX);
            Serial.print("start_x : ");
            Serial.println(displayed_sprites[sprite_number].start_x);
            Serial.print("start_y : ");
            Serial.println(displayed_sprites[sprite_number].start_y);
            Serial.print("color : 0x");
            Serial.println(displayed_sprites[sprite_number].color, HEX);
            Serial.print("size : ");
            Serial.println(displayed_sprites[sprite_number].size);
#endif

            uint8_t mask = 0x01;

            for (uint8_t sprite_column = 0; sprite_column < 8; sprite_column++)
            {
                if (sprite_data & mask)
                {
                    graphic_tft.fillRect(
                        20 + displayed_sprites[sprite_number].start_x + sprite_column * 2 * displayed_sprites[sprite_number].size,
                        displayed_sprites[sprite_number].start_y + sprite_row * 2 * displayed_sprites[sprite_number].size,
                        2 * displayed_sprites[sprite_number].size,
                        2 * displayed_sprites[sprite_number].size,
                        displayed_sprites[sprite_number].color);
                }
                mask <<= 1;
            }
        }
    }

    */
