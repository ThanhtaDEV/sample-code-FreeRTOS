// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

int time_interval = 1000;
int threshold_low = 5;
int threshold_medium = 15;

void task_RUNG(void *pvParameters)
{
  for(;;)
  {
    int count = 0;
    unsigned long start_time = millis(); //Thời điểm bắt đầu đo
    bool signal_detect = false;
    while (millis() - start_time < time_interval )
    { 
      if(digitalRead(IN_SW_1801P) == HIGH)
      {
        count++;
        delay(10); //tránh đếm trùng tín hiệu
      }
    }
    // thông điệm gửi đi theo ngưỡng tần suất
    Message sw_send = {0};
      sw_send.id_Tx = IN_SW_1801P;
      sw_send.id_Rx = OUT_BUZZER;
      sw_send.payload = BUZZER_SW_DISABLE;
      if(xQueueSend(Rung_Queue, &sw_send, portMAX_DELAY) == pdPASS) //trường hợp nhận được 3 ngưỡng hoạt động
      { 
        if(count <= threshold_low)  // count <= 5
        {
          sw_send.payload = BUZZER_SW_DISABLE;  //rung mức thấp, không cảnh báo
          Serial.print("Giá trị rung mức thấp: ");
          Serial.print(IN_SW_1801P);
          Serial.println(count);
        }
        else if(count > threshold_low && count <= threshold_medium  ) 
        {
          sw_send.payload = BUZZER_SW_ENABLE_MEDIUM; //rung vừa, cảnh báo vừa
          Serial.print("Giá trị rung mức vừa: ");
          Serial.print(IN_SW_1801P);
          Serial.println(count);
        }
        else
        {
          sw_send.payload = BUZZER_SW_ENABLE_HIGH; //rung mạnh, cảnh báo lớn
          Serial.print("Giá trị rung mức cao: ");
          Serial.print(IN_SW_1801P);
          Serial.println(count);
        }
      }
      else //trường hợp cảm biến bị ngắt kết nối với Arduino
      {
        while (millis() - start_time < time_interval )
        {
          if(digitalRead(IN_SW_1801P) == LOW)
          {
            signal_detect = true;
            break;
          }
        }
        if(signal_detect)
        {
          Serial.println("Cảm biến đang hoạt động bình thường.");
        }
        else
        {
          Serial.println("Cảnh báo: Cảm biến có thể đã bị ngắt kết nối!");
        }
        delay(1000);
      }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}  
#endif