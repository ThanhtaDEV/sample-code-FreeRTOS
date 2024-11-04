// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

void task_RUNG(void *pvParameters)
{
  for(;;)
  {
   Message sw_send = {0};
      sw_send.id_Tx = OUT_BUZZER;
      sw_send.id_Rx = IN_SW_1801P;
      sw_send.payload = BUZZER_SW_DISABLE;
      xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
  }
}  
#endif