#ifndef __OLED_H__
#define __OLED_H__

#include "typedef.h"
#include "oled_data.h"

// OLED 分配的IIC索引
#define OLED_IIC_DEV_IDX 0
#define OLED_SCL_PIN     IO_PORTA_00
#define OLED_SDA_PIN     IO_PORTB_07

// oled 刷新显示的时间周期，单位：x10ms
#define OLED_DISPLAY_REFRESH_TIME ((u8)5)

// oled 驱动芯片横向分辨率
#define OLED_MAX_COLUMN_SiZE (128)
// oled 驱动芯片纵向分辨率
#define OLED_MAX_ROW_SIZE  (64)
#define OLED_MAX_PAGE_SIZE (8)
#define OLED_MAX_PAGE_NUM  (OLED_MAX_ROW_SIZE / OLED_MAX_PAGE_SIZE)

// oled 横向和纵向起始地址
#define OLED_COLUMN_BASE_L (0x00)
#define OLED_COLUMN_BASE_H (0x10)
#define OLED_PAGE_BASE     (0xB0)

void oled_write_data(u8 data);
void oled_write_cmd(u8 cmd);

void oled_set_cursor(u8 column, u8 page);
void oled_draw_point(u8 x, u8 y, u8 is_display);

void oled_display_refresh(void);
void oled_display_clear(void);
void oled_display_clear_area(s16 x, s16 y, u8 width, u8 height);

void oled_display_img(s16 x, s16 y, u8 width, u8 height, const u8 *img);
void oled_display_char(u8 x, u8 y, u8 ch, oled_font_size_type_t font_size);
void oled_display_string(u8 x, u8 y, const char *str,
                         oled_font_size_type_t font_size_type);

void oled_init(void);

void oled_display_refresh_10ms_isr(void);
void oled_display_refresh_handle(void);

#endif
