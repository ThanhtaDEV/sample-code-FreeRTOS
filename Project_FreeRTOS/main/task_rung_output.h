// rung_Task.h
#ifndef TASK_RUNG_OUTPUT_H
#define TASK_RUNG_OUTPUT_H

#include "platform.h" 

void task_RUNG_output()
{ 
  for(;;)
  {
     Message sw_receive = {0};
    if(xQueueReceive(Platform_Queue, &sw_receive, portMAX_DELAY) == pdTRUE)
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
