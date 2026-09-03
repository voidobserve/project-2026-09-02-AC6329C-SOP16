#ifndef __OLED_H__
#define __OLED_H__

// OLED 分配的IIC索引
#define OLED_IIC_DEV_IDX 0
#define OLED_SCL_PIN     IO_PORTA_00
#define OLED_SDA_PIN     IO_PORTB_07

// oled 驱动芯片横向分辨率
#define OLED_MAX_COLUMN_SiZE (128)
// oled 驱动芯片纵向分辨率
#define OLED_MAX_ROW_SIZE  (64)
#define OLED_MAX_PAGE_SIZE (8)
#define OLED_MAX_PAGE_NUM  (OLED_MAX_ROW_SIZE / OLED_MAX_PAGE_SIZE)

// oled 横向和纵向起始地址
#define OLED_COLUMN_BASE (0x00)
#define OLED_PAGE_BASE   (0xB0)

#endif
