#include "camera.h"

#include "lcd.h"
#include "ui.h"

#include <coco_detect.hpp>
#include <esp_log.h>

#define TAG "esp32-s3-eye-camera-cpp"

static COCODetect *detect = NULL;

void camera_process_image(camera_fb_t *fb)
{
    dl::image::img_t img;
    img.pix_type = dl::image::DL_IMAGE_PIX_TYPE_RGB565;
    img.width    = fb->width;
    img.height   = fb->height;
    img.data     = fb->buf;

    ESP_LOGI(TAG, "Run detection");
    if (detect == NULL)
    {
        detect = new COCODetect();
    }

    auto &detect_results = detect->run(img);
    ESP_LOGI(TAG, "Done with detection");

    ui_draw_start(fb->buf, fb->height * fb->width * 2, fb->width, fb->height);

    for (const auto &res : detect_results)
    {
        ESP_LOGI(TAG, "[category: %d, score: %f, x1: %d, y1: %d, x2: %d, y2: %d]", res.category, res.score, res.box[0],
                 res.box[1], res.box[2], res.box[3]);

        ui_draw_box(res.box[0], res.box[1], res.box[2], res.box[3], res.category);
    }
    ui_draw_end();
}
