// rung_Task.h
#ifndef TASK_RUNG_OUTPUT_H
#define TASK_RUNG_OUTPUT_H

#include "platform.h" 

enum BuzzerMode
{
  BUZZER_MODE_DISABLE,
  BUZZER_MODE_LOW,
  BUZZER_MODE_HIGH
};

// Biến toàn cục lưu trạng thái hiện tại của buzzer
volatile BuzzerMode currentBuzzerMode = BUZZER_MODE_DISABLE;

void buzzerTask(void *pvParameters) 
{
  for (;;) 
  {
    switch (currentBuzzerMode) 
    {
      case BUZZER_MODE_DISABLE:
        analogWrite(buzzer_pin, 0); // Tắt buzzer
        vTaskDelay(100 / portTICK_PERIOD_MS); // Nghỉ để tránh lặp không cần thiết
        break;

      case BUZZER_MODE_LOW:
        analogWrite(buzzer_pin, 200); // PWM 50% duty cycle
        vTaskDelay(100 / portTICK_PERIOD_MS); // Bật trong 100ms
        analogWrite(buzzer_pin, 0); // Tắt
        vTaskDelay(900 / portTICK_PERIOD_MS); // Nghỉ 900ms
        break;

      case BUZZER_MODE_HIGH:
        analogWrite(buzzer_pin, 250); // PWM 100% duty cycle
        vTaskDelay(100 / portTICK_PERIOD_MS); // Bật trong 100ms
        analogWrite(buzzer_pin, 0); // Tắt
        vTaskDelay(50 / portTICK_PERIOD_MS); // Nghỉ 50ms
        break;

      default:
        // Nếu trạng thái không hợp lệ, tắt buzzer
        analogWrite(buzzer_pin, 0);
        break;
    }
  }
}

void task_RUNG_output(void *pvParameters)
{
  for (;;)
  {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Message sw_receive {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};

    if (xQueueReceive(Rung_Queue, &sw_receive, portMAX_DELAY) == pdTRUE)
    {
      if (sw_receive.id_Rx == OUT_BUZZER && sw_receive.id_Tx == IN_SW_1801P)
      {
        switch (sw_receive.payload)
        {
          case BUZZER_SW_DISABLE:
            if (currentBuzzerMode != BUZZER_MODE_DISABLE)
            {
              Serial.println("Disabling Buzzer...");
              currentBuzzerMode = BUZZER_MODE_DISABLE;
            }
            break;

          case BUZZER_SW_ENABLE_LOW:
            if (currentBuzzerMode != BUZZER_MODE_LOW)
            {
              Serial.println("Enabling Buzzer Low...");
              currentBuzzerMode = BUZZER_MODE_LOW;
            }
            break;

          case BUZZER_SW_ENABLE_HIGH:
            if (currentBuzzerMode != BUZZER_MODE_HIGH)
            {
              Serial.println("Enabling Buzzer High...");
              currentBuzzerMode = BUZZER_MODE_HIGH;
            }
            break;

          default:
            Serial.println("Unknown Buzzer Command.");
            break;
        }
      }
    }
  }
}

#endif
