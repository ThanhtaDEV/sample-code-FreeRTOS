// rung_Task.h
#ifndef TASK_RUNG_H
#define TASK_RUNG_H

#include "platform.h" // Chứa định nghĩa ModuleID, ActionPayload, Message, QueueHandle_t

int time_interval = 3000;
int threshold_low = 5;
int threshold_medium = 10;
int count = 0;
void sendMessage(ModuleID id_Tx, ModuleID id_Rx, ActionPayload payload, QueueHandle_t queue);

void task_RUNG(void *pvParameters)
{
  for(;;)
  {
    unsigned long start_time = millis(); //Thời điểm bắt đầu đo
    while (millis() - start_time < time_interval )
    { 
      if(analogRead(IN_SW_1801P_pin) < 1000 )
      {
        count++;
        Serial.print("Count Value: ");
        Serial.println(count);
      }
      if(count > threshold_medium + 5)
      {
        Serial.print("Vượt quá ngưỡng test: ");
        Serial.println(count);
        break;
      }
      vTaskDelay(100/portTICK_PERIOD_MS);
    }
    // thông điệm gửi đi theo ngưỡng tần suất
    if (count <= threshold_low) // count <= 5
    { 
      sendMessage(IN_SW_1801P, OUT_BUZZER, BUZZER_SW_DISABLE, Rung_Queue);
      Serial.println("DISABLE WARNING lEVEL");
      Serial.println(count);
    }

    else if (count > threshold_low && count <= threshold_medium)  // 5<...<10
    {   
      sendMessage(IN_SW_1801P, OUT_BUZZER, BUZZER_SW_ENABLE_LOW, Rung_Queue);
      Serial.print("LOW WARNING LEVEL: ");
      Serial.println(count);
    }

    else
    {
      sendMessage(IN_SW_1801P, OUT_BUZZER, BUZZER_SW_ENABLE_HIGH, Rung_Queue);
      Serial.print("HIGH WARNING LEVEL: ");
      Serial.println(count);
    }
    if( count > 0)
    {
      count--;
    }
  }
}  

void sendMessage(ModuleID id_Tx, ModuleID id_Rx, ActionPayload payload, QueueHandle_t queue)
{
    Message sw_send {INVALID_MODULEID, INVALID_MODULEID, INVALID_ACTIONPAYLOAD};
    sw_send.id_Tx = id_Tx;
    sw_send.id_Rx = id_Rx;
    sw_send.payload = payload;
    if (xQueueOverwrite(queue, &sw_send) == pdPASS)
    {
      Serial.print("task_rung.h sendMessage() Message sent: ");
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