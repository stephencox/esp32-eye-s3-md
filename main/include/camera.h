#pragma once

#include <esp_camera.h>
#include <esp_err.h>

#define CAM_WIDTH              (240)
#define CAM_HEIGHT             (240)

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t camera_init();
esp_err_t camera_capture();
void camera_process_image(camera_fb_t *fb);

#ifdef __cplusplus
}
#endif
