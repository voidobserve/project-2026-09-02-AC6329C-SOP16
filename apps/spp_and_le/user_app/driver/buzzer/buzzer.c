#include "buzzer.h"
#include "includes.h"

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