#include "rf24g_key.h"
#include "le/ble_api.h" // adv_report_t
#include "key_driver.h"

#if (RF24GKEY_ENABLE)

/*
    用 AK803-SOP16 写的遥控器
*/
const u8 rf24g_key_type_28keys_table[][RF34G_KEY_EVENT_MAX + 1] = {
    {RF24G_KEY_VAL_R1C1, RF24G_28_KEY_EVENT_R1C1_PRESS,
     RF24G_28_KEY_EVENT_R1C1_CLICK, RF24G_28_KEY_EVENT_R1C1_LONG,
     RF24G_28_KEY_EVENT_R1C1_HOLD, RF24G_28_KEY_EVENT_R1C1_LOOSE},
    {RF24G_KEY_VAL_R1C2, RF24G_28_KEY_EVENT_R1C2_PRESS,
     RF24G_28_KEY_EVENT_R1C2_CLICK, RF24G_28_KEY_EVENT_R1C2_LONG,
     RF24G_28_KEY_EVENT_R1C2_HOLD, RF24G_28_KEY_EVENT_R1C2_LOOSE},
    {RF24G_KEY_VAL_R1C3, RF24G_28_KEY_EVENT_R1C3_PRESS,
     RF24G_28_KEY_EVENT_R1C3_CLICK, RF24G_28_KEY_EVENT_R1C3_LONG,
     RF24G_28_KEY_EVENT_R1C3_HOLD, RF24G_28_KEY_EVENT_R1C3_LOOSE},
    {RF24G_KEY_VAL_R1C4, RF24G_28_KEY_EVENT_R1C4_PRESS,
     RF24G_28_KEY_EVENT_R1C4_CLICK, RF24G_28_KEY_EVENT_R1C4_LONG,
     RF24G_28_KEY_EVENT_R1C4_HOLD, RF24G_28_KEY_EVENT_R1C4_LOOSE},

    {RF24G_KEY_VAL_R2C1, RF24G_28_KEY_EVENT_R2C1_PRESS,
     RF24G_28_KEY_EVENT_R2C1_CLICK, RF24G_28_KEY_EVENT_R2C1_LONG,
     RF24G_28_KEY_EVENT_R2C1_HOLD, RF24G_28_KEY_EVENT_R2C1_LOOSE},
    {RF24G_KEY_VAL_R2C2, RF24G_28_KEY_EVENT_R2C2_PRESS,
     RF24G_28_KEY_EVENT_R2C2_CLICK, RF24G_28_KEY_EVENT_R2C2_LONG,
     RF24G_28_KEY_EVENT_R2C2_HOLD, RF24G_28_KEY_EVENT_R2C2_LOOSE},
    {RF24G_KEY_VAL_R2C3, RF24G_28_KEY_EVENT_R2C3_PRESS,
     RF24G_28_KEY_EVENT_R2C3_CLICK, RF24G_28_KEY_EVENT_R2C3_LONG,
     RF24G_28_KEY_EVENT_R2C3_HOLD, RF24G_28_KEY_EVENT_R2C3_LOOSE},
    {RF24G_KEY_VAL_R2C4, RF24G_28_KEY_EVENT_R2C4_PRESS,
     RF24G_28_KEY_EVENT_R2C4_CLICK, RF24G_28_KEY_EVENT_R2C4_LONG,
     RF24G_28_KEY_EVENT_R2C4_HOLD, RF24G_28_KEY_EVENT_R2C4_LOOSE},

    {RF24G_KEY_VAL_R3C1, RF24G_28_KEY_EVENT_R3C1_PRESS,
     RF24G_28_KEY_EVENT_R3C1_CLICK, RF24G_28_KEY_EVENT_R3C1_LONG,
     RF24G_28_KEY_EVENT_R3C1_HOLD, RF24G_28_KEY_EVENT_R3C1_LOOSE},
    {RF24G_KEY_VAL_R3C2, RF24G_28_KEY_EVENT_R3C2_PRESS,
     RF24G_28_KEY_EVENT_R3C2_CLICK, RF24G_28_KEY_EVENT_R3C2_LONG,
     RF24G_28_KEY_EVENT_R3C2_HOLD, RF24G_28_KEY_EVENT_R3C2_LOOSE},
    {RF24G_KEY_VAL_R3C3, RF24G_28_KEY_EVENT_R3C3_PRESS,
     RF24G_28_KEY_EVENT_R3C3_CLICK, RF24G_28_KEY_EVENT_R3C3_LONG,
     RF24G_28_KEY_EVENT_R3C3_HOLD, RF24G_28_KEY_EVENT_R3C3_LOOSE},
    {RF24G_KEY_VAL_R3C4, RF24G_28_KEY_EVENT_R3C4_PRESS,
     RF24G_28_KEY_EVENT_R3C4_CLICK, RF24G_28_KEY_EVENT_R3C4_LONG,
     RF24G_28_KEY_EVENT_R3C4_HOLD, RF24G_28_KEY_EVENT_R3C4_LOOSE},

    {RF24G_KEY_VAL_R4C1, RF24G_28_KEY_EVENT_R4C1_PRESS,
     RF24G_28_KEY_EVENT_R4C1_CLICK, RF24G_28_KEY_EVENT_R4C1_LONG,
     RF24G_28_KEY_EVENT_R4C1_HOLD, RF24G_28_KEY_EVENT_R4C1_LOOSE},
    {RF24G_KEY_VAL_R4C2, RF24G_28_KEY_EVENT_R4C2_PRESS,
     RF24G_28_KEY_EVENT_R4C2_CLICK, RF24G_28_KEY_EVENT_R4C2_LONG,
     RF24G_28_KEY_EVENT_R4C2_HOLD, RF24G_28_KEY_EVENT_R4C2_LOOSE},
    {RF24G_KEY_VAL_R4C3, RF24G_28_KEY_EVENT_R4C3_PRESS,
     RF24G_28_KEY_EVENT_R4C3_CLICK, RF24G_28_KEY_EVENT_R4C3_LONG,
     RF24G_28_KEY_EVENT_R4C3_HOLD, RF24G_28_KEY_EVENT_R4C3_LOOSE},
    {RF24G_KEY_VAL_R4C4, RF24G_28_KEY_EVENT_R4C4_PRESS,
     RF24G_28_KEY_EVENT_R4C4_CLICK, RF24G_28_KEY_EVENT_R4C4_LONG,
     RF24G_28_KEY_EVENT_R4C4_HOLD, RF24G_28_KEY_EVENT_R4C4_LOOSE},

    {RF24G_KEY_VAL_R5C1, RF24G_28_KEY_EVENT_R5C1_PRESS,
     RF24G_28_KEY_EVENT_R5C1_CLICK, RF24G_28_KEY_EVENT_R5C1_LONG,
     RF24G_28_KEY_EVENT_R5C1_HOLD, RF24G_28_KEY_EVENT_R5C1_LOOSE},
    {RF24G_KEY_VAL_R5C2, RF24G_28_KEY_EVENT_R5C2_PRESS,
     RF24G_28_KEY_EVENT_R5C2_CLICK, RF24G_28_KEY_EVENT_R5C2_LONG,
     RF24G_28_KEY_EVENT_R5C2_HOLD, RF24G_28_KEY_EVENT_R5C2_LOOSE},
    {RF24G_KEY_VAL_R5C3, RF24G_28_KEY_EVENT_R5C3_PRESS,
     RF24G_28_KEY_EVENT_R5C3_CLICK, RF24G_28_KEY_EVENT_R5C3_LONG,
     RF24G_28_KEY_EVENT_R5C3_HOLD, RF24G_28_KEY_EVENT_R5C3_LOOSE},
    {RF24G_KEY_VAL_R5C4, RF24G_28_KEY_EVENT_R5C4_PRESS,
     RF24G_28_KEY_EVENT_R5C4_CLICK, RF24G_28_KEY_EVENT_R5C4_LONG,
     RF24G_28_KEY_EVENT_R5C4_HOLD, RF24G_28_KEY_EVENT_R5C4_LOOSE},

    {RF24G_KEY_VAL_R6C1, RF24G_28_KEY_EVENT_R6C1_PRESS,
     RF24G_28_KEY_EVENT_R6C1_CLICK, RF24G_28_KEY_EVENT_R6C1_LONG,
     RF24G_28_KEY_EVENT_R6C1_HOLD, RF24G_28_KEY_EVENT_R6C1_LOOSE},
    {RF24G_KEY_VAL_R6C2, RF24G_28_KEY_EVENT_R6C2_PRESS,
     RF24G_28_KEY_EVENT_R6C2_CLICK, RF24G_28_KEY_EVENT_R6C2_LONG,
     RF24G_28_KEY_EVENT_R6C2_HOLD, RF24G_28_KEY_EVENT_R6C2_LOOSE},
    {RF24G_KEY_VAL_R6C3, RF24G_28_KEY_EVENT_R6C3_PRESS,
     RF24G_28_KEY_EVENT_R6C3_CLICK, RF24G_28_KEY_EVENT_R6C3_LONG,
     RF24G_28_KEY_EVENT_R6C3_HOLD, RF24G_28_KEY_EVENT_R6C3_LOOSE},
    {RF24G_KEY_VAL_R6C4, RF24G_28_KEY_EVENT_R6C4_PRESS,
     RF24G_28_KEY_EVENT_R6C4_CLICK, RF24G_28_KEY_EVENT_R6C4_LONG,
     RF24G_28_KEY_EVENT_R6C4_HOLD, RF24G_28_KEY_EVENT_R6C4_LOOSE},

    {RF24G_KEY_VAL_R7C1, RF24G_28_KEY_EVENT_R7C1_PRESS,
     RF24G_28_KEY_EVENT_R7C1_CLICK, RF24G_28_KEY_EVENT_R7C1_LONG,
     RF24G_28_KEY_EVENT_R7C1_HOLD, RF24G_28_KEY_EVENT_R7C1_LOOSE},
    {RF24G_KEY_VAL_R7C2, RF24G_28_KEY_EVENT_R7C2_PRESS,
     RF24G_28_KEY_EVENT_R7C2_CLICK, RF24G_28_KEY_EVENT_R7C2_LONG,
     RF24G_28_KEY_EVENT_R7C2_HOLD, RF24G_28_KEY_EVENT_R7C2_LOOSE},
    {RF24G_KEY_VAL_R7C3, RF24G_28_KEY_EVENT_R7C3_PRESS,
     RF24G_28_KEY_EVENT_R7C3_CLICK, RF24G_28_KEY_EVENT_R7C3_LONG,
     RF24G_28_KEY_EVENT_R7C3_HOLD, RF24G_28_KEY_EVENT_R7C3_LOOSE},
    {RF24G_KEY_VAL_R7C4, RF24G_28_KEY_EVENT_R7C4_PRESS,
     RF24G_28_KEY_EVENT_R7C4_CLICK, RF24G_28_KEY_EVENT_R7C4_LONG,
     RF24G_28_KEY_EVENT_R7C4_HOLD, RF24G_28_KEY_EVENT_R7C4_LOOSE},
};

