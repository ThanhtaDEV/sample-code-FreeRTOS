// khoi_Task.h
#ifndef TASK_KHOI_H
#define TASK_KHOI_H

#include "platform.h" 

void task_KHOI(void *pvParameters)
{
  for(;;)
  {
    Message mq_send = {0};
      mq_send.id_Tx = OUT_FAN;
      mq_send.id_Rx = IN_MQ_135;
      mq_send.payload = FAN_MQ_DISABLE;
      xQueueSend(Platform_Queue, &mq_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &mq_send, portMAX_DELAY) == pdPASS)
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