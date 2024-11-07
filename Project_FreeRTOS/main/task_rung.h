// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

void task_RUNG(void *pvParameters)
{
    const int khoi_noiseThreshold = ....; // cập nhật giá trị
    int khoi_unchangedCount=0;
    int khoi_lastValue1 = 0;
    int khoi_lastValue2 = 0;
    int khoi_count=0;
    unsigned long khoi_lastSignalTime = 0;
  for(;;)
  {
    unsigned long khoi_currentTime = millis(); // đọc giá trị thời gian hiện tại
    int khoi_value = analogRead(IN_MQ_135);

   Message sw_send = {0};
      sw_send.id_Tx = OUT_BUZZER;
      sw_send.id_Rx = IN_SW_1801P;
      sw_send.payload = BUZZER_SW_DISABLE;
      xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &mq_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
        if(abs(khoi_value-khoi_lastValue1)>=khoi_noiseThreshold)
        {
          khoi_count++;
          // ngưỡng báo động
          if(khoi_value > ... && khoi_count>=... )// ngưỡng giá trị báo động 
          {
            sw_send.payload = FAN_MQ_ENABLE;
            Serial.print("Buzzer on");
            if(khoi_value>...)
            // gửi tới loa cảnh báo......z
            khoi_lastValue1=khoi_value;

          }
          else 
          {
          sw_send.payload = FAN_MQ_DISABLE;
          Serial.print("Buzzer off");
          }
          
        }
        else
        {
          khoi_count=0;
          khoi_lastValue1=khoi_value;
        }
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
  }
}  
#endif