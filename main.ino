#include <Arduino.h>

#include "lib/TasksController.h"
#include "lib/XLogger.h"


#define PIN1 12
#define PIN2 13

TasksController *controller;

void setup() {
  initTasksController();
  test();
}

void initTasksController() {
  int pins[] = { PIN2, PIN1 };
  controller = new TasksController(pins);
  controller->start();
}

void loop() {
  XLogger::log("OK");
}

void test() {
}

