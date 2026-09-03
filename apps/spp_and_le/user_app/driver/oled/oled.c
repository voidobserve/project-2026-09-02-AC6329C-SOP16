#include "oled.h"
#include <string.h> // memset
#include "includes.h"
#include "asm/iic_soft.h"

// TODO
typedef struct
{

} oled_param_t;

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
volatile u8 oled_video_memory[OLED_MAX_COLUMN_SiZE][OLED_MAX_PAGE_NUM];

const struct soft_iic_config soft_iic_cfg[] = {
    {
        .scl = OLED_SCL_PIN,
        .sda = OLED_SDA_PIN,
        .delay = 10,
        .io_pu = 0,
    },
};

void oled_send_cmd(u8 cmd)
{
    soft_iic_start(OLED_IIC_DEV_IDX);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x78);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x00);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, cmd);
    soft_iic_stop(OLED_IIC_DEV_IDX);
}

void oled_send_data(u8 data)
{
    soft_iic_start(OLED_IIC_DEV_IDX);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x78);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, 0x40);
    soft_iic_tx_byte(OLED_IIC_DEV_IDX, data);
    soft_iic_stop(OLED_IIC_DEV_IDX);
}

void oled_set_position(u8 column, u8 page)
{
    column += OLED_COLUMN_BASE;
    oled_send_cmd(page + OLED_PAGE_BASE);
    oled_send_cmd(0x0F & column);
    oled_send_cmd((column & 0xF0) >> 4 | OLED_COLUMN_BASE);
}

void oled_draw_point(u8 x, u8 y, u8 is_display)
{
    u8 page_idx;

    if (x >= OLED_MAX_COLUMN_SiZE || y >= OLED_MAX_ROW_SIZE) {
        return;
    }

    page_idx = y / 8;

    if (is_display) {
        oled_video_memory[x][page_idx] |= 0x01;
    } else {
        oled_video_memory[x][page_idx] &= ~0x01;
    }
}

// 更新显示
void oled_display_refresh(void)
{
    u8 i;
    u8 j;
    for (i = 0; i < OLED_MAX_PAGE_NUM; i++) {
        oled_set_position(0, i);
        for (j = 0; j < OLED_MAX_COLUMN_SiZE; j++) {
            oled_send_data(oled_video_memory[j][i]);
        }
    }
}

// 清空显示
void oled_display_clear(void)
{
    memset(oled_video_memory, 0, sizeof(oled_video_memory));
    oled_display_refresh();
}

void oled_init(void)
{
    soft_iic_init(0);

    //--display off 						 关闭显示(0xAE/0xAF 关闭/开启)
    oled_send_cmd(0xAE);
    //---set low column address 			 默认为 0
    oled_send_cmd(0x00);
    //									 [1:0]00:列地址模式;01:行地址;10:页地址模式;
    oled_send_cmd(0x02);
    //---set high column address
    oled_send_cmd(0x10);
    // 设置内存地址模式
    oled_send_cmd(0x20);
    //--set start line address			 设置显示开始行[5:0]
    oled_send_cmd(0x40);
    //--set page address
    oled_send_cmd(0xB0);
    // contract control					 设置对比度
    oled_send_cmd(0x81);
    //--128
    oled_send_cmd(0xFF);
    // set segment remap					 设置左右反置 (0xa0/0xa1 开/关)
    oled_send_cmd(0xA1);
    // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    oled_send_cmd(0xA4);
    //--normal / reverse					 设置正常显示(0xa6/0xa7  正常/不正常)
    oled_send_cmd(0xA6);
    //--set multiplex ratio(1 to 64) 		 设置驱动路数(1~64)
    oled_send_cmd(0xA8);
    //--1/32 duty 						 默认 0x3F	(1~64)
    oled_send_cmd(0x3F);
    // Com scan direction					 设置扫描方向 (0xc0/0xc8 上下翻转/正常)
    oled_send_cmd(0xC8);
    //-set display offset					 设置显示偏移
    oled_send_cmd(0xD3);

    oled_send_cmd(0x00); //
    // set osc division 					 设置时钟分频因子,震荡频率
    oled_send_cmd(0xD5);
    // [3:0],								 分频因子;[7:4],震荡频率
    oled_send_cmd(0x80);
    // set area color mode off
    oled_send_cmd(0xD8);

    oled_send_cmd(0x05); //
                         // Set Pre-Charge Period				 设置预充电周期
    oled_send_cmd(0xD9);
    //									 [3:0],PHASE 1;[7:4],PHASE 2;
    oled_send_cmd(0xF1);
    // set com pin configuartion			 设置硬件引脚配置
    oled_send_cmd(0xDA);
    //									 [5:4]配置
    oled_send_cmd(0x12);
    // set Vcomh							 设置VCOMH电压倍率
    oled_send_cmd(0xDB);
    // [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    oled_send_cmd(0x30);
    // set charge pump enable				 电荷泵设置
    oled_send_cmd(0x8D);
    // 									 开启/关闭 0x14/0x10
    oled_send_cmd(0x14);
    // 亮度调节0x00~0xFF (亮度设置,越大越亮)
    oled_send_cmd(0xFF);
    //--turn on oled panel				 开启显示
    oled_send_cmd(0xAF);

    // 清屏
    oled_display_clear();
    // 定位到坐标为 0，0 的地方，方便下次写入
    oled_set_position(0, 0);
}

void oled_display_refresh_handle(void)
{
    // TODO 刷新时间到来，再刷新
    // if ()
}