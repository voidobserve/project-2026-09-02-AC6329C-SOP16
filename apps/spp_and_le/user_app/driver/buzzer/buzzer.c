#include "buzzer.h"
#include "includes.h"

static volatile u8 beep_time_cnt = 0;
static volatile u8 is_beep_enable = 0;

void buzzer_init(void)
{
    gpio_set_pull_down(BUZZER_PIN, 0);
    gpio_set_pull_up(BUZZER_PIN, 0);
    gpio_set_die(BUZZER_PIN, 1); // 关闭模拟输入通道
    gpio_set_hd(BUZZER_PIN, 0);  // 看需求是否需要开启强推,会导致芯片功耗大
    gpio_set_hd0(BUZZER_PIN, 0);
    gpio_set_direction(BUZZER_PIN, 0); // 输出

    buzzer_off();
}

void buzzer_on(void)
{
    gpio_set_output_value(BUZZER_PIN, BUZZER_ON_LEV);
}

void buzzer_off(void)
{
    gpio_set_output_value(BUZZER_PIN, !(BUZZER_ON_LEV));
}

/**
 * @brief 让蜂鸣器鸣叫一声，到时间后停止
 * 
 * @param beep_time 蜂鸣器鸣叫的时间，单位：10ms
 *
 */
void buzzer_play(u8 beep_time)
{
    // is_beep_enable = 0;
    beep_time_cnt = beep_time;
    is_beep_enable = 1;
}

void buzzer_pause(void)
{
    is_beep_enable = 0; 
    beep_time_cnt= 0;
}

void buzzer_handle_10ms_isr(void)
{
    if (is_beep_enable) {
        if (beep_time_cnt >= 10) {
            beep_time_cnt -= 10;
            buzzer_on();
        } else {
            beep_time_cnt = 0;
            buzzer_off();
            is_beep_enable = 0;
        }
    } else {
        buzzer_off();
        beep_time_cnt = 0;
    }
}