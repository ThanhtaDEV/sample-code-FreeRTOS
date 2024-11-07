#ifndef TASK_PERSON_OUTPUT_H
#define TASK_PERSON_OUTPUT_H

#include "platform.h"

void task_PERSON_output()
{
  for(;;)
  {
    Message pir_receive = {0};
    if(xQueueReceive(Person_Queue, &pir_receive, portMAX_DELAY) == pdTRUE)
    { 
      // nhận được từ input đã gửi ở trên thì thực hiện.....
    }
    else
    {
      //còn không thì thực hiện......
    }
  }
}
#endif
