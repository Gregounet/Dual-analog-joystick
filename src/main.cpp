#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

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

#define NB_CALIBRATION_CYCLES 5
#define SPEED_LIMIT 1000.0

#define DEBUG_SERIAL

int8_t left_x, left_y;
int8_t right_x, right_y;

int8_t left_h_speed, left_v_speed;
int8_t right_h_speed, right_v_speed;

int16_t left_h_center, left_v_center;
int16_t right_h_center, right_v_center;

float left_h_left_scale;
float left_h_right_scale;
float left_v_up_scale;
float left_v_down_scale;

float right_h_left_scale;
float right_h_right_scale;
float right_v_up_scale;
float right_v_down_scale;

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

  Serial.println("Calibration");

  int32_t left_h, left_v, right_h, right_v;
  left_h = left_v = right_h = right_v = 0;

  for (uint8_t cycle = 0; cycle < NB_CALIBRATION_CYCLES; cycle++)
  {
    left_h += analogRead(LJ_HORIZONTAL_PIN);
    left_v += analogRead(LJ_VERTICAL_PIN);
    right_h += analogRead(RJ_HORIZONTAL_PIN);
    right_v += analogRead(RJ_VERTICAL_PIN);
    delay(100);
  }
  Serial.println("Calibration over");

#if defined(DEBUG_SERIAL)
  Serial.print("Left - h = ");
  Serial.print(left_h);
  Serial.print(", v = ");
  Serial.print(left_v);
  Serial.print(" -- Right - h = ");
  Serial.print(right_h);
  Serial.print(", v = ");
  Serial.println(right_v);
#endif

  left_h_center = left_h / NB_CALIBRATION_CYCLES;
  left_v_center = left_v / NB_CALIBRATION_CYCLES;
  right_h_center = right_h / NB_CALIBRATION_CYCLES;
  right_v_center = right_v / NB_CALIBRATION_CYCLES;

#if defined(DEBUG_SERIAL)
  Serial.print("Left - h center = ");
  Serial.print(left_h_center);
  Serial.print(", v center = ");
  Serial.print(left_v_center);
  Serial.print(" -- Right - h center = ");
  Serial.print(right_h_center);
  Serial.print(", v center = ");
  Serial.println(right_v_center);
#endif

  left_h_left_scale = (1.0 / ((float)(left_h_center))) * SPEED_LIMIT;
  left_h_right_scale = (1.0 / ((float)(1023.0 - left_h_center))) * SPEED_LIMIT;
  left_v_up_scale = (1.0 / ((float)(left_v_center))) * SPEED_LIMIT;
  left_v_down_scale = (1.0 / ((float)(1023.0 - left_v_center))) * SPEED_LIMIT;

  right_h_left_scale = (1.0 / ((float)(right_h_center))) * SPEED_LIMIT;
  right_h_right_scale = (1.0 / ((float)(1023.0 - right_h_center))) * SPEED_LIMIT;
  right_v_up_scale = (1.0 / ((float)(right_v_center))) * SPEED_LIMIT;
  right_v_down_scale = (1.0 / ((float)(1023.0 - right_v_center))) * SPEED_LIMIT;

#if defined(DEBUG_SERIAL)
  Serial.print("Left - left sc = ");
  Serial.print(left_h_left_scale);
  Serial.print(", right sc = ");
  Serial.print(left_h_right_scale);
  Serial.print(", up sc = ");
  Serial.print(left_v_up_scale);
  Serial.print(", down sc = ");
  Serial.println(left_v_down_scale);

  Serial.print("Right - left sc = ");
  Serial.print(right_h_left_scale);
  Serial.print(", right sc = ");
  Serial.print(right_h_right_scale);
  Serial.print(", up sc = ");
  Serial.print(right_v_up_scale);
  Serial.print(", down sc = ");
  Serial.println(right_v_down_scale);
  delay(10000);
#endif
}

void loop()
{

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

  left_h_speed =
      (lj_horizontal < left_h_center)
          ? (((float)lj_horizontal - (float)left_h_center) * left_h_left_scale)
          : (((float)lj_horizontal - (float)left_h_center) * left_h_left_scale);

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
