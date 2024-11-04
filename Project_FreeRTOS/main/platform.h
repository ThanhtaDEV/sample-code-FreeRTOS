#ifndef PLATFORM_H
#define PLATFORM_H

#include <Arduino_FreeRTOS.h>
#include <queue.h>

enum ModuleID
{
  //input Rx
  IN_SW_1801P = 0x01 ,   //... Khai báo cảm biến lên chân digital của Arduino
  IN_MQ_135 = 0x35 ,     //... Khai báo cảm biến lên chân analog của Arduino
  IN_PIR = 0x36 ,        //... Khai báo cảm biến lên chân digital của Arduino
  
  //output Tx
  OUT_BUZZER = 0x37 ,    //... khai báo còi vào chân digital trên arduino
  OUT_FAN = 0x38 ,       //... khai báo quạt vào chân digital trên arduino
  OUT_LED = 0x39         //... khai báo led vào chân digital trên arduino
};

enum ActionPayload 
{ 
  //Buzzer output
  BUZZER_SW_DISABLE,
  BUZZER_SW_ENABLE,

  //Fan output
  FAN_MQ_DISABLE,
  FAN_MQ_ENABLE,
  
  //Led output
  LED_PIR_DISABLE,
  LED_PIR_ENABLE
};

struct Message 
{
  ModuleID id_Tx;
  ModuleID id_Rx;
  ActionPayload payload;
};

extern QueueHandle_t Platform_Queue;  // Khai báo queue để sử dụng trong các file khác

#endif  
