// khoi_Task.h
#ifndef TASK_KHOI_OUTPUT_H
#define TASK_KHOI_OUTPUT_H

#include "platform.h" 

void task_KHOI_output(void *pvParameters)
{
  int flag_pri = 0;
  int pre_flag_pri = 0;
  for(;;)
  {
    Message mq_receive = {0};
    if(xQueueReceive(Khoi_Queue, &mq_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(mq_receive.id_Rx == OUT_FAN)
      {
        if(mq_receive.id_Tx == IN_MQ_135)
        flag_pri = 1;
        {
          if(pre_flag_pri < flag_pri)
          {
            pre_flag_pri = flag_pri;
            switch (mq_receive.payload)
            {
              case FAN_MQ_DISABLE:
              //call function turn off fan
                break;
              default:
                //code
            }
            if(FAN_MQ_DISABLE == mq_receive.payload)
            {
              //call function turn off fan
            }
          }
        }
        if(mq_receive.id_Tx == IN_PIR)
        {
          switch (mq_receive.payload)
          {
            case FAN_MQ_DISABLE:
              //call function turn off fan
              break;

            default:
          }
          if(FAN_MQ_DISABLE == mq_receive.payload)
          {
            //call function turn off fan
          }
      }
    }
    else
    {
      //còn không thì thực hiện......
    }
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }  
}
#endif