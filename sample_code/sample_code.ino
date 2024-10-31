#include <Arduino_FreeRTOS.h>
#include <queue.h>
// mong muốn xây dựng platform cơ bản,
// có hàm gửi, nhận,có chổ cấu hình queue
// define massage format
enum ModuleID
{
  //input Rx
  IN_SW_1801P = 0x01 ,   //... Khai báo cảm biến lên chân digital của Arduino
  IN_MQ_135 = 0x35 ,     //... Khai báo cảm biến lên chân analog của Arduino
  IN_PIR = 0x36 ,        //... Khai báo cảm biến lên chân digital của Arduino
  
  //output Tx
  OUT_BUZZER = 0x37 ,    //... khai báo còi vào chân digital trên arduino
  OUT_FAN = 0x38 ,       //... khai báo quạt vào chân digital trên arduino
  OUT_LED = 0x39         //... khai báo led vào chân digital trên arduino
};

enum ActionPayload 
{ 
  //Buzzer output
  BUZZER_SW_DISABLE,
  BUZZER_SW_ENABLE,

  //Fan output
  FAN_MQ_DISABLE,
  FAN_MQ_ENABLE,
  
  //Led output
  LED_PIR_DISABLE,
  LED_PIR_ENABLE
};

struct Message 
{
  ModuleID id_Tx;
  ModuleID id_Rx;
  ActionPayload payload;
};

QueueHandle_t Platform_Queue;

void setup() 
{
  //setup đầu vào
  pinMode(IN_SW_1801P, INPUT);
  pinMode(IN_MQ_135, INPUT);
  pinMode(IN_PIR, INPUT);

  //setup đầu ra
  pinMode(OUT_BUZZER, OUTPUT);
  pinMode(OUT_FAN, OUTPUT);
  pinMode(OUT_LED, OUTPUT);

  Platform_Queue = xQueueCreate(5, sizeof(Message));

  // Tạo 3 task thực hiện input
  xTaskCreate(task_RUNG, "RUNG", 128, NULL, 1, NULL);
  xTaskCreate(task_KHOI, "KHOI", 128, NULL, 1, NULL);
  xTaskCreate(task_PERSON, "PERSON", 128, NULL, 1, NULL);

  //3 task thực hiện output
  xTaskCreate(task_RUNG_output, "RUNG", 128, NULL, 2, NULL);
  xTaskCreate(task_KHOI_output, "KHOI", 128, NULL, 2, NULL);
  xTaskCreate(task_PERSON_output, "PERSON", 128, NULL, 2, NULL);
}

void loop() 
{
  // Không dùng loop khi chạy FreeRTOS
}

void task_RUNG(void *pvParameters)
{
  for(;;)
  {
   Message sw_send = {0};
      sw_send.id_Tx = OUT_BUZZER;
      sw_send.id_Rx = IN_SW_1801P;
      sw_send.payload = BUZZER_SW_DISABLE;
      xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &sw_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
  }
}  
void task_RUNG_output()
{ 
  for(;;)
  {
     Message sw_receive = {0};
    if(xQueueReceive(Platform_Queue, &sw_receive, portMAX_DELAY) == pdTRUE)
    { 
      // nhận được từ input đã gửi ở trên thì thực hiện.....
    }
    else
    {
      //còn không thì thực hiện......
    }
  }   
}

////////////////////////////
void task_KHOI(void *pvParameters)
{
  for(;;)
  {
    Message mq_send = {0};
      mq_send.id_Tx = OUT_FAN;
      mq_send.id_Rx = IN_MQ_135;
      mq_send.payload = FAN_MQ_DISABLE;
      xQueueSend(Platform_Queue, &mq_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &mq_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
  }
}

void task_KHOI_output(void *pvParameters)
{
  for(;;)
  {
    Message mq_receive = {0};
    if(xQueueReceive(Platform_Queue, &mq_receive, portMAX_DELAY) == pdTRUE)
    { 
      // nhận được từ input đã gửi ở trên thì thực hiện.....
    }
    else
    {
      //còn không thì thực hiện......
    }
  }  
}

//////////////////////////////////////
void task_PERSON(void *pvParameters)
{
  for(;;)
  {
    Message pir_send = {0};
      pir_send.id_Tx = OUT_LED;
      pir_send.id_Rx = IN_PIR;
      pir_send.payload = LED_PIR_DISABLE;
      xQueueSend(Platform_Queue, &pir_send, portMAX_DELAY);
      if(xQueueSend(Platform_Queue, &pir_send, portMAX_DELAY) == pdPASS)
      {
        // Thực hiện nhiệm vụ của input để đóng gói và gửi đi
      }
      else
      {
        //Còn không nhận được thì.... ví dụ pir hư thì đóng gói gửi đi
      }
  }
void task_PERSON_output()
{
  for(;;)
  {
    Message pir_receive = {0};
    if(xQueueReceive(Platform_Queue, &pir_receive, portMAX_DELAY) == pdTRUE)
    { 
      // nhận được từ input đã gửi ở trên thì thực hiện.....
    }
    else
    {
      //còn không thì thực hiện......
    }
  }
}














