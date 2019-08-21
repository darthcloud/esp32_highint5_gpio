#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_intr_alloc.h"
#include "driver/gpio.h"

uint32_t test;

static void cpu1_task(void *arg) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_PIN_INTR_NEGEDGE,
        .pin_bit_mask = (1ULL << 26),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };

    gpio_config(&io_conf);

#if 1
    ESP_INTR_DISABLE(26);
    intr_matrix_set(1, ETS_GPIO_INTR_SOURCE, 26);
    ESP_INTR_ENABLE(26);
#else
    esp_intr_alloc(ETS_UART1_INTR_SOURCE, ESP_INTR_FLAG_LEVEL5 | ESP_INTR_FLAG_IRAM, NULL, NULL, NULL);
#endif

    while (1) {
        printf("%s alive test: %u\n", __FUNCTION__, test);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(cpu1_task, "cpu1_task", 2048, NULL, 10, NULL, 1);
}

