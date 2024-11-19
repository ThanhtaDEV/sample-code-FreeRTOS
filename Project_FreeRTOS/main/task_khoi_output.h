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
  Serial.println("Fan: Medium alert!");
  for (int i = 0; i < 2; i++) // Rung 2 lần
  {
    digitalWrite(Fan_pin, 200); // Tín hiệu PWM (50% duty cycle)
    delay(200);                  // Quay trong 200ms
    analogWrite(Fan_pin, 0);  // Tắt tín hiệu
    delay(800);                  // Nghỉ 800ms
  }
}

// Hàm điều khiển buzzer mức cảnh báo cao
void FanAlertHigh()
{
  Serial.println("Fan: High alert!");
  for (int i = 0; i < 8; i++) // Quay 8 lần
  {
    analogWrite(Fan_pin, 200); // Tín hiệu PWM (100% duty cycle)
    delay(200);                  // Quay trong 200ms
    digitalWrite(Fan_pin, 0);  // Tắt tín hiệu
    delay(100);                  // Nghỉ 100ms
  }
}

void task_KHOI_output()
{ 
 FanStatus Fan_sts = FAN_MQ_UNKNOWN_STS;
  for(;;)
  {
   
    Message mq_receive = {0};
    if(xQueueReceive(Khoi_Queue, &mq_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(mq_receive.id_Rx == OUT_FAN)
      {
        if(mq_receive.id_Tx == IN_MQ_135)
        {
          switch(mq_receive.payload)
          {
            case  FAN_MQ_DISABLE:
              if(Fan_sts != FAN_MQ_DISABLE_STS)
              {
                Serial.println("Fan: Disabled");
                digitalWrite(Fan_pin, 0); // Đảm bảo buzzer tắt
                Fan_sts = FAN_MQ_DISABLE_STS;
              }
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