volatile u8 rf24g_key_driver_event = 0; // 由 key_driver_scan() 更新
volatile u8 rf24g_key_driver_value = 0; // 由 key_driver_scan() 更新

static volatile u8 rf24g_rx_flag = 0;       // 是否收到了新的数据
volatile rf24g_recv_info_t rf24g_recv_info; // 存放接收到的数据包
volatile u8 chromatic_circle_val = 0; // 存放色环按键对应的数值，范围：0x00~0xFF
volatile u8 rf24g_key_val = NO_KEY;   // 存放按键键值

static u8 rf24g_get_key_value(void); // 获取按键键值的函数声明
volatile struct key_driver_para rf24g_scan_para = {
    .scan_time = RF24G_KEY_SCAN_TIME_MS, // 按键扫描频率, 单位: ms
    .last_key = NO_KEY, // 上一次get_value按键值, 初始化为NO_KEY;
    .filter_time = RF24G_KEY_SCAN_FILTER_TIME_MS, // 按键消抖延时;
    .long_time =
        RF24G_KEY_LONG_TIME_MS / RF24G_KEY_SCAN_TIME_MS, // 按键判定长按数量
    .hold_time = (RF24G_KEY_LONG_TIME_MS + RF24G_KEY_HOLD_TIME_MS) /
                 RF24G_KEY_SCAN_TIME_MS, // 按键判定HOLD数量
    .click_delay_time =
        RF24G_KEY_SCAN_CLICK_DELAY_TIME_MS, // 按键被抬起后等待连击延时数量
    .key_type = KEY_DRIVER_TYPE_RF24GKEY,
    .get_value = rf24g_get_key_value,
};

