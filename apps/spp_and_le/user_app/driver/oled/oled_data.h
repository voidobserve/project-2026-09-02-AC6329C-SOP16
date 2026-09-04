#ifndef __OLED_DATA_H__
#define __OLED_DATA_H__

#include "typedef.h"

enum
{
	OLED_FONT_SIZE_TYPE_8X16 = 0,
	OLED_FONT_SIZE_TYPE_6X8 ,
};
typedef u8 oled_font_size_type_t;

/* ASCII 字模数据声明 */
extern const uint8_t oled_ascii_font_8x16[][16];
extern const uint8_t oled_ascii_font_6x8[][6];



#endif

