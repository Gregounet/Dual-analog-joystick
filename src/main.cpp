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

#define DEBUG_SERIAL

uint8_t left_x, left_y;
uint8_t right_x, right_y;

int8_t left_h_speed, left_v_speed;
int8_t right_h_speed, right_v_speed;

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
  tft.print("Funky funk");

  pinMode(LJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(RJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  uint8_t color = 0;
  for (uint8_t spr = 0; spr < NB_SPRITES; spr++)
  {
    show_sprite(spr, (spr % 5) * 10, 5 + (spr / 5) * 10, spr % 8);
  }
}

void loop()
{
  delay(1000);
  return;

  int16_t lj_horizontal = analogRead(LJ_HORIZONTAL_PIN);
  int16_t lj_vertical = analogRead(LJ_VERTICAL_PIN);
  int16_t rj_horizontal = analogRead(RJ_HORIZONTAL_PIN);
  int16_t rj_vertical = analogRead(RJ_VERTICAL_PIN);

#if defined(DEBUG_SERIAL)
  Serial.print("Left - horizontal = ");
  Serial.print(lj_horizontal);
  Serial.print(", vertical = ");
  Serial.print(lj_vertical);
  Serial.print(" -- Right - horizontal = ");
  Serial.print(rj_horizontal);
  Serial.print(", vertical = ");
  Serial.println(rj_vertical);
#endif

  left_h_speed = left_v_speed = right_h_speed = right_v_speed = 0;

  if (lj_horizontal < 255)
    left_h_speed = -1;
  if (lj_horizontal > 767)
    left_h_speed = +1;
  if (lj_vertical < 255)
    left_v_speed = +1;
  if (lj_vertical > 767)
    left_v_speed = -1;

  if (rj_horizontal < 255)
    right_h_speed = -1;
  if (rj_horizontal > 767)
    right_h_speed = +1;
  if (rj_vertical < 255)
    right_v_speed = +1;
  if (rj_vertical > 767)
    right_v_speed = -1;

#if defined(DEBUG_SERIAL)
  Serial.print("Left - h speed = ");
  Serial.print(left_h_speed);
  Serial.print(", v speed = ");
  Serial.print(left_v_speed);
  Serial.print(" -- Right - h speed = ");
  Serial.print(right_h_speed);
  Serial.print(", v speed= ");
  Serial.println(right_v_speed);
#endif

  delay(1000);
}
