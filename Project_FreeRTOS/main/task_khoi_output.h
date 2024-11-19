#ifndef TASK_KHOI_OUTPUT_H
#define TASK_KHOI_OUTPUT_H

#include "platform.h"
#define FAN_PIN 3
#define ERROR_LED_PIN 13  // Đèn LED báo lỗi nếu cần
pinMode(3, OUTPUT);
pinMode(13, OUTPUT);
// Hàm bật quạt với công suất 100%
void turnOnFan100() 
{
  analogWrite(FAN_PIN, 255);
}

// Hàm bật quạt với công suất 50%
void turnOnFan50() 
{
  analogWrite(FAN_PIN, 128); 
}

// Hàm tắt quạt
void turnOffFan() 
{
  analogWrite(FAN_PIN, 0); // Tắt quạt hoàn toàn
}

// Hàm kiểm tra kết nối quạt
bool isFanConnected() 
{
  // Kiểm tra kết nối - giả sử nếu tín hiệu là LOW thì quạt mất kết nối
  return digitalRead(FAN_PIN) != LOW; 
}

void task_KHOI_output() 
{
  for (;;) 
  {
    Message mq_receive = {0};
    if (xQueueReceive(Khoi_Queue, &mq_receive, portMAX_DELAY) == pdTRUE) 
    {
      if (mq_receive.id_Rx == OUT_FAN) 
      {
        if (mq_receive.id_Tx == IN_MQ_135)
        {
          // Kiểm tra kết nối trước khi điều khiển quạt
          if (isFanConnected())
          {
            if (mq_receive.payload == FAN_MQ_ENABLE_LEVEL1)
            {
              Serial.println("Fan turned ON 50%");
              turnOnFan50();
            }
            else if (mq_receive.payload == FAN_MQ_ENABLE_LEVEL2) 
            {
              Serial.println("Fan turned ON 100%");
              turnOnFan100();
            }
            else if (mq_receive.payload == FAN_MQ_DISABLE) 
            {
              Serial.println("Fan turned off");
              turnOffFan();
            }
          }
          else 
          {
            Serial.println("Fan connection lost");
            digitalWrite(ERROR_LED_PIN, HIGH); // Bật LED báo lỗi
            turnOffFan(); // Tắt quạt để đảm bảo an toàn
          }
        }
      }
    } else
    {

    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }  
}

#endif