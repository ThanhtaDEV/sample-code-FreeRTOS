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

// Handle cho task con
TaskHandle_t buzzerTaskHandle = NULL;

// Hàm điều khiển buzzer mức cảnh báo thấp
void buzzerAlertLow(void *pvParameters)
{
  Serial.println("Buzzer Low Alert Task Started.");
  for(;;)
  {
    // Thực hiện cảnh báo mức thấp
    analogWrite(buzzer_pin, 200); // Tín hiệu PWM (50% duty cycle)
    vTaskDelay(100/ portTICK_PERIOD_MS);  // báo trong 200ms
    analogWrite(buzzer_pin, 0);  // Tắt tín hiệu
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Nghỉ 20ms
  }
}

// Hàm điều khiển buzzer mức cảnh báo cao
void buzzerAlertHigh(void *pvParameters)
{
  Serial.println("Buzzer High Alert Task Started.");
  for(;;)
  {
    analogWrite(buzzer_pin, 250); // Tín hiệu PWM (100% duty cycle)
    vTaskDelay(100 / portTICK_PERIOD_MS);  // báo trong 100ms
    analogWrite(buzzer_pin, 0);  // Tắt tín hiệu
    vTaskDelay(100 / portTICK_PERIOD_MS); // Nghỉ 50ms
  }
}

void task_RUNG_output(void *pvParameters)
{ 
  BuzzerStatus buzzer_sts = BUZZER_UNKNOWN_STS;
  for(;;)
  {
    vTaskDelay(100/portTICK_PERIOD_MS);
    Message sw_receive {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};

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
                if(buzzerTaskHandle != NULL)
                {
                  vTaskDelete(buzzerTaskHandle); // Xóa task con nếu đang chạy
                  buzzerTaskHandle = NULL;
                }
              }
              break;

            case BUZZER_SW_ENABLE_LOW:
              if(buzzer_sts != BUZZER_LOW_STS)
              {
                if (buzzerTaskHandle != NULL) 
                {
                vTaskDelete(buzzerTaskHandle); // Xóa task con nếu đang chạy
                buzzerTaskHandle = NULL;
                Serial.println("Xoá Task thành công.");
                }
                vTaskDelay(100 / portTICK_PERIOD_MS);
                
                Serial.println("Creating Buzzer Low Task...");
                if (xTaskCreate(buzzerAlertLow, "BuzzerLowTask", 1024, NULL, 5, &buzzerTaskHandle) == pdPASS)
                {
                  Serial.println("Buzzer Low Task Created Successfully.");
                  buzzer_sts = BUZZER_LOW_STS;
                }
                else
                {
                  Serial.println("Failed to Create Buzzer Low Task.");
                }
              }
              break;

            case BUZZER_SW_ENABLE_HIGH:
              if(buzzer_sts != BUZZER_HIGH_STS)
              {
                if(buzzerTaskHandle != NULL)
                {
                  vTaskDelete(buzzerTaskHandle); // Xóa task con nếu đang chạy
                  buzzerTaskHandle = NULL;
                }
                vTaskDelay(100 / portTICK_PERIOD_MS);

                Serial.println("Creating Buzzer High Task...");
                if (xTaskCreate(buzzerAlertHigh, "BuzzerHighTask", 1024, NULL, 5, &buzzerTaskHandle) == pdPASS)
                {
                  Serial.println("Buzzer High Task Created Successfully.");
                  buzzer_sts = BUZZER_HIGH_STS;
                }
                else
                {
                  Serial.println("Failed to Create Buzzer High Task.");
                }
              }
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
