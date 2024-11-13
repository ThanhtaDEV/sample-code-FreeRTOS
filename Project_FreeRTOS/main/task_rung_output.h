// rung_Task.h
#ifndef TASK_RUNG_OUTPUT_H
#define TASK_RUNG_OUTPUT_H

#include "platform.h" 

void task_RUNG_output()
{ 
  for(;;)
  {
     Message sw_receive = {0};
    if(xQueueReceive(Rung_Queue, &sw_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(sw_receive.id_Rx ==  OUT_BUZZER)
      {
        if(sw_receive.id_Tx == IN_SW_1801P)
        {
          switch(sw_receive.payload)
          {
            case BUZZER_SW_DISABLE:
              ///////////////////
              break;
            case BUZZER_SW_ENABLE_MEDIUM:
              ////////////////////
              break;
            case BUZZER_SW_ENABLE_HIGH :
              ////////////////////
              break;
            default:
            //////////////////////
              break;
          }
        }
      }
    }
    else
    {
      //còn không thì thực hiện......
    }
  }   
}
#endif
