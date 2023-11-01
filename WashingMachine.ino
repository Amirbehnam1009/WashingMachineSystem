#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <LiquidCrystal.h>
#include <semphr.h>

const int fillMotorPin = 40;
const int spinMotorPin = 41;
const int rinseMotorPin = 42;

const int ledPin = 51;
const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Task handles
TaskHandle_t fillWaterTaskHandle;
TaskHandle_t spinTaskHandle;
TaskHandle_t rinseTaskHandle;

// Task periods (in milliseconds)
const unsigned long fillPeriod = 5000;   // Example period for filling water
const unsigned long spinPeriod = 3000;   // Example period for spinning
const unsigned long rinsePeriod = 4000;  // Example period for rinsing

// Task priorities
const int fillPriority = 1;
const int spinPriority = 2;
const int rinsePriority = 2;

// Mutexes
SemaphoreHandle_t mutex;

// Task functions
void fillWaterTask(void* parameter) {
  while (true) {
    xSemaphoreTake(mutex, portMAX_DELAY);

    // Control water filling process
    lcd.clear();
    lcd.print("Filling Water");
    digitalWrite(fillMotorPin, HIGH);
    delay(2000);  // Adjust the duration as per your requirement
    digitalWrite(fillMotorPin, LOW);

    xSemaphoreGive(mutex);

    vTaskDelay(pdMS_TO_TICKS(fillPeriod));  // Delay based on task period
  }
}

void spinTask(void* parameter) {
  while (true) {
    xSemaphoreTake(mutex, portMAX_DELAY);

    // Control spinning operation
    lcd.clear();
    lcd.print("Spinning");
    digitalWrite(spinMotorPin, HIGH);
    delay(3000);  // Adjust the duration as per your requirement
    digitalWrite(spinMotorPin, LOW);

    xSemaphoreGive(mutex);

    vTaskDelay(pdMS_TO_TICKS(spinPeriod));  // Delay based on task period
  }
}

void rinseTask(void* parameter) {
  while (true) {
    xSemaphoreTake(mutex, portMAX_DELAY);

    // Control rinsing process
    lcd.clear();
    lcd.print("Rinsing");
    digitalWrite(rinseMotorPin, HIGH);
    delay(2500);  // Adjust the duration as per your requirement
    digitalWrite(rinseMotorPin, LOW);

    xSemaphoreGive(mutex);

    vTaskDelay(pdMS_TO_TICKS(rinsePeriod));  // Delay based on task period
  }
}

void setup() {
  // Initialize hardware and pins
  pinMode(fillMotorPin, OUTPUT);
  pinMode(spinMotorPin, OUTPUT);
  pinMode(rinseMotorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);  // Initialize LCD
  lcd.print("Washing Machine");

  // Create mutex
  mutex = xSemaphoreCreateMutex();

  // Check if mutex was created successfully
  if (mutex != NULL) {
    // Create tasks with assigned priorities
    xTaskCreate(fillWaterTask, "FillWater", 1000, NULL, fillPriority, &fillWaterTaskHandle);
    xTaskCreate(spinTask, "Spin", 1000, NULL, spinPriority, &spinTaskHandle);
    xTaskCreate(rinseTask, "Rinse", 1000, NULL, rinsePriority, &rinseTaskHandle);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();
  }
}

void loop() {
  // Empty loop, tasks are scheduled to run by FreeRTOS
}