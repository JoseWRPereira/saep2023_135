#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.h"
#include "saidas.h"


void app_main(void)
{
    // gpio_set_direction(GPIO_NUM_2,GPIO_MODE_INPUT_OUTPUT);
    saidas_init();
    wifi_init();

    while( 1 )
    {
        // gpio_set_level(GPIO_NUM_2, !gpio_get_level(GPIO_NUM_2));
        // vTaskDelay(500/portTICK_PERIOD_MS);
        vTaskDelay(100);
    }
}
