#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include "sprites.h"

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

#undef DEBUG_SERIAL

int8_t munch_x, munch_y;
int8_t ghost_x, ghost_y;

int8_t munch_h_speed, munch_v_speed, munch_boost;
int8_t ghost_h_speed, ghost_v_speed, ghost_boost;

uint8_t munch_sprites[3][3] = {{10, 10, 10}, {12, 14, 13}, {11, 11, 11}};
uint8_t ghost_sprites[3][3] = {{16, 16, 16}, {18, 15, 18}, {17, 17, 17}};

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
  Serial.begin(9600);
  delay(100);

  tft.initR(INITR_BLACKTAB);
  tft.setSPISpeed(100000000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(0);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(20, 0);
  tft.print("Disco disco");

  pinMode(LJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(RJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  munch_x = 40;
  munch_y = 20;
  munch_boost = 1;

  ghost_x = 20;
  ghost_y = 60;
  ghost_boost = 1;

  /*
    uint8_t color = 0;
    for (uint8_t spr = 0; spr < NB_SPRITES; spr++)
    {
      show_sprite(spr, (spr % 5) * 10, 5 + (spr / 5) * 10, spr % 8);
    }
  */
}

void loop()
{
  static uint8_t munch_color = 1;
  static uint8_t previous_munch_color;
  static uint8_t munch_sprite = 14;
  static uint8_t previous_munch_sprite;
  static uint8_t previous_munch_x;
  static uint8_t previous_munch_y;

  static uint8_t ghost_color = 4;
  static uint8_t previous_ghost_color;
  static uint8_t ghost_sprite = 15;
  static uint8_t previous_ghost_sprite;
  static uint8_t previous_ghost_x;
  static uint8_t previous_ghost_y;

  delay(100);

  int16_t lj_horizontal = analogRead(LJ_HORIZONTAL_PIN);
  int16_t lj_vertical = analogRead(LJ_VERTICAL_PIN);
  int16_t rj_horizontal = analogRead(RJ_HORIZONTAL_PIN);
  int16_t rj_vertical = analogRead(RJ_VERTICAL_PIN);

  int8_t lj_button = digitalRead(LJ_FIRE_PIN);
  int8_t rj_button = digitalRead(RJ_FIRE_PIN);

  int8_t main_button = digitalRead(BUTTON_PIN);

#if defined(DEBUG_SERIAL)
  Serial.print("Left - horizontal = ");
  Serial.print(lj_horizontal);
  Serial.print(", vertical = ");
  Serial.print(lj_vertical);
  Serial.print(" -- Right - horizontal = ");
  Serial.print(rj_horizontal);
  Serial.print(", vertical = ");
  Serial.println(rj_vertical);
  Serial.print("Left - button = ");
  Serial.print(lj_button);
  Serial.print(" -- Main - button = ");
  Serial.print(main_button);
  Serial.print(" -- Right - button = ");
  Serial.println(rj_button);
#endif

  if (lj_button == LOW)
    ghost_boost = 2;
  else
    ghost_boost = 1;

  if (rj_button == LOW)
    munch_boost = 2;
  else
    munch_boost = 1;

  if (main_button == LOW)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);

  ghost_h_speed = ghost_v_speed = munch_h_speed = munch_v_speed = 0;

  if (lj_horizontal < 255)
    ghost_h_speed = -1 * ghost_boost;
  if (lj_horizontal > 767)
    ghost_h_speed = +1 * ghost_boost;
  if (lj_vertical < 255)
    ghost_v_speed = -1 * ghost_boost;
  if (lj_vertical > 767)
    ghost_v_speed = +1 * ghost_boost;

  if (rj_horizontal < 255)
    munch_h_speed = -1 * ghost_boost;
  if (rj_horizontal > 767)
    munch_h_speed = +1 * ghost_boost;
  if (rj_vertical < 255)
    munch_v_speed = -1 * ghost_boost;
  if (rj_vertical > 767)
    munch_v_speed = +1 * ghost_boost;

#if defined(DEBUG_SERIAL)
  Serial.print("Left - h speed = ");
  Serial.print(ghost_h_speed);
  Serial.print(", v speed = ");
  Serial.print(ghost_v_speed);
  Serial.print(" -- Right - h speed = ");
  Serial.print(munch_h_speed);
  Serial.print(", v speed= ");
  Serial.println(munch_v_speed);
#endif

  munch_color = (munch_boost == 1) ? 1 : 2;
  munch_sprite = munch_sprites[munch_h_speed + 1][munch_v_speed + 1];
  munch_x += munch_h_speed;
  munch_x = (munch_x < 0) ? 0 : munch_x;
  munch_x = (munch_x > (SCREEN_WIDTH - 16) / 2) ? (SCREEN_WIDTH - 16) / 2 : munch_x;
  munch_y += munch_v_speed;
  munch_y = (munch_y < 0) ? 0 : munch_y;
  munch_y = (munch_y > (SCREEN_HEIGHT - 16) / 2) ? (SCREEN_HEIGHT - 16) / 2 : munch_y;

  if (munch_color != previous_munch_color || munch_sprite != previous_munch_sprite || munch_x != previous_munch_x || munch_y != previous_munch_y)
  {
    erase(previous_munch_x, previous_munch_y, ST7735_BLACK);
    show_sprite(munch_sprite, munch_x, munch_y, munch_color);
    previous_munch_color = munch_color;
    previous_munch_sprite = munch_sprite;
    previous_munch_x = munch_x;
    previous_munch_y = munch_y;
  }

  ghost_color = (ghost_boost == 1) ? 4 : 0;
  ghost_sprite = ghost_sprites[ghost_h_speed + 1][ghost_v_speed + 1];
  ghost_x += ghost_h_speed;
  ghost_x = (ghost_x < 0) ? 0 : ghost_x;
  ghost_x = (ghost_x > (SCREEN_WIDTH - 16) / 2) ? (SCREEN_WIDTH - 16) / 2 : ghost_x;
  ghost_y += ghost_v_speed;
  ghost_y = (ghost_y < 0) ? 0 : ghost_y;
  ghost_y = (ghost_y > (SCREEN_HEIGHT - 16) / 2) ? (SCREEN_HEIGHT - 16) / 2 : ghost_y;
  if (ghost_color != previous_ghost_color || ghost_sprite != previous_ghost_sprite || ghost_x != previous_ghost_x || ghost_y != previous_ghost_y)
  {
    erase(previous_ghost_x, previous_ghost_y, ST7735_BLACK);
    show_sprite(ghost_sprite, ghost_x, ghost_y, ghost_color);
    previous_ghost_color = ghost_color;
    previous_ghost_sprite = ghost_sprite;
    previous_ghost_x = ghost_x;
    previous_ghost_y = ghost_y;
  }
  delay(0);
}
