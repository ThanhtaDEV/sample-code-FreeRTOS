// khoi_Task.h
#ifndef TASK_KHOI_OUTPUT_H
#define TASK_KHOI_OUTPUT_H

#include "platform.h" 

void task_KHOI_output(void *pvParameters)
{
  for(;;)
  {
    Message mq_receive = {0};
    if(xQueueReceive(Platform_Queue, &mq_receive, portMAX_DELAY) == pdTRUE)
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