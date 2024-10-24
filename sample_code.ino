#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Đầu vào
#define SW_1801P ... //... khai báo cảm biến SW_1801P vào chân digital trên arduino
#define MQ_135 ...  //... khai báo cảm biến MQ_135 vào chân analog on arduino
#define PIR ...    //... khai báo cảm biến PIR vào chân digital trên arduino

// Đầu ra
#define Buzzer ... //... khai báo còi vào chân digital trên arduino
#define Quat ...  //... khai báo quạt vào chân digital trên arduino
#define Led ...  //... khai báo led vào chân degital trên arduino

//tạo 3 hàm sermaphore
semaphore_handle Rung_semaphore;
semaphore_handle Khoi_semaphore;
semaphore_handle Person_semaphore;

void setup() 
{
  //setup đầu vào
  pinMode(SW_1801P, INPUT);
  pinMode(MQ_135, INPUT);
  pinMode(PIR, INPUT);
  //setup đầu ra
  pinMode(Buzzer, OUTPUT);
  pinMode(Quat, OUTPUT);
  pinMode(Led, OUTPUT);

  Rung_semaphore = xSemaphoreCreateBinary();
  Khoi_semaphore = xSemaphoreCreateBinary();
  Person_semaphore = xSemaphoreCreateBinary();

  // Tạo 3 task thực hiện input
  xTaskCreate(task_rung, "RUNG", 128, NULL, 1, NULL);
  xTaskCreate(task_khoi, "KHOI", 128, NULL, 1, NULL);
  xTaskCreate(task_person, "PERSON", 128, NULL, 1, NULL);

  //3 task thực hiện output
  xTaskCreate(task_rung_output, "RUNG", 128, NULL, 2, NULL);
  xTaskCreate(task_khoi_output, "KHOI", 128, NULL, 2, NULL);
  xTaskCreate(task_person_output, "PERSON", 128, NULL, 2, NULL);
}

void loop() 
{
  // Không dùng loop khi chạy FreeRTOS
}

void task_rung(void *pvParameters)
{
  for(;;)
  {
    if (digitalRead(SW_1801P) == HIGH)
    {
      xSemaphoreGive(Rung_semaphore);
    }
void task_rung_output()
{ 
  for(;;)
  {
    if(xSemaphoreTask(Rung_semaphore, portMAX_DELAY))
    {
      digitalWrite(Buzzer, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(Buzzer, LOW)
    }
  }   
}

void task_khoi(void *pvParameters)
{
  for(;;)
  {
    int giatri_MQ = analogRead(MQ_135);

    if(giatri_MQ > ...) //... nồng độ khói mà người dùng muốn cài 
    {
      xSemaphoreGive(Khoi_semaphore);
    }
  }
}
void task_khoi_output(void *pvParameters)
{
  for(;;)
  {
    if(xSemaphoreTask(Khoi_semaphore, portMAX_DELAY))
    {
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Led, HIGH);
      digitalWrite(Quat, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(Buzzer, LOW);
    }
  }  
}

void task_person(void *pvParameters)
{
  for(;;)
  {
    if(digitalRead(PIR) == HIGH);
    {
      xSemaphoreGive(Person_semaphore);
    }
  }
void task_person_output()
{
  for(;;)
  {
    if(xSemaphoreTake(Person_semaphore, portMAX_DELAY))
    {
    digitalWrite(Led, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(Led, LOW);
    }
  }
}

//Yều cầu quạt hư thì có cần quản lí thiết bị truyne62 cho con Arduino xử lí rồi
//in ra màn hình thiết bị hay ko














