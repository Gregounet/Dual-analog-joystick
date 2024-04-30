#include <Arduino.h>

#define LJ_HORIZONTAL_PIN A0
#define LJ_VERTICAL_PIN A1
#define LJ_FIRE_PIN 7
#define RJ_HORIZONTAL_PIN A2
#define RJ_VERTICAL_PIN A3
#define RJ_FIRE_PIN 6
#define BUTTON_PIN 5
#define LED 4

#undef DEBUG_SERIAL

#define OUTPUT_B0 8
#define OUTPUT_B1 9
#define OUTPUT_B2 10
#define OUTPUT_B3 11
#define OUTPUT_B4 12

#define SELECT_JOYSTICK 13

void setup()
{
  Serial.begin(9600);
  delay(100);
  pinMode(LJ_HORIZONTAL_PIN, INPUT);
  pinMode(LJ_VERTICAL_PIN, INPUT);
  pinMode(RJ_HORIZONTAL_PIN, INPUT);
  pinMode(RJ_VERTICAL_PIN, INPUT);

  pinMode(LJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(RJ_FIRE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED, OUTPUT);

  pinMode(OUTPUT_B0, OUTPUT);
  pinMode(OUTPUT_B1, OUTPUT);
  pinMode(OUTPUT_B2, OUTPUT);
  pinMode(OUTPUT_B3, OUTPUT);
  pinMode(OUTPUT_B4, OUTPUT);

  pinMode(SELECT_JOYSTICK, INPUT);

  digitalWrite(OUTPUT_B0, HIGH);
  digitalWrite(OUTPUT_B1, HIGH);
  digitalWrite(OUTPUT_B2, HIGH);
  digitalWrite(OUTPUT_B3, HIGH);
  digitalWrite(OUTPUT_B4, HIGH);
}

void loop()
{
  delay(100);

  uint8_t transmit = 0xFF;

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
  delay(1000);
#endif

  if (digitalRead(SELECT_JOYSTICK) == LOW)
  {
    if (lj_horizontal < 255)
    {
      digitalWrite(OUTPUT_B1, HIGH);
      digitalWrite(OUTPUT_B3, LOW);
      transmit &= 0xFE;
      // Serial.println("Etape A");
    }
    if (lj_horizontal > 767)
    {
      digitalWrite(OUTPUT_B1, LOW);
      digitalWrite(OUTPUT_B3, HIGH);
      transmit &= 0xFB;
      // Serial.println("Etape B");
    }
    if (lj_horizontal >= 255 && lj_horizontal <= 767)
    {
      digitalWrite(OUTPUT_B1, HIGH);
      digitalWrite(OUTPUT_B3, HIGH);
      transmit &= 0xFF;
      // Serial.println("Etape C");
    }
    if (lj_vertical < 255)
    {
      digitalWrite(OUTPUT_B2, HIGH);
      digitalWrite(OUTPUT_B0, LOW);
      transmit &= 0xF7;
      // Serial.println("Etape D");
    }
    if (lj_vertical > 767)
    {
      digitalWrite(OUTPUT_B2, LOW);
      digitalWrite(OUTPUT_B0, HIGH);
      transmit &= 0xFD;
      // Serial.println("Etape E");
    }
    if (lj_vertical >= 255 && lj_vertical <= 767)
    {
      digitalWrite(OUTPUT_B2, HIGH);
      digitalWrite(OUTPUT_B0, HIGH);
      transmit &= 0xFF;
      // Serial.println("Etape F");
    }
    if (lj_button == 0)
    {
      digitalWrite(OUTPUT_B4, LOW);
      transmit &= 0xEF;
    }
    else
    {
      digitalWrite(OUTPUT_B4, HIGH);
      transmit &= 0xFF;
    }
  }
  else
  {
    if (rj_horizontal < 255)
    {
      digitalWrite(OUTPUT_B1, HIGH);
      digitalWrite(OUTPUT_B3, LOW);
      transmit &= 0xFE;
    }
    if (rj_horizontal > 767)
    {
      digitalWrite(OUTPUT_B1, LOW);
      digitalWrite(OUTPUT_B3, HIGH);
      transmit &= 0xFB;
    }
    if (rj_horizontal >= 255 && lj_horizontal <= 767)
    {
      digitalWrite(OUTPUT_B1, HIGH);
      digitalWrite(OUTPUT_B3, HIGH);
      transmit &= 0xFF;
    }
    if (rj_vertical < 255)
    {
      digitalWrite(OUTPUT_B2, HIGH);
      digitalWrite(OUTPUT_B0, LOW);
      transmit &= 0xF7;
    }
    if (rj_vertical > 767)
    {
      digitalWrite(OUTPUT_B2, LOW);
      digitalWrite(OUTPUT_B0, HIGH);
      transmit &= 0xFD;
    }
    if (rj_vertical >= 255 && lj_vertical <= 767)
    {
      digitalWrite(OUTPUT_B2, HIGH);
      digitalWrite(OUTPUT_B0, HIGH);
      transmit &= 0xFF;
    }
    if (rj_button == 0)
    {
      digitalWrite(OUTPUT_B4, LOW);
      transmit &= 0xEF;
    }
    else
    {
      digitalWrite(OUTPUT_B4, HIGH);
      transmit &= 0xFF;
    }
  }
  Serial.print("Transmit : ");
  Serial.println(transmit, HEX);

  //  digitalWrite(OUTPUT_B0, LOW);
}
/*
 *
 * b0: up
 * b1: right
 * b2: down
 * b3: left
 * b4: fire
 *
 */
