#include <Microduino_Motor.h>

#define motor_pin0A 6
#define motor_pin0B 8
#define motor_pin1A 5
#define motor_pin1B 7

#define CMD_STOP   -1
#define CMD_UP     1
#define CMD_DOWN   2
#define CMD_LEFT   3
#define CMD_RIGHT  4

#define RIGHT  1
#define LEFT   0

Motor MotorLeft(motor_pin0A, motor_pin0B);
Motor MotorRight(motor_pin1A, motor_pin1B);

bool leftFlag = false;
bool rightFlag = false;
bool back, maze, maze_c;
bool stopone;
int16_t leftSpeed = 0;
int16_t rightSpeed = 0;
uint32_t mazeTimer;

bool fback = true;

void motorUpdate(int16_t speed1, int16_t speed2)
{
  MotorLeft.Driver(speed1);
  MotorRight.Driver(speed2);
}

void motorCoast()
{
  leftSpeed = 0;
  rightSpeed = 0;
}

void bleControl(int8_t bleCmd)
{
  switch (bleCmd)
  {
    case CMD_UP:
      setAllLed(color);
      leftSpeed = speed;
      rightSpeed = speed;
      fback = true;
      break;
    case CMD_DOWN:
      setAllLed(COLOR_RED);
      leftSpeed = - speed;
      rightSpeed = - speed;
      fback = false;
      break;
    case CMD_LEFT:
      if (fback)
      {
        leftSpeed = speed - 1.5 * STEER;
        rightSpeed = speed + 1.5 * STEER;
        setLed(COLOR_NONE, RIGHT);
      }
      else
      {
        leftSpeed = -speed + STEER;
        rightSpeed = -speed - STEER;
        setLed(COLOR_NONE, RIGHT);
      }
      ledBlink(500, COLOR_RED, LEFT);
      break;
    case CMD_RIGHT:
      if (fback)
      {
        leftSpeed = speed + 1.5 * STEER;
        rightSpeed = speed - 1.5 * STEER;
        setLed(COLOR_NONE, LEFT);
      }
      else
      {
        leftSpeed = -speed - STEER;
        rightSpeed = -speed + STEER;
        setLed(COLOR_NONE, LEFT);
      }
      ledBlink(500, COLOR_RED, RIGHT);
      break;
    case CMD_STOP:
      if (leftSpeed != 0 || rightSpeed != 0)
      {
        setAllLed(COLOR_NONE);
        leftSpeed = 0;
        rightSpeed = 0;
      }
      break;
    default:
      break;
  }
}

void trackControl(uint16_t trackVal1, uint16_t trackVal2)
{
  if (trackVal1 < TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = TRACK_SPEED;
    rightSpeed = TRACK_SPEED;
    setAllLed(COLOR_COLD);
  }
  else if (trackVal1 < TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    leftSpeed = TRACK_SPEED + TRACK_STEER;
    rightSpeed = TRACK_SPEED - TRACK_STEER;
    setLed(COLOR_WARM, LEFT);
    setLed(COLOR_NONE, RIGHT);
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = TRACK_SPEED - TRACK_STEER;
    rightSpeed = TRACK_SPEED + TRACK_STEER;
    setLed(COLOR_WARM, RIGHT);
    setLed(COLOR_NONE, LEFT);
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    setAllLed(COLOR_NONE);
    leftSpeed = 0;
    rightSpeed = 0;
  }
}

void mazeControl(uint16_t trackVal1, uint16_t trackVal2)
{
  bleControl(key);
  
  if (key > 0 && !maze)
  {
    maze = true;
    mazeTimer = millis();
  }

  if (trackVal1 < TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    if (stopone)
    {
      key = CMD_STOP;
      stopone = false;
    }
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD || (trackVal1 < TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD))
  {
    stopone = true;
    tone(PIN_BUZZER, 600);
    setAllLed(COLOR_RED);
    key = CMD_DOWN;
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD && maze)
  {
    setAllLed(COLOR_GREEN);
    motorUpdate(0, 0);
    timeupload((millis() - mazeTimer) / 1000);
    key = CMD_STOP;
    maze = false;
    stopone = false;
  }
}
