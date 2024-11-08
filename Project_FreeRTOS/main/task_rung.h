// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

void task_RUNG(void *pvParameters)
{
    int rung_noiseThreshold = 50; // cập nhật giá trị
    int rung_lastValue = 0;
    int rung_count = 0;
  for(;;)
  {
    int rung_currentvalue = analogRead(IN_SW_1801P);
    Serial.print("Rung Value: ");
    Serial.println(rung_currentvalue); 
   Message sw_send = {0};
      sw_send.id_Tx = IN_SW_1801P;
      sw_send.id_Rx = OUT_BUZZER;
      sw_send.payload = BUZZER_SW_DISABLE;
      if(xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
        if(abs(rung_currentvalue-rung_lastValue)>=rung_noiseThreshold)
        {
          rung_count++;
          // ngưỡng báo động
          if(rung_currentvalue>=80 && rung_count>=3 )// ngưỡng giá trị báo động 
          {
            sw_send.payload = BUZZER_SW_ENABLE;
            Serial.print("Buzzer on");
            rung_lastValue=rung_currentvalue;
            rung_count=0;
          }
          else 
          {
            rung_count=0;
            if(rung_currentvalue>=80)
            {
              Serial.println("Warning");
              sw_send.payload = BUZZER_SW_ENABLE;
              Serial.print("Buzzer on");
              rung_lastValue=rung_currentvalue;
            }
            else
            {
              rung_lastValue=rung_currentvalue;
              sw_send.payload = BUZZER_SW_DISABLE;
              Serial.print("Buzzer off");
            }
          }  
        }
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
      // kiểm tra cảm biến
  }
}  
#endif