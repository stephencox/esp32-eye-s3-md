#include "lcd.h"

#include "bsp.h"
#include "camera.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_err.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_log.h>
#include <esp_lvgl_port.h>
#include <freertos/FreeRTOS.h>

#define TAG "esp32-s3-eye-lcd"

static esp_lcd_panel_handle_t lcd_panel    = NULL;
static esp_lcd_panel_io_handle_t io_handle = NULL;
static lv_disp_t *disp_handle              = NULL;

static void lcd_backlight_off() { ESP_ERROR_CHECK(gpio_set_level(BSP_LCD_BACKLIGHT, 1)); }

void lcd_backlight_on() { ESP_ERROR_CHECK(gpio_set_level(BSP_LCD_BACKLIGHT, 0)); }

static void lcd_init_backlight()
{
    gpio_config_t bk_gpio_config = {.mode = GPIO_MODE_OUTPUT, .pin_bit_mask = 1ULL << BSP_LCD_BACKLIGHT};
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
}

static void lcd_init_spi()
{
    spi_bus_config_t buscfg = {
        .sclk_io_num     = BSP_LCD_SPI_CLK,
        .mosi_io_num     = BSP_LCD_SPI_MOSI,
        .miso_io_num     = GPIO_NUM_NC,
        .quadwp_io_num   = GPIO_NUM_NC,
        .quadhd_io_num   = GPIO_NUM_NC,
        .max_transfer_sz = LCD_H_RES * LCD_V_RES / 10 * 2 + 8,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(BSP_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO));
}

static void lcd_init_panel()
{
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num       = BSP_LCD_DC,
        .cs_gpio_num       = BSP_LCD_SPI_CS,
        .pclk_hz           = BSP_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits      = LCD_CMD_BITS,
        .lcd_param_bits    = LCD_PARAM_BITS,
        .spi_mode          = 3,
        .trans_queue_depth = 10,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BSP_LCD_SPI_NUM, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BSP_LCD_RST,
        .color_space    = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
    };
    // Initialize the LCD configuration
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &lcd_panel));

    // Reset the display
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_panel));

    // Initialize LCD panel
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd_panel, true));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel, true));
}

void lcd_lvgl_init()
{
    ESP_LOGI(TAG, "Initialize lvgl port");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    /* Add LCD screen */
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle     = io_handle,
        .panel_handle  = lcd_panel,
        .buffer_size   = LCD_H_RES * LCD_V_RES / 10 * 2,
        .double_buffer = true,
        .hres          = LCD_H_RES,
        .vres          = LCD_V_RES,
        .monochrome    = false,
        .color_format  = LV_COLOR_FORMAT_RGB565,
        .rotation =
            {
                .swap_xy  = false,
                .mirror_x = false,
                .mirror_y = false,
            },
        .flags =
            {
                .buff_dma   = true,
                .swap_bytes = true,
            },
    };

    disp_handle = lvgl_port_add_disp(&disp_cfg);
}

void lcd_init()
{
    lcd_init_backlight();
    lcd_backlight_off();
    lcd_init_spi();
    lcd_init_panel();
}
