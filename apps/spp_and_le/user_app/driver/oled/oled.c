#include "oled.h"
#include <string.h> // memset
#include "includes.h"
#include "asm/iic_soft.h"
#include "oled_data.h"

#include "rf24g_key.h"

volatile u8 oled_display_refresh_cnt = 0;

// OLED的显存
// 存放格式如下.
// [0]0 1 2 3 ... 127
// [1]0 1 2 3 ... 127
// [2]0 1 2 3 ... 127
// [3]0 1 2 3 ... 127
// [4]0 1 2 3 ... 127
// [5]0 1 2 3 ... 127
// [6]0 1 2 3 ... 127
// [7]0 1 2 3 ... 127
volatile u8 oled_display_buf[OLED_MAX_COLUMN_SiZE][OLED_MAX_PAGE_NUM];

const struct soft_iic_config soft_iic_cfg[] = {
    {
        .scl = OLED_SCL_PIN,
        .sda = OLED_SDA_PIN,
        .delay = 10,
        .io_pu = 0,
    },
};

void oled_write_cmd(u8 cmd)
{
    soft_iic_start(OLED_IIC_DEV_IDX);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x78); // 发送OLED的I2C从机地址
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x00); // 0x00，表示即将写命令
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, cmd);
    soft_iic_stop(OLED_IIC_DEV_IDX);
}

void oled_write_data(u8 data)
{
    soft_iic_start(OLED_IIC_DEV_IDX);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x78); // 发送OLED的I2C从机地址
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x40); // 0x40，表示即将写数据
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, data);
    soft_iic_stop(OLED_IIC_DEV_IDX);
}

void oled_set_cursor(u8 column, u8 page)
{
    column += OLED_COLUMN_BASE_L;
    oled_write_cmd(page + OLED_PAGE_BASE);
    oled_write_cmd(0x0F & column);
    oled_write_cmd((column & 0xF0) >> 4 | OLED_COLUMN_BASE_H);
}

void oled_draw_point(u8 x, u8 y, u8 is_display)
{
    u8 page_idx;
    u8 bit_mask;

    if (x >= OLED_MAX_COLUMN_SiZE || y >= OLED_MAX_ROW_SIZE) {
        return;
    }

    page_idx = y / 8;
    bit_mask = 0x01 << (y % 8);

    if (is_display) {
        oled_display_buf[x][page_idx] |= bit_mask;
    } else {
        oled_display_buf[x][page_idx] &= ~bit_mask;
    }
}

// 更新显示
void oled_display_refresh(void)
{
    u8 i;
    u8 j;
    for (i = 0; i < OLED_MAX_PAGE_NUM; i++) {
        oled_set_cursor(0, i);
        for (j = 0; j < OLED_MAX_COLUMN_SiZE; j++) {
            oled_write_data(oled_display_buf[j][i]);
        }
    }
}

// 清空显示
void oled_display_clear(void)
{
    memset(oled_display_buf, 0, sizeof(oled_display_buf));
}

/**
 * @brief 清空显存中指定区域的数据
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 */
void oled_display_clear_area(s16 x, s16 y, u8 width, u8 height)
{
    s16 i;
    s16 j;

    //遍历指定页
    for (j = y; j < y + height; j++) {
        //遍历指定列
        for (i = x; i < x + width; i++) {
            //超出屏幕的内容不显示
            if (i >= 0 && i <= (OLED_MAX_COLUMN_SiZE - 1) && j >= 0 &&
                j <= (OLED_MAX_ROW_SIZE - 1)) {
                //将显存数组指定数据清零
                oled_display_buf[i][j / 8] &= ~(0x01 << (j % 8));
            }
        }
    }
}

/**
 * @brief 显示一个图像
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param img 
 */
void oled_display_img(s16 x, s16 y, u8 width, u8 height, const u8 *img)
{
    u8 i = 0;
    u8 j = 0;
    s16 page;
    s16 shift;

    /*将图像所在区域清空*/
    oled_display_clear_area(x, y, width, height);

    /*遍历指定图像涉及的相关页*/
    /*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
    for (j = 0; j < (height - 1) / 8 + 1; j++) {
        /*遍历指定图像涉及的相关列*/
        for (i = 0; i < width; i++) {
            // 超出屏幕的内容不显示
            if (x + i >= 0 && x + i <= (OLED_MAX_COLUMN_SiZE - 1)) {
                /*负数坐标在计算页地址和移位时需要加一个偏移*/
                /* 将坐标归一化为向下取整的页地址和页内偏移 */
                page = y / 8;
                shift = y % 8;
                if (y < 0) {
                    page -= 1;
                    shift += 8;
                }

                //超出屏幕的内容不显示
                if (page + j >= 0 && page + j < OLED_MAX_PAGE_NUM) {
                    /*显示图像在当前页的内容*/
                    oled_display_buf[x + i][page + j] |= img[j * width + i]
                                                         << (shift);
                }

                //超出屏幕的内容不显示
                if (page + j + 1 >= 0 && page + j + 1 < OLED_MAX_PAGE_NUM) {
                    /*显示图像在下一页的内容*/
                    oled_display_buf[x + i][page + j + 1] |=
                        img[j * width + i] >> (8 - shift);
                }
            }
        }
    }
}

/**
 * @brief 显示一个字符
 * 
 * @param x 0 ~ 127
 * @param y 0 ~ 63 
 * @param ch 
 * @param font_size 指定字体大小
 */
