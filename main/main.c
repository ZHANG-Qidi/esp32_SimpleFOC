#include <stdio.h>

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    setup();
    TickType_t last = xTaskGetTickCount();
    for (;;) {
        vTaskDelayUntil(&last, pdMS_TO_TICKS(1));
        loop();
    }
}