// 底层按键扫描，由 __resolve_adv_report() 调用
void rf24g_scan(adv_report_t *adv_report )
{
    //     rf24g_recv_info_t *p = (rf24g_recv_info_t *)recv_buff;
    //     if (p->header1 == REMOTE_TYPE_28KEY_HEADER_1 &&
    //         p->header2 == REMOTE_TYPE_28KEY_HEADER_2) {
    // #if USER_DEBUG_ENABLE
    //         // printf_buf(recv_buff, sizeof(rf24g_recv_info_t)); // 打印接收到的数据包
    // #endif

    //         rf24g_key_val = p->key_val;
    //         rf24g_rx_flag = 1;
    //     }

    u8 ad_type; //
    u8 key_val; // 键值
    s8 rssi;    // 信号强度， -127 ~ 128 dbm
    u8 header_1;
    u8 header_2;

    if (adv_report->length < 10) {
        return;
    }

    // printf_buf(adv_report->data, adv_report->length);

    header_1 = adv_report->data[5];
    header_2 = adv_report->data[6];
    if (!((header_1 == REMOTE_TYPE_28KEY_HEADER_1 &&
           header_2 == REMOTE_TYPE_28KEY_HEADER_2) ||
          (header_1 == REMOTE_TYPE_24KEY_HEADER_1 &&
           header_2 == REMOTE_TYPE_24KEY_HEADER_2))) {
        // 遥控器的格式头不符合，直接返回
        return;
    }

    ad_type = adv_report->data[4];
    rssi = adv_report->rssi;
    key_val = adv_report->data[7];

    // printf("ad_type == %u\n", ad_type);
    printf("rssi == %d\n", (int)rssi);
    // printf("header_1 == %02x\n", (u16)header_1);
    // printf("header_2 == %02x\n", (u16)header_2);
    printf("key_val == %02x\n", (u16)key_val);

    // TODO 


}

static u8 rf24g_get_key_value(void)
{
    u8 key_value = 0;
    static u16 time_out_cnt =
        0; // 加入超时，防止丢包（超时时间与按键扫描时间有关）
    static u8 last_key_value =
        0; // 上一次按键键值，在超时时间内返回上一次按键键值

    if (rf24g_rx_flag == 1) // 收到2.4G广播
    {
        rf24g_rx_flag = 0;

        key_value = rf24g_key_val;

        /*
            2.4G接收可能会丢失100~200ms的数据包（响应会慢一些）
            值 == 20，10ms调用一次该函数，这里填充200ms的超时值
        */
        time_out_cnt = 20;
        // time_out_cnt = 5;

        last_key_value = key_value;
        return key_value;
    }

    if (time_out_cnt != 0) {
        time_out_cnt--;
        return last_key_value;
    }

    return NO_KEY;
}

// 根据按键键值和key_driver_scan得到的事件值，转换为对应的按键事件
u8 rf24g_convert_key_event(u8 key_value, u8 key_driver_event)
{
    // 将key_driver_scan得到的key_event转换成自定义的key_event对应的索引
    // 索引对应 rf24g_key_event_table[][] 中的索引
    u8 key_event_index = 0; // 默认为0，0对应无效索引
    if (KEY_EVENT_PRESS == key_driver_event) {
        key_event_index = 1;
    } else if (KEY_EVENT_CLICK == key_driver_event) {
        key_event_index = 2;
    } else if (KEY_EVENT_LONG == key_driver_event) {
        key_event_index = 3;
    } else if (KEY_EVENT_HOLD == key_driver_event) {
        key_event_index = 4;
    } else if (KEY_EVENT_UP == key_driver_event) {
        // 长按后松手
        key_event_index = 5;
    }

    if (0 == key_event_index || NO_KEY == key_value) {
        // 按键事件与上面的事件都不匹配
        // 得到的键值是无效键值
        return RF24G_28_KEY_EVENT_NONE;
    }

    // 遍历表格中的每一个按键：
    for (u8 i = 0; i < sizeof(rf24g_key_type_28keys_table) /
                           sizeof(rf24g_key_type_28keys_table[0]);
         i++) {
        if (key_value == rf24g_key_type_28keys_table[i][0]) {
            return rf24g_key_type_28keys_table[i][key_event_index];
        }
    }

    // 如果运行到这里，都没有找到对应的按键，返回无效按键事件
    return RF24G_28_KEY_EVENT_NONE;
}

void rf24_key_handle(void)
{
    u8 rf24g_key_event = 0;
    rf24_key_handle_func_t rf24g_key_handle_func_ptr = NULL;

    rf24g_key_event =
        rf24g_convert_key_event(rf24g_key_driver_value, rf24g_key_driver_event);
    rf24g_key_driver_value =
        NO_KEY; // 置为无效键值（由于扫描函数只更新，不会清除，在这里要清除）

    if (rf24g_key_event == RF24G_28_KEY_EVENT_NONE) {
        // 如果是无效的按键事件，直接返回
        return;
    }

    rf24g_key_handle_func_ptr = rf24_28keys_handle_func_buff[rf24g_key_event];

    if (NULL == rf24g_key_handle_func_ptr) {
        // 如果按键事件没有对应的处理函数，直接退出
        return;
    }

    // 直接调用对应的处理函数
    rf24g_key_handle_func_ptr();
}

// =============================================================================

void rf24g_28keys_event_r1c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    printf("28keys event r1c1\n");
#endif
}

void rf24g_28keys_event_r1c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    printf("28keys event r1c2\n");
#endif
}

void rf24g_28keys_event_r1c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    printf("28keys event r1c3\n");
#endif
}

void rf24g_28keys_event_r1c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    printf("28keys event r1c4\n");
#endif
}

void rf24g_28keys_event_r2c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r2c1\n");
#endif
}

void rf24g_28keys_event_r2c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r2c2\n");
#endif
}

void rf24g_28keys_event_r2c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r2c3\n");
#endif
}

void rf24g_28keys_event_r2c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r2c4\n");
#endif
}

void rf24g_28keys_event_r3c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r3c1\n");
#endif
}

void rf24g_28keys_event_r3c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r3c2\n");
#endif
}

void rf24g_28keys_event_r3c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r3c3\n");
#endif
}

void rf24g_28keys_event_r3c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r3c4\n");
#endif
}

void rf24g_28keys_event_r4c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r4c1\n");
#endif
}

void rf24g_28keys_event_r4c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r4c2\n");
#endif
}

void rf24g_28keys_event_r4c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r4c3\n");
#endif
}

void rf24g_28keys_event_r4c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r4c4\n");
#endif
}

void rf24g_28keys_event_r5c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r5c1\n");
#endif
}

void rf24g_28keys_event_r5c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r5c2\n");
#endif
}

void rf24g_28keys_event_r5c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r5c3\n");
#endif
}

void rf24g_28keys_event_r5c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r5c4\n");
#endif
}

void rf24g_28keys_event_r6c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r6c1\n");
#endif
}

void rf24g_28keys_event_r6c2_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r6c2\n");
#endif
}

void rf24g_28keys_event_r6c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r6c3\n");
#endif
}

void rf24g_28keys_event_r6c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r6c4\n");
#endif
}

void rf24g_28keys_event_r7c1_click_handle(void)
{
#if USER_DEBUG_ENABLE
    printf("28keys event r7c1\n");
#endif
}

void rf24g_28keys_event_r7c3_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r7c3\n");
#endif
}

void rf24g_28keys_event_r7c4_click_handle(void)
{
#if USER_DEBUG_ENABLE
    // printf("28keys event r7c4\n");
#endif
}

const rf24_key_handle_func_t rf24_28keys_handle_func_buff[RF24G_28_KEY_EVENT_MAX] = {
    [RF24G_28_KEY_EVENT_R1C1_PRESS] =
        rf24g_28keys_event_r1c1_click_handle, // 按键事件处理函数

    [RF24G_28_KEY_EVENT_R1C2_PRESS] = rf24g_28keys_event_r1c2_click_handle,

    [RF24G_28_KEY_EVENT_R1C3_PRESS] = rf24g_28keys_event_r1c3_click_handle,

    [RF24G_28_KEY_EVENT_R1C4_PRESS] = rf24g_28keys_event_r1c4_click_handle,
    // =======================================================================

    [RF24G_28_KEY_EVENT_R2C1_PRESS] = rf24g_28keys_event_r2c1_click_handle,

    [RF24G_28_KEY_EVENT_R2C2_PRESS] = rf24g_28keys_event_r2c2_click_handle,

    [RF24G_28_KEY_EVENT_R2C3_PRESS] = rf24g_28keys_event_r2c3_click_handle,

    [RF24G_28_KEY_EVENT_R2C4_PRESS] = rf24g_28keys_event_r2c4_click_handle,

    // =======================================================================
    [RF24G_28_KEY_EVENT_R3C1_PRESS] = rf24g_28keys_event_r3c1_click_handle,

    [RF24G_28_KEY_EVENT_R3C2_PRESS] = rf24g_28keys_event_r3c2_click_handle,

    [RF24G_28_KEY_EVENT_R3C3_PRESS] = rf24g_28keys_event_r3c3_click_handle,

    [RF24G_28_KEY_EVENT_R3C4_PRESS] = rf24g_28keys_event_r3c4_click_handle,
    // =======================================================================
    [RF24G_28_KEY_EVENT_R4C1_PRESS] = rf24g_28keys_event_r4c1_click_handle,

    [RF24G_28_KEY_EVENT_R4C2_PRESS] = rf24g_28keys_event_r4c2_click_handle,

    [RF24G_28_KEY_EVENT_R4C3_PRESS] = rf24g_28keys_event_r4c3_click_handle,

    [RF24G_28_KEY_EVENT_R4C4_PRESS] = rf24g_28keys_event_r4c4_click_handle,
    // =======================================================================
    [RF24G_28_KEY_EVENT_R5C1_PRESS] = rf24g_28keys_event_r5c1_click_handle,

    [RF24G_28_KEY_EVENT_R5C2_PRESS] = rf24g_28keys_event_r5c2_click_handle,

    [RF24G_28_KEY_EVENT_R5C3_PRESS] = rf24g_28keys_event_r5c3_click_handle,

    [RF24G_28_KEY_EVENT_R5C4_PRESS] = rf24g_28keys_event_r5c4_click_handle,
    // =======================================================================
    [RF24G_28_KEY_EVENT_R6C1_PRESS] = rf24g_28keys_event_r6c1_click_handle,

    [RF24G_28_KEY_EVENT_R6C2_PRESS] = rf24g_28keys_event_r6c2_click_handle,

    [RF24G_28_KEY_EVENT_R6C3_PRESS] = rf24g_28keys_event_r6c3_click_handle,

    [RF24G_28_KEY_EVENT_R6C4_PRESS] = rf24g_28keys_event_r6c4_click_handle,
    // =======================================================================
    [RF24G_28_KEY_EVENT_R7C1_PRESS] = rf24g_28keys_event_r7c1_click_handle,

    // [RF24G_28_KEY_EVENT_R7C2_PRESS] = rf24g_28keys_event_r7c2_click_handle,

    [RF24G_28_KEY_EVENT_R7C3_PRESS] = rf24g_28keys_event_r7c3_click_handle,

    [RF24G_28_KEY_EVENT_R7C4_PRESS] = rf24g_28keys_event_r7c4_click_handle,

};

#endif // RF24GKEY_ENABLE