void oled_display_char(u8 x, u8 y, u8 ch, oled_font_size_type_t font_size_type)
{
    if (ch < ' ' || ch > '~') {
        // 超出了字库的范围，不显示
        return;
    }

    if (OLED_FONT_SIZE_TYPE_8X16 == font_size_type) {
        oled_display_img(x, y, 8, 16, oled_ascii_font_8x16[ch - ' ']);
    } else if (OLED_FONT_SIZE_TYPE_6X8 == font_size_type) {
        oled_display_img(x, y, 6, 8, oled_ascii_font_6x8[ch - ' ']);
    }
}

void oled_display_string(u8 x, u8 y, const char *str,
                         oled_font_size_type_t font_size_type)
{
    while (*str != '\0') {
        if (OLED_FONT_SIZE_TYPE_8X16 == font_size_type) {
            if (((u16)x + 8) >= OLED_MAX_COLUMN_SiZE) {
                x = 0;
                y += 16;
            }
        } else if (OLED_FONT_SIZE_TYPE_6X8 == font_size_type) {
            if (((u16)x + 6) >= OLED_MAX_COLUMN_SiZE) {
                x = 0;
                y += 8;
            }
        } else {
            break;
        }

        if (x >= OLED_MAX_COLUMN_SiZE || y >= OLED_MAX_ROW_SIZE) {
            break;
        }

        oled_display_char(x, y, *str, font_size_type);
        if (OLED_FONT_SIZE_TYPE_8X16 == font_size_type) {
            x += 8;
        } else if (OLED_FONT_SIZE_TYPE_6X8 == font_size_type) {
            x += 6;
        }

        str++;
    }
}

void oled_init(void)
{
    soft_iic_init(0);

    oled_write_cmd(0xAE); // 设置显示开启/关闭，0xAE关闭，0xAF开启
    oled_write_cmd(0xD5); // 设置显示时钟分频比/振荡器频率
    oled_write_cmd(0x80); // 0x00~0xFF

    oled_write_cmd(0xA8); // 设置多路复用率
    oled_write_cmd(0x3F); // 0x0E~0x3F

    oled_write_cmd(0xD3); // 设置显示偏移
    oled_write_cmd(0x00); // 0x00~0x3F

    oled_write_cmd(0x40); // 设置显示开始行，0x40~0x7F

    oled_write_cmd(0xA1); // 设置左右方向，0xA1正常，0xA0左右反置
    oled_write_cmd(0xC8); // 设置上下方向，0xC8正常，0xC0上下反置
    oled_write_cmd(0xDA); // 设置COM引脚硬件配置
    oled_write_cmd(0x12);

    oled_write_cmd(0x81); // 设置对比度
    oled_write_cmd(0xCF); // 0x00~0xFF
    oled_write_cmd(0xD9); // 设置预充电周期
    oled_write_cmd(0xF1);
    oled_write_cmd(0xDB); // 设置VCOMH取消选择级别
    oled_write_cmd(0x30);
    oled_write_cmd(0xA4); // 设置整个显示打开/关闭
    oled_write_cmd(0xA6); // 设置正常/反色显示，0xA
    oled_write_cmd(0x8D); // 设置充电泵
    oled_write_cmd(0x14);
    oled_write_cmd(0xAF); // 开启显示

    oled_display_clear(); // 清屏
    oled_display_refresh();
    // 定位到坐标为 0，0 的地方，方便下次写入
    oled_set_cursor(0, 0);
}

void oled_display_refresh_10ms_isr(void)
{
    if (oled_display_refresh_cnt < ((u8)-1)) {
        oled_display_refresh_cnt++;
    }
}

void oled_display_refresh_handle(void)
{
    u8 buf[10];
    if (oled_display_refresh_cnt >= OLED_DISPLAY_REFRESH_TIME) {
        oled_display_refresh_cnt = 0;
        oled_display_string(0, 16 * 0, "TYPE", OLED_FONT_SIZE_TYPE_8X16);
        oled_display_string(0, 16 * 1, "RSSI", OLED_FONT_SIZE_TYPE_8X16);
        oled_display_string(0, 16 * 2, "KEY", OLED_FONT_SIZE_TYPE_8X16);

        if (REMOTER_TYPE_28KEY == rf24g_remoter_param.remoter_type) {
            oled_display_string(6 * 12, 16 * 0, "28", OLED_FONT_SIZE_TYPE_8X16);
        } else if (REMOTER_TYPE_24KEY == rf24g_remoter_param.remoter_type) {
            oled_display_string(6 * 12, 16 * 0, "24", OLED_FONT_SIZE_TYPE_8X16);
        } else {
            oled_display_string(6 * 12, 16 * 0, "  ", OLED_FONT_SIZE_TYPE_8X16);
        }

        // rssi :
        sprintf(buf, "%d", (int32_t)rf24g_remoter_param.rssi);
        // printf("buf == %s\n", buf);
        oled_display_string(6 * 12, 16 * 1, buf, OLED_FONT_SIZE_TYPE_8X16);
        // key:
        sprintf(buf, "0x%02x", (u16)rf24g_remoter_param.key_val);
        // printf("buf == %s\n", buf);
        oled_display_string(6 * 12, 16 * 2, buf, OLED_FONT_SIZE_TYPE_8X16);
        // sprintf(buf, "%s");

        if (rf24g_remoter_param.is_key_pass) {
            oled_display_string(6 * 8, 16 * 3, "PASS",
                                OLED_FONT_SIZE_TYPE_8X16);
        } else {
            oled_display_clear_area(6 * 8, 16 * 3, 8 * 4, 16);
        }

        oled_display_refresh();
    }
}