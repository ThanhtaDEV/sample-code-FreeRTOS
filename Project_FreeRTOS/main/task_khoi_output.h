// khoi_Task.h
#ifndef TASK_KHOI_OUTPUT_H
#define TASK_KHOI_OUTPUT_H

#include "platform.h" 
enum FanStatus
{
  FAN_MQ_DISABLE_STS,
  FAN_MQ_LOW_STS,
  FAN_MQ_HIGH_STS,
  FAN_MQ_UNKNOWN_STS
};

void FanAlertLow() 
{
  Serial.println("Fan: Medium alert with decreasing speed!");

  int initialDelay = 20;  // Thời gian delay ban đầu ngắn hơn
  int increment = 60;     // Tăng delay nhanh hơn
  int maxDelay = 200;     // Giới hạn delay ngắn hơn

  for (int delayTime = initialDelay; delayTime <= maxDelay; delayTime += increment) {
    digitalWrite(Fan_pin, LOW); // Kích hoạt relay (quạt chạy)
    delay(delayTime);           // Thời gian bật relay
    digitalWrite(Fan_pin, HIGH); // Tắt relay
    delay(delayTime / 3);        // Thời gian nghỉ cực ngắn
  }
}

void FanAlertHigh() 
{
  Serial.println("Fan: High alert with decreasing speed!");

  int initialDelay = 30;  // Thời gian delay ban đầu ngắn hơn
  int increment = 90;     // Tăng delay nhanh hơn
  int maxDelay = 500;     // Giới hạn delay ngắn hơn

  for (int delayTime = initialDelay; delayTime <= maxDelay; delayTime += increment) {
    digitalWrite(Fan_pin, LOW); // Kích hoạt relay (quạt chạy)
    delay(delayTime);           // Thời gian bật relay
    digitalWrite(Fan_pin, HIGH); // Tắt relay
    delay(delayTime / 3);        // Thời gian nghỉ cực ngắn
  }
}



void FanOff()
{
  Serial.println("Fan: Off"); // In thông báo ra Serial Monitor
  digitalWrite(Fan_pin, HIGH); // relay kích mức thấp nên để high để tắt quạt, nếu relay kích mức cao thì đổi thành LOW
}
void task_KHOI_output(void *pvParameters)
{ 
 FanStatus Fan_sts = FAN_MQ_UNKNOWN_STS;
  for(;;)
  {
   
    Message mq_receive {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};
    if(xQueueReceive(Khoi_Queue, &mq_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(mq_receive.id_Rx == OUT_FAN)
      {
        if(mq_receive.id_Tx == IN_MQ_135)
        {
          switch(mq_receive.payload)
          {
            case  FAN_MQ_DISABLE:
                Serial.println("Fan: Disabled");
                FanOff(); 
                //digitalWrite(Fan_pin, 0);
                Fan_sts = FAN_MQ_DISABLE_STS;
              break;

            case FAN_MQ_ENABLE_MEDIUM:
              // if(buzzer_sts != BUZZER_LOW_STS)
              // {
                Serial.println("Fan: MEDIUM");
                FanAlertLow(); // Gọi hàm cảnh báo mức thấp
                Fan_sts = FAN_MQ_LOW_STS;
              // }
              break;

            case FAN_MQ_ENABLE_HIGH:
              // if(buzzer_sts != BUZZER_HIGH_STS)
              // {
                Serial.println("Fan: HIGH");
                FanAlertHigh(); // Gọi hàm cảnh báo mức cao
                Fan_sts = FAN_MQ_HIGH_STS;
              // }
              break;

            default:
              Serial.println("Unknown Fan state");
              digitalWrite(Fan_pin, 0); // Tắt fan khi trạng thái không xác định
              Fan_sts = FAN_MQ_UNKNOWN_STS;
              break;
          }
        }
      }
    }
    else
    {
      // Thực hiện hành động khác nếu không có message trong hàng đợi
    }
  }   
}

#endif

