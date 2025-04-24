#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void ui_init();
void ui_draw_start(uint8_t *data_bytes, size_t data_size, size_t width, size_t height);
void ui_draw_box(int16_t x, int16_t y, int16_t w, int16_t h, int16_t class_index);
void ui_draw_end();

#ifdef __cplusplus
}
#endif