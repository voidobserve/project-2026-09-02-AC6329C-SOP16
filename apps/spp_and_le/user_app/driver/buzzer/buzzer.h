#ifndef __BUZZER_H__
#define __BUZZER_H__

// 驱动驱动蜂鸣器的引脚
#define BUZZER_PIN IO_PORTB_05
// 蜂鸣器鸣叫时，引脚的电平：
#define BUZZER_ON_LEV (0)

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

#endif
