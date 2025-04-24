#pragma once

#define LCD_CMD_BITS           (8)
#define LCD_PARAM_BITS         (8)

/* Display */
#define BSP_LCD_SPI_MOSI       (GPIO_NUM_47)
#define BSP_LCD_SPI_CLK        (GPIO_NUM_21)
#define BSP_LCD_SPI_CS         (GPIO_NUM_44)
#define BSP_LCD_DC             (GPIO_NUM_43)
#define BSP_LCD_RST            (GPIO_NUM_NC)
#define BSP_LCD_BACKLIGHT      (GPIO_NUM_48)

/* Camera */
#define BSP_CAMERA_XCLK        (GPIO_NUM_15)
#define BSP_CAMERA_PCLK        (GPIO_NUM_13)
#define BSP_CAMERA_VSYNC       (GPIO_NUM_6)
#define BSP_CAMERA_HSYNC       (GPIO_NUM_7)
#define BSP_CAMERA_D0          (GPIO_NUM_11)
#define BSP_CAMERA_D1          (GPIO_NUM_9)
#define BSP_CAMERA_D2          (GPIO_NUM_8)
#define BSP_CAMERA_D3          (GPIO_NUM_10)
#define BSP_CAMERA_D4          (GPIO_NUM_12)
#define BSP_CAMERA_D5          (GPIO_NUM_18)
#define BSP_CAMERA_D6          (GPIO_NUM_17)
#define BSP_CAMERA_D7          (GPIO_NUM_16)
#define BSP_CAMERA_SIOD        (GPIO_NUM_4)
#define BSP_CAMERA_SIOC        (GPIO_NUM_5)
#define BSP_CAMERA_PWDN        (GPIO_NUM_NC)
#define BSP_CAMERA_RESET       (GPIO_NUM_NC)
#define BSP_CAMERA_XCLK_FREQ   (24 * 1000 * 1000)

#define BSP_LCD_PIXEL_CLOCK_HZ (80 * 1000 * 1000)
#define BSP_LCD_SPI_NUM        (SPI3_HOST)