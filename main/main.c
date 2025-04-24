#include "camera.h"
#include "lcd.h"
#include "ui.h"

#include <esp_log.h>
#include <esp_lvgl_port.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char *TAG = "esp32-s3-eye-main";

void app_main(void)
{
    lcd_init();
    ui_init();
    ESP_ERROR_CHECK(camera_init());

    while (1)
    {
        ESP_LOGD(TAG, "Capture image");
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb)
        {
            ESP_LOGE(TAG, "Camera Capture Failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGD(TAG, "Process image");
        camera_process_image(fb);

        esp_camera_fb_return(fb);
    }
}
