#include "ui.h"

#include "bsp.h"
#include "camera.h"
#include "lcd.h"
#include "yolo.h"

#include <esp_log.h>
#include <esp_lvgl_port.h>

#define TAG "esp32-eye-s3-ui"

static lv_obj_t *ui_canvas = NULL;
static lv_layer_t ui_layer;
static uint8_t *image_buffer    = NULL;
static lv_image_dsc_t image_dsc = {
    .header.cf = LV_COLOR_FORMAT_RGB565,
    .header.w  = CAM_WIDTH,
    .header.h  = CAM_HEIGHT,
    .data_size = CAM_WIDTH * CAM_HEIGHT * 2,
};

static void ui_create()
{
    image_buffer = calloc(CAM_WIDTH * CAM_HEIGHT * 2, sizeof(uint8_t));
    if (image_buffer == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate buffer");
        return;
    }

    image_dsc.data = (const uint8_t *)image_buffer;

    lvgl_port_lock(0);

    LV_DRAW_BUF_DEFINE_STATIC(draw_buf, LCD_H_RES, LCD_V_RES, LV_COLOR_FORMAT_RGB565);
    LV_DRAW_BUF_INIT_STATIC(draw_buf);
    ui_canvas = lv_canvas_create(lv_screen_active());
    lv_canvas_set_draw_buf(ui_canvas, &draw_buf);
    lv_obj_center(ui_canvas);

    lvgl_port_unlock();
}

void ui_init()
{
    lcd_lvgl_init();
    ui_create();
    lcd_backlight_on();
}

void ui_draw_start(uint8_t *data_bytes, size_t data_size, size_t width, size_t height)
{
    lvgl_port_lock(0);

    // Copy data and swap bytes
    for (int i = 0; i < data_size; i += 2)
    {
        image_buffer[i]     = data_bytes[i + 1];
        image_buffer[i + 1] = data_bytes[i];
    }

    lv_canvas_init_layer(ui_canvas, &ui_layer);

    lv_draw_image_dsc_t dsc;
    lv_draw_image_dsc_init(&dsc);
    dsc.src          = &image_dsc;

    lv_area_t coords = {0, 0, LCD_H_RES, LCD_V_RES};
    lv_draw_image(&ui_layer, &dsc, &coords);
}

void ui_draw_box(int16_t left_up_x, int16_t left_up_y, int16_t right_down_x, int16_t right_down_y, int16_t class)
{
    lv_draw_rect_dsc_t dsc;
    lv_draw_rect_dsc_init(&dsc);
    dsc.bg_opa       = LV_OPA_TRANSP;
    dsc.border_color = lv_palette_main(LV_PALETTE_BLUE);
    dsc.border_width = 2;

    lv_area_t coords = {left_up_x, left_up_y, right_down_x, right_down_y};
    lv_draw_rect(&ui_layer, &dsc, &coords);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_palette_main(LV_PALETTE_BLUE);
    label_dsc.text  = yolo_get_class_name(class);
    label_dsc.opa   = LV_OPA_COVER;

    lv_draw_label(&ui_layer, &label_dsc, &coords);
}

void ui_draw_end()
{
    lv_canvas_finish_layer(ui_canvas, &ui_layer);
    lvgl_port_unlock();
}
