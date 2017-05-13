#include "userDef.h"
#include "colorLed.h"
#include "eeprom.h"
#include "carBLEJson.h"
#include "motor.h"

uint16_t lineValA;
uint16_t lineValB;
uint16_t micVal;

void setup() {
  Serial.begin(57600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LINEA, INPUT);
  pinMode(PIN_LINEB, INPUT);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);

  MotorLeft.Fix(1.1);
  MotorRight.Fix(1);

  read_eeprom();

  if (mode < 0 || mode > 3 || speed < 100 || speed > 255 || color < 1 || color > 9)
  {
    mode = 1;
    speed = 150;
    color = 8;
  }

#if DEBUG
  Serial.println("--------------");
  Serial.print("MODE DATA:");
  Serial.println(mode);
  Serial.print("KEY DATA:");
  Serial.println(key);
  Serial.print("SPEED DATA:");
  Serial.println(speed);
  Serial.print("COLOR DATA:");
  Serial.println(color);
  Serial.println("--------------");
#endif

  for (int i = 0; i < 10; i++)
  {
    setAllLed(i);
    delay(150);   //每个颜色持续时间
  }
}

void loop() {

  lineValA = analogRead(PIN_LINEA);
  lineValB = analogRead(PIN_LINEB);

  Serial.print("\tLINEA Val:");
  Serial.print(lineValA);
  Serial.print("\tLINEB Val:");
  Serial.println(lineValB);

  Carblejson_data();
  switch (mode)
  {
    case MODE_A:
      bleControl(key);
      break;
    case MODE_B:
      trackControl(lineValA, lineValB);
      break;
    case MODE_C:
      mazeControl(lineValA, lineValB);
      break;
    default:
      motorCoast();
      break;
  }

  motorUpdate(leftSpeed, rightSpeed);

  if (buzzerS == "true")
    tone(PIN_BUZZER, 1000);
  else
    noTone(PIN_BUZZER);
}
