#include "indicator_light.h"
#include "includes.h"

void indicator_light_init(void)
{
    gpio_set_pull_down(INDICATOR_LIGHT_PIN, 0);
    gpio_set_pull_up(INDICATOR_LIGHT_PIN, 0);
    gpio_set_die(INDICATOR_LIGHT_PIN, 1); // 关闭模拟输入通道
    gpio_set_hd(INDICATOR_LIGHT_PIN,
                0); // 看需求是否需要开启强推,会导致芯片功耗大
    gpio_set_hd0(INDICATOR_LIGHT_PIN, 0);
    gpio_set_direction(INDICATOR_LIGHT_PIN, 0); // 输出

    indicator_light_off();
}

void indicator_light_on(void)
{
    gpio_set_output_value(INDICATOR_LIGHT_PIN, INDICATOR_LIGHT_ON_LEV);
}

void indicator_light_off(void)
{
    gpio_set_output_value(INDICATOR_LIGHT_PIN, !(INDICATOR_LIGHT_ON_LEV));
}
