#include "camera.h"

#include "bsp.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#define TAG "esp32-s3-eye-camera-c"

static camera_config_t camera_config = {
    .pin_pwdn     = BSP_CAMERA_PWDN,
    .pin_reset    = BSP_CAMERA_RESET,
    .pin_xclk     = BSP_CAMERA_XCLK,
    .pin_sccb_sda = BSP_CAMERA_SIOD,
    .pin_sccb_scl = BSP_CAMERA_SIOC,
    .pin_d7       = BSP_CAMERA_D7,
    .pin_d6       = BSP_CAMERA_D6,
    .pin_d5       = BSP_CAMERA_D5,
    .pin_d4       = BSP_CAMERA_D4,
    .pin_d3       = BSP_CAMERA_D3,
    .pin_d2       = BSP_CAMERA_D2,
    .pin_d1       = BSP_CAMERA_D1,
    .pin_d0       = BSP_CAMERA_D0,
    .pin_vsync    = BSP_CAMERA_VSYNC,
    .pin_href     = BSP_CAMERA_HSYNC,
    .pin_pclk     = BSP_CAMERA_PCLK,
    .xclk_freq_hz = BSP_CAMERA_XCLK_FREQ,
    .ledc_timer   = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_RGB565,
    .frame_size   = FRAMESIZE_240X240,
    .fb_count     = 1,
    .grab_mode    = CAMERA_GRAB_WHEN_EMPTY,
};

esp_err_t camera_init()
{
    // Initialize the camera with the given configuration
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        // Log an error if camera initialization fails
        ESP_LOGE(TAG, "Camera init failed");
        return err;
    }

    sensor_t *sensor = esp_camera_sensor_get();
    if (sensor == NULL)
    {
        // Log an error if the sensor cannot be retrieved
        ESP_LOGE(TAG, "Could not get sensor");
        return ESP_FAIL;
    }

    // Flip the image vertically
    sensor->set_vflip(sensor, 1); // flip it back

    return ESP_OK;
}
