// khoi_Task.h
#ifndef TASK_KHOI_H
#define TASK_KHOI_H

#include "platform.h" 

void task_KHOI(void *pvParameters)
{
  int khoi_noiseThreshold = 50;
  int khoi_lastValue=0;
  int khoi_count=0;
  int khoi_alarm1=80;
  int khoi_alarm2 = 120;
  for(;;)
  {
    pinMode(testPin, OUTPUT);
    digitalWrite(testPin,HIGH);
    Message mq_send = {0};
      mq_send.id_Tx = IN_MQ_135;
      mq_send.id_Rx = OUT_FAN;
      mq_send.payload = FAN_MQ_DISABLE;
      if(xQueueSend(Khoi_Queue, &mq_send, portMAX_DELAY) == pdPASS)
      {
        //--------------------------------detect disconnected pin----------------------------//
        digitalWrite(testPin, HIGH);
        bool isHigh = digitalRead(IN_MQ_135) == HIGH;
        digitalWrite(testPin, LOW);
        bool isLow = digitalRead(IN_MQ_135) == LOW;
        if (isHigh && isLow) 
        {
          Serial.println("pin is disconnected");
          mq_send.payload=....;//disconected

        }
        else 
        {
          Serial.println("pin is connected");
          mq_send.payload=....;//conected
        }
        pinMode(testPin, INPUT);
        //---------------------------------chống nhiễu----------------------------//
        khoi_currentvalue = analogRead(IN_MQ_135);
        Serial.print("Khoi Value: ");
        Serial.println(khoi_currentvalue); 
        if(abs(khoi_currentvalue-khoi_lastValue)>=khoi_noiseThreshold)
        { 
          khoi_count++;
          // ngưỡng báo động
          if(khoi_currentvalue>khoi_alarm1 && khoi_count>=3 )// ngưỡng giá trị báo động 
          {
            if(khoi_currentvalue>=khoi_alarm2)
            {
              mq_send.payload = .....;  //action2
              khoi_lastValue=khoi_currentvalue;
              khoi_count=0
            }
            else
            {
            mq_send.payload = .....;  //action1
            khoi_lastValue=khoi_currentvalue;
            khoi_count=0;
            }
          }
          else 
          {
            khoi_count=0;
            if(khoi_currentvalue>khoi_alarm1)
            {
              if(khoi_currentvalue>khoi_alarm2)
              {
                mq_send.payload = .....;  //action2
                khoi_lastValue=khoi_currentvalue;                
              }
              else
              {
                mq_send.payload = .....;  //action1
                khoi_lastValue=khoi_currentvalue;
              }
            }
            else 
            {
              khoi_lastValue=khoi_currentvalue;
              mq_send.payload = FAN_MQ_DISABLE;
              Serial.print("fan off");
            }
          }  
        }    
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
    vTaskDelay(500/ portTICK_PERIOD_MS);
  }
}
#endif