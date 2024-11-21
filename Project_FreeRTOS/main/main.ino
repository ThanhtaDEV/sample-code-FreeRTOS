#include <Arduino_FreeRTOS.h>
#include <queue.h>

#include "platform.h"

#include "task_rung.h"
#include "task_rung_output.h"

#include "task_khoi.h"
#include "task_khoi_output.h"

#include "task_person.h"
#include "task_person_output.h"

QueueHandle_t Rung_Queue;
QueueHandle_t Khoi_Queue;
QueueHandle_t Person_Queue;

void setup() 
{
  Serial.begin(9600);
  //setup đầu vào
  pinMode(IN_SW_1801P_pin, INPUT);
  pinMode(IN_MQ_135, INPUT);
  pinMode(IN_PIR, INPUT);

  //setup đầu ra
  analogWrite(buzzer_pin, 0); // Tắt buzzer ban đầu

  pinMode(OUT_FAN, OUTPUT);
  pinMode(OUT_LED, OUTPUT);

  Rung_Queue = xQueueCreate(5, sizeof(Message));
  Khoi_Queue = xQueueCreate(5, sizeof(Message));
  Person_Queue = xQueueCreate(5, sizeof(Message));

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

