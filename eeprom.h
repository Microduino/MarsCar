#include <EEPROM.h>

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

uint8_t mode = 1;
int8_t key = -1;
uint8_t speed = 150;
uint8_t color = 8;

struct config_type
{
  uint8_t EEPROMmode;
  uint8_t EEPROMspeed;
  uint8_t EEPROMcolor;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(0, config_readback);
  mode = config_readback.EEPROMmode;
  speed = config_readback.EEPROMspeed;
  color = config_readback.EEPROMcolor;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMmode = mode;
  config.EEPROMspeed = speed;
  config.EEPROMcolor = color;
  EEPROM_write(0, config);            // 变量config存储到EEPROM，地址0写入
}

void soundmode(uint8_t _mode)
{
  if (_mode == 0)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(100);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(100);
  }
  if (_mode == 1)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_RED);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
  else if (_mode == 2)
  {
    tone(PIN_BUZZER, 523);
    setAllLed(COLOR_GREEN);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 590);
    setAllLed(COLOR_GREEN);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
  else if (_mode == 3)
  {
    tone(PIN_BUZZER, 500);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 590);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
    tone(PIN_BUZZER, 660);
    setAllLed(COLOR_BLUE);
    delay(300);
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    delay(300);
  }
}
