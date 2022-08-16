#include <Arduino.h>

void Key_Enter_push()
{
  dacWrite(26,100);
  Serial.print("Key enter");
  Serial.println(millis());
}
void Key_up_push()
{
  dacWrite(25,200);
  Serial.println("Key up");
  Serial.println(millis());
}
void Key_down_push()
{
  dacWrite(26,255);
  Serial.println("Key down");
  Serial.println(millis());
}
void Key_left_push()
{
  Serial.println("Key left");
  Serial.println(millis());
}
void Key_right_push()
{
  Serial.println("Key right");
  Serial.println(millis());
}
void Key_x_release()
{
  Serial.println("Key release");
  Serial.println(millis());
}
typedef enum{
  KEY_ENTER= 1,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_DELAY //for delay
}_Cmd_Type;
typedef enum{
  RUN_AUTO,
  RUN_MANUAL
}_Run_Type;
typedef struct{
    uint8_t Cmd;
    uint16_t PressTime;
    uint16_t DelayTime;
}__command;

typedef enum{
  CMD_KEY_START,
  CMD_KEY_WAITING,
  CMD_KEY_RELEASE,
  CMD_KEY_DELAY,
  CMD_KEY_IDLE

}_Cmd_State;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Code started..");
  Serial.setTimeout(1);
}
int RunType = RUN_MANUAL;
uint8_t RecvData[4096];
uint16_t RecvCnt =0;
uint32_t RecvCnt_time;

__command Command;
_Cmd_State CmdState= CMD_KEY_IDLE;
uint32_t Cmd_time = 0;
void loop() {
  if(Serial.available())
  {
    RecvCnt_time = millis() +20 ;// time out 5ms
    int tot = Serial.readBytes(RecvData+RecvCnt,Serial.available());
    RecvCnt+= tot;
    if(RecvCnt >= 2000)
      RecvCnt =0;
  }
  if((millis() > RecvCnt_time) && (RecvCnt)) //process buffer 
  {
    
    uint8_t checksum = 0;
    for(int i =0;i<RecvCnt-1;i++)
    {
      checksum+= RecvData[i];
    }
    if(checksum == RecvData[RecvCnt-1])
    {
      if(RecvCnt == 6) //manual
      {
        Command.Cmd=RecvData[0];
        memcpy(&Command.PressTime,&RecvData[1],4);
        Serial.print("Command.PressTime: ");
        Serial.println(Command.PressTime);
        Serial.print("Command.DelayTime: ");
        Serial.println(Command.DelayTime);
        RunType = RUN_MANUAL;
        CmdState = CMD_KEY_START;
      }
    }
    else{
      Serial.println("checksum fail");
    }
    memset(RecvData,0,1024);
    RecvCnt =0;

  }
  switch (RunType)
  {
    case RUN_AUTO:
      switch (CmdState)
      {
      case CMD_KEY_START:
        switch (Command.Cmd)
        {
          case KEY_ENTER:
            Key_Enter_push();
            break;
          case KEY_UP:
            Key_up_push();
            break;
          case KEY_DOWN:
            Key_down_push();
            break;
          case KEY_LEFT:
            Key_left_push();
            break;
          case KEY_RIGHT:
            Key_right_push();
            break;
          default:
            break;
        }
        CmdState = CMD_KEY_RELEASE;
        Cmd_time = millis() + Command.PressTime;
        break;
      case CMD_KEY_RELEASE:
        if(millis() > Cmd_time)
        {
          CmdState = CMD_KEY_IDLE;
          Key_x_release();
        }
        break;
      case CMD_KEY_DELAY:
        /* code */
        //no need delay when run manual 
        break;
      
      default:
        break;
      }
      /* code */
      break;
    case RUN_MANUAL:
      switch (CmdState)
      {
      case CMD_KEY_START:
        switch (Command.Cmd)
        {
          case KEY_ENTER:
            Key_Enter_push();
            break;
          case KEY_UP:
            Key_up_push();
            break;
          case KEY_DOWN:
            Key_down_push();
            break;
          case KEY_LEFT:
            Key_left_push();
            break;
          case KEY_RIGHT:
            Key_right_push();
            break;
          default:
            break;
        }
        CmdState = CMD_KEY_RELEASE;
        Cmd_time = millis() + Command.PressTime;
        break;
      case CMD_KEY_RELEASE:
        if(millis() > Cmd_time)
        {
          CmdState = CMD_KEY_IDLE;
          Key_x_release();
        }
        break;
      case CMD_KEY_DELAY:
        /* code */
        //no need delay when run manual 
        break;
      
      default:
        break;
      }
      /* code */
      break;
    default:
      break;
  }
}