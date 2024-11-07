// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

void task_RUNG(void *pvParameters)
{
    const int rung_noiseThreshold = ....; // cập nhật giá trị
    int rung_unchangedCount=0;
    int rung_lastValue1 = 0;
    int rung_lastValue2 = 0;
    int rung_count=0;
    unsigned long rung_lastSignalTime = 0;
  for(;;)
  {
    unsigned long rung_currentTime = millis(); // đọc giá trị thời gian hiện tại


    int rung_value = analogRead(IN_SW_1801P);
    Serial.print("Rung Value: ");
    Serial.println(rung_value); 
   Message sw_send = {0};
      sw_send.id_Tx = OUT_BUZZER;
      sw_send.id_Rx = IN_SW_1801P;
      sw_send.payload = BUZZER_SW_DISABLE;
      xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY);
           if(xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
        if(abs(rung_value-rung_lastValue1)>=rung_noiseThreshold)
        {
          rung_count++;
          // ngưỡng báo động
          if(rung_value > ... && rung_count>=... )// ngưỡng giá trị báo động 
          {
            sw_send.payload = BUZZER_SW_ENABLE;
            Serial.print("Buzzer on");
            rung_lastValue1=rung_value;

          }
          else 
          {
          sw_send.payload = BUZZER_SW_DISABLE;
          Serial.print("Buzzer off");
          }
          
        }
        else
        {

          if(rung_value>....)
          {
            sw_send.payload = BUZZER_SW_ENABLE;
            Serial.print("Buzzer on");
          }
          rung_count=0;
          rung_lastValue1=rung_value;
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