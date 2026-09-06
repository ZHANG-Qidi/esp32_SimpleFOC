#include <stdio.h>

#include "Arduino.h"
#include "arduino_main.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t app_main_handle;

static bool example_timer_on_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(app_main_handle, &xHigherPriorityTaskWoken);
    return (xHigherPriorityTaskWoken == pdTRUE);
}

void gptimer_init(void) {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1 * 1000 * 1000,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = ARDUINO_LOOP_PERIOD,
        .flags.auto_reload_on_alarm = true,

    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = example_timer_on_alarm_cb,

    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void app_main(void) {
    app_main_handle = xTaskGetCurrentTaskHandle();
    gptimer_init();
    setup();
    // TickType_t last = xTaskGetTickCount();
    for (;;) {
        // vTaskDelayUntil(&last, pdMS_TO_TICKS(1));
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        loop();
    }
}
