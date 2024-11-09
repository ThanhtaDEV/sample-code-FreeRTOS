#ifndef TASK_KHOI_OUTPUT_H
#define TASK_KHOI_OUTPUT_H

#include "platform.h"
#define FAN_PiN 3
void task_khoi_output(void *pvParameters)
{
  int flag_pri = 0;
  int pre_flag_pri = 0;
  for(;;) {
    Message mq_receive = {0};
    
    // Nhận thông điệp từ hàng đợi
    if(xQueueReceive(Khoi_Queue, &mq_receive, portMAX_DELAY) == pdTRUE) {
      // Xử lý thông điệp nhận được cho OUT_FAN
      if(mq_receive.id_Rx == OUT_FAN) {
        if(mq_receive.id_Tx == IN_MQ_135) {
          flag_pri = 1;
          if(pre_flag_pri < flag_pri) {
            pre_flag_pri = flag_pri;
            switch (mq_receive.payload) {
              case FAN_MQ_DISABLE:
                turnOffFan();
                break;
              case FAN_MQ_ENABLE:
                adjustFanSpeed(mq_receive.//payloadValue); // Điều chỉnh tốc độ quạt
                break;
              default:
                break;
            }
          }
        }
  /*
        if(mq_receive.id_Tx == IN_PIR) {
          flag_pri = 2;
          if(pre_flag_pri < flag_pri) {
            pre_flag_pri = flag_pri;
            switch (mq_receive.payload) {
              case FAN_MQ_DISABLE:
                turnOffFan();
                break;
              case FAN_MQ_ENABLE:
                adjustFanSpeed(mq_receive.//payloadValue); // Điều chỉnh tốc độ quạt
                break;
              default:
                break;
            }
          }
        }

        // Nếu command là FAN_MQ_DISABLE thì tắt quạt hoàn toàn
        if (mq_receive.payload == FAN_MQ_DISABLE) {
          turnOffFan();
        }
      }
    }
  */
    else {
     
    }
    
    vTaskDelay(500 / portTICK_PERIOD_MS);

}

// Hàm điều chỉnh tốc độ quạt từ 0 đến 100%
void adjustFanSpeed(int speedPercentage) {
  int pwmValue = map(speedPercentage, 0, 100, 0, 255); // Chuyển đổi từ 0-100% sang 0-255 PWM
  analogWrite(FAN_PIN, pwmValue);
  Serial.print("Fan speed set to ");
  Serial.print(speedPercentage);
  Serial.println("%");
}

// Hàm tắt quạt
void turnOffFan() {
  analogWrite(FAN_PIN, 0); // Tắt quạt hoàn toàn
  Serial.println("Fan turned off");
}
#endif




