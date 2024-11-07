#ifndef TASK_PERSON_H
#define TASK_PERSON_H


#include "platform.h"

void task_PERSON(void *pvParameters)
{
  for(;;)
  {
    Message pir_send = {0};
      pir_send.id_Tx = OUT_LED;
      pir_send.id_Rx = IN_PIR;
      pir_send.payload = LED_PIR_DISABLE;
      xQueueSend(Person_Queue, &pir_send, portMAX_DELAY);
      if(xQueueSend(Person_Queue, &pir_send, portMAX_DELAY) == pdPASS)
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