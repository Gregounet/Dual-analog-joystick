#if !defined(SPRITES_H)
#define SPRITES_H

#include <stdint.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

#define NB_SPRITES 19
#define NB_COLORS 8

#define LJ_HORIZONTAL_PIN A0
#define LJ_VERTICAL_PIN A1
#define LJ_FIRE_PIN 7
#define RJ_HORIZONTAL_PIN A2
#define RJ_VERTICAL_PIN A3
#define RJ_FIRE_PIN 6
#define BUTTON_PIN 5
#define LED 4

#define TFT_CS 10
#define TFT_RST -1
#define TFT_DC 9

#define DEBUG_SERIAL

extern uint8_t left_x, left_y;
extern uint8_t right_x, right_y;

extern int8_t left_h_speed, left_v_speed;
extern int8_t right_h_speed, right_v_speed;

extern Adafruit_ST7735 tft;

int show_sprite(uint8_t sprite_number, uint8_t sprite_x, uint8_t sprite_y, uint8_t sprite_color_index);

#endif // (SPRITES_H)
