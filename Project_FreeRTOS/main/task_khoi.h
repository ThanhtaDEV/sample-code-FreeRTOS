// khoi_Task.h
#ifndef TASK_KHOI_H
#define TASK_KHOI_H

#include "platform.h" 
int khoi_time_interval=3000;
int khoi_count=0;
int khoi_alarm1=150;
int khoi_alarm2 =750;
int khoi_threshold_count=20;
int khoi_currentvalue=0;
void sendMessage(int id_Tx, int id_Rx, int payload, QueueHandle_t queue);

void task_KHOI(void *pvParameters)
{
  for(;;)
  {   
    
    unsigned long khoi_start_time = millis();
    while(millis()-khoi_start_time < khoi_time_interval)
    {
      khoi_currentvalue=analogRead(IN_MQ_135_pin);
      if(khoi_currentvalue > khoi_alarm1 )
      {
        khoi_count++;
        Serial.print("Count Value: ");
        Serial.println(khoi_count);
        Serial.print("Khoi Value: ");
        Serial.println(khoi_currentvalue);
        if(khoi_count>khoi_threshold_count)
        {
          break;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);      
      }
    } 
    if(khoi_currentvalue<khoi_alarm1)
    {
        sendMessage(IN_MQ_135, OUT_FAN, FAN_MQ_DISABLE, Khoi_Queue);
        Serial.println("DISABLE WARNING lEVEL: ");
        Serial.println(khoi_currentvalue);
        vTaskDelay(1800 / portTICK_PERIOD_MS);
    }
    else if(khoi_currentvalue > khoi_alarm1 && khoi_currentvalue <= khoi_alarm2)
  
    {
        sendMessage(IN_MQ_135, OUT_FAN, FAN_MQ_ENABLE_MEDIUM, Khoi_Queue);
        Serial.println("LOW WARNING lEVEL: ");
        Serial.println(khoi_currentvalue);
        vTaskDelay(1800 / portTICK_PERIOD_MS);
    }
    else if(khoi_currentvalue>khoi_alarm2)
    {
        sendMessage(IN_MQ_135, OUT_FAN, FAN_MQ_ENABLE_HIGH, Khoi_Queue);
        Serial.println("HIGH WARNING lEVEL: ");
        Serial.println(khoi_currentvalue);
        vTaskDelay(1800 / portTICK_PERIOD_MS);
    }  
    if(khoi_count>0)
    {
      khoi_count=0;
    }
  }
}
void sendMessage(int id_Tx, int id_Rx, int payload, QueueHandle_t queue)
{
    Message mq_send = {0};
    mq_send.id_Tx = id_Tx;
    mq_send.id_Rx = id_Rx;
    mq_send.payload = payload;
    if (xQueueSend(queue, &mq_send, portMAX_DELAY) == pdPASS)
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