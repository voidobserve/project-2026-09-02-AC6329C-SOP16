#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "typedef.h"

// 驱动驱动蜂鸣器的引脚
#define BUZZER_PIN IO_PORTB_05
// 蜂鸣器鸣叫时，引脚的电平：
#define BUZZER_ON_LEV (0)

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

void buzzer_play(u8 beep_time);
void buzzer_pause(void);
void buzzer_handle_10ms_isr(void);

#endif
