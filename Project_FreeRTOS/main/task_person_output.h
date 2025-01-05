#ifndef TASK_PERSON_OUTPUT_H
#define TASK_PERSON_OUTPUT_H

#include "platform.h"

enum PirStatus
{
  LED_PIR_DISABLE_STS,
  LED_PIR_HIGH_STS,
  LED_PIR_UNKNOWN_STS
};

// Hàm điều khiển Led mức cảnh báo cao
void LedAlertHigh()
{
  Serial.println("Led: High alert!");
  for (int i = 0; i < 4; i++) 
  {
    analogWrite(Led_pin, 200); // Tín hiệu PWM (100% duty cycle)
    delay(250);                  
    //digitalWrite(Led_pin, 0);  // Tắt tín hiệu
    //delay(100);                  // Nghỉ 100ms
  }
}

void task_PERSON_output(void *pvParameters)
{ 
 PirStatus Led_sts = LED_PIR_UNKNOWN_STS;
  for(;;)
  {
     vTaskDelay(20 / portTICK_PERIOD_MS);
    Message pir_receive {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};
    if(xQueueReceive(Person_Queue, &pir_receive, portMAX_DELAY) == pdTRUE)
    { 
      if(pir_receive.id_Rx == OUT_LED)
      {
        if(pir_receive.id_Tx == IN_PIR)
        {
          switch(pir_receive.payload)
          {
            case LED_PIR_DISABLE:
              if(Led_sts != LED_PIR_DISABLE_STS)
              {
                Serial.println("Led: Disabled");
                digitalWrite(Led_pin, 0); // Đảm bảo led tắt
                Led_sts = LED_PIR_DISABLE_STS;
              }
              break;

            case LED_PIR_ENABLE_HIGH:
                Serial.println("Led: HIGH");
                LedAlertHigh(); // Gọi hàm cảnh báo mức cao
                Led_sts = LED_PIR_HIGH_STS;
              break;

            default:
              Serial.println("Unknown Led state");
              digitalWrite(Led_pin, 0); // Tắt led khi trạng thái không xác định
             Led_sts = LED_PIR_UNKNOWN_STS;
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