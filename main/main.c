#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_intr_alloc.h"
#include "driver/gpio.h"

/* See "interrupt cpu using table" in esp-idf/components/soc/esp32/include/soc/soc.h */
/* We could use 26 or 31 for an level 5 interrupt */
#define INTR_NUM 31
#define CPU_CORE 1
#define GPIO_NUM 26

uint32_t intr_cnt = 0;

static void cpu1_task(void *arg) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_PIN_INTR_NEGEDGE,
        .pin_bit_mask = (1ULL << GPIO_NUM),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };

    gpio_config(&io_conf);

#if 1
    ESP_INTR_DISABLE(INTR_NUM);
    intr_matrix_set(CPU_CORE, ETS_GPIO_INTR_SOURCE, INTR_NUM);
    ESP_INTR_ENABLE(INTR_NUM);
#else
    /* Should be the proper way, but doesn't work  */
    esp_intr_alloc(ETS_UART1_INTR_SOURCE, ESP_INTR_FLAG_LEVEL5 | ESP_INTR_FLAG_IRAM, NULL, NULL, NULL);
#endif

    while (1) {
        printf("%s alive intr_cnt: %u\n", __FUNCTION__, intr_cnt);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(cpu1_task, "cpu1_task", 2048, NULL, 10, NULL, 1);
}

