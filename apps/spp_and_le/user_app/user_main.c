#include "system/includes.h"
#include "indicator_light.h"
#include "buzzer.h"
#include "oled.h"
#include "rf24g_key.h"

// 函数声明
extern void user_init(void);
extern void user_main(void);
extern void user_10ms_isr(void);

void user_10ms_isr(void)
{
    buzzer_handle_10ms_isr();
    oled_display_refresh_10ms_isr();
}

void user_init(void)
{
    indicator_light_init();
    buzzer_init();
    oled_init();

    sys_s_hi_timer_add(NULL, user_10ms_isr, 10);
    task_create(user_main, NULL, "user_task");
}

void user_main(void)
{
    u8 i;
    while (1) {

        if (rf24g_remoter_param.is_update) {
            rf24g_remoter_param.is_update = 0;
            // USER_TO_DO is_key_pass 到了LED显示可能会变成0
            rf24g_remoter_param.is_key_pass = 0;
            for (i = 0; i < ARRAY_SIZE(rf24g_key_val_table); i++) {
                if (rf24g_key_val_table[i] == rf24g_remoter_param.key_val) {
                    rf24g_remoter_param.is_key_pass = 1;
                    break;
                }
            }

            if (rf24g_remoter_param.is_key_pass) {
                buzzer_play(12);
                indicator_light_on();
            } else {
                buzzer_pause();
                indicator_light_off();
            }
        }

        oled_display_refresh_handle();
        os_time_dly(1);
    }
}
