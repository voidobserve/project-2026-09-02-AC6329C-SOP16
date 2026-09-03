#ifndef __INDICATOR_LIGHT_H__
#define __INDICATOR_LIGHT_H__

// 驱动指示灯的引脚
#define INDICATOR_LIGHT_PIN IO_PORTB_06
// 指示灯点亮时，引脚的电平：
#define INDICATOR_LIGHT_ON_LEV (1)

void indicator_light_init(void);
void indicator_light_on(void);
void indicator_light_off(void);

#endif
