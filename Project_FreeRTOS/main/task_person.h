#ifndef TASK_PERSON_H
#define TASK_PERSON_H


#include "platform.h"
int person_time_interval = 1000;
//int person_threshold_low = 5;
int person_threshold_high = 20;
int person_count = 0;
void sendMessage_1(ModuleID id_Tx, ModuleID id_Rx, ActionPayload payload, QueueHandle_t queue);

void task_PERSON(void *pvParameters)
{
  for(;;)
  {
    unsigned long person_start_time = millis(); //Thời điểm bắt đầu đo
    while (millis() - person_start_time < person_time_interval )
    { 
      int val=analogRead(IN_PIR_pin);
      if(val>600)
      {
        person_count++;
        Serial.print("Count Value: ");
        //Serial.println(person_count);
        Serial.println(val);
         //tránh đếm trùng tín hiệu
          if(person_count > person_threshold_high + 10)
        {
          break;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
      }
      
    }
    // thông điệm gửi đi theo ngưỡng tần suất
    if (person_count <= person_threshold_high) // count <= 5
    { 
      sendMessage_1(IN_PIR, OUT_LED, LED_PIR_DISABLE, Person_Queue);
      Serial.println("DISABLE WARNING lEVEL: ");
      Serial.println(person_count);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else
    {
      sendMessage_1(IN_PIR,OUT_LED , LED_PIR_ENABLE_HIGH, Person_Queue);
      Serial.print("HIGH WARNING LEVEL: ");
      Serial.println(person_count);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    if(person_count>0)
    {
      person_count--;     
    }
  }
}

void sendMessage_1(ModuleID id_Tx, ModuleID id_Rx, ActionPayload payload, QueueHandle_t queue)
{
    Message pir_send {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};
    pir_send.id_Tx = id_Tx;
    pir_send.id_Rx = id_Rx;
    pir_send.payload = payload;

    if (xQueueOverwrite(queue, &pir_send) == pdPASS)
    {
      Serial.print("Message sent: ");
      Serial.print("id_Tx: ");
      Serial.print(id_Tx);
      Serial.print(", id_Rx: ");
      Serial.print(id_Rx);
      Serial.print(", payload: ");
      Serial.println(payload);
    }
    else
    {
      Serial.println("Failed to send message.");
    }
}
#endif