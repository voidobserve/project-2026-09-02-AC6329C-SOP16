#include "system/includes.h"
#include "indicator_light.h"
#include "buzzer.h"
#include "oled.h"

// 函数声明
extern void user_init(void);
extern void user_main(void);

void user_init(void)
{
    indicator_light_init();
    buzzer_init();
    oled_init();

    task_create(user_main, NULL, "user_task");
}

void user_main(void)
{
    while (1) {
        // printf("main loop\n");
        os_time_dly(1);
    }
}
