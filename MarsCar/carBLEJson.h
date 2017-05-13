#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String modeS = "";
String keyS = "";
String speedS = "";
String colorS = "";
String buzzerS = "";
String projtctid = "";

String ID = "{\"ID\":\"591411b2d4e8860001bc437a\"}";

#define  MODE_OFF     0
#define  MODE_A       1
#define  MODE_B       2
#define  MODE_C       3

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
}

void uploaddata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}

//{"mode":"1"}
//{"key":"1"}
//{"speed":"200"}
//{"color":"1"}
//{"buzzer":"1"}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      uploaddata("mode", mode);
      delay(30);
      uploaddata("speed", speed);
      delay(30);
      uploaddata("color", color);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *modeval = aJson.getObjectItem(msg, "mode");
  if (modeval) {
    modeS  = modeval->valuestring;
    mode = modeS.toInt();
    write_eeprom();
    soundmode(mode);
  }
  else {
#if DEBUG
    Serial.println("no mode data");
#endif
  }
  aJsonObject *keyval = aJson.getObjectItem(msg, "key");
  if (keyval) {
    keyS  = keyval->valuestring;
    key = keyS.toInt();
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no key data");
#endif
  }
  aJsonObject *speedval = aJson.getObjectItem(msg, "speed");
  if (speedval) {
    speedS  = speedval->valuestring;
    speed = speedS.toInt();
    setAllLed(COLOR_GREEN);
    delay(100);
    setAllLed(COLOR_NONE);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no speed data");
#endif
  }
  aJsonObject *colorval = aJson.getObjectItem(msg, "color");
  if (colorval) {
    colorS  = colorval->valuestring;
    color = colorS.toInt();
    setAllLed(COLOR_GREEN);
    delay(100);
    setAllLed(COLOR_NONE);
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no color data");
#endif
  }
  aJsonObject *buzzerval = aJson.getObjectItem(msg, "buzzer");
  if (buzzerval) {
    buzzerS = buzzerval->valuestring;
  }
  else {
#if DEBUG
    Serial.println("no buzzer data");
#endif
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
}

aJsonObject *createMessage(uint8_t y)
{
  aJsonObject *msg = aJson.createObject();
  aJsonObject *analog = aJson.createItem(y);
  aJson.addItemToObject(msg, "Time", analog);
  return msg;
}

void Carblejson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

void timeupload(uint8_t y)
{
  aJsonObject *msg = createMessage(y);
  aJson.print(msg, &serial_stream);
  Serial.println();
  aJson.deleteItem(msg);
}
