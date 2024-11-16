// rung_Task.h
#ifndef TASK_RUNG_OUTPUT_H
#define TASK_RUNG_OUTPUT_H

#include "platform.h" 

enum BuzzerStatus
{
  BUZZER_DISABLE_STS,
  BUZZER_LOW_STS,
  BUZZER_HIGH_STS,
  BUZZER_UNKNOWN_STS
};

// Hàm điều khiển buzzer mức cảnh báo thấp
void buzzerAlertLow()
{
  Serial.println("Buzzer: Low alert!");
  for (int i = 0; i < 2; i++) // Rung 2 lần
  {
    analogWrite(buzzer_pin, 200); // Tín hiệu PWM (50% duty cycle)
    delay(200);                  // Rung trong 200ms
    analogWrite(buzzer_pin, 0);  // Tắt tín hiệu
    delay(800);                  // Nghỉ 800ms
  }
}

// Hàm điều khiển buzzer mức cảnh báo cao
void buzzerAlertHigh()
{
  Serial.println("Buzzer: High alert!");
  for (int i = 0; i < 8; i++) // Rung 8 lần
  {
    analogWrite(buzzer_pin, 200); // Tín hiệu PWM (100% duty cycle)
    delay(200);                  // Rung trong 200ms
    analogWrite(buzzer_pin, 0);  // Tắt tín hiệu
    delay(100);                  // Nghỉ 100ms
  }
}

void task_RUNG_output()
{ 
  BuzzerStatus buzzer_sts = BUZZER_UNKNOWN_STS;
  for(;;)
  {
    delay(100);
    Message sw_receive = {0};
    if(xQueueReceive(Rung_Queue, &sw_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(sw_receive.id_Rx == OUT_BUZZER)
      {
        if(sw_receive.id_Tx == IN_SW_1801P)
        {
          switch(sw_receive.payload)
          {
            case BUZZER_SW_DISABLE:
              if(buzzer_sts != BUZZER_DISABLE_STS)
              {
                Serial.println("Buzzer: Disabled");
                analogWrite(buzzer_pin, 0); // Đảm bảo buzzer tắt
                buzzer_sts = BUZZER_DISABLE_STS;
              }
              break;

            case BUZZER_SW_ENABLE_LOW:
              // if(buzzer_sts != BUZZER_LOW_STS)
              // {
                Serial.println("Buzzer: LOW");
                buzzerAlertLow(); // Gọi hàm cảnh báo mức thấp
                buzzer_sts = BUZZER_LOW_STS;
              // }
              break;

            case BUZZER_SW_ENABLE_HIGH:
              // if(buzzer_sts != BUZZER_HIGH_STS)
              // {
                Serial.println("Buzzer: HIGH");
                buzzerAlertHigh(); // Gọi hàm cảnh báo mức cao
                buzzer_sts = BUZZER_HIGH_STS;
              // }
              break;

            default:
              Serial.println("Unknown buzzer state");
              analogWrite(buzzer_pin, 0); // Tắt buzzer khi trạng thái không xác định
              buzzer_sts = BUZZER_UNKNOWN_STS;
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
