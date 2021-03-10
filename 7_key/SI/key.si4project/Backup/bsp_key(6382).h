#ifndef _BSP_KEY_H
#define _BSP_KEY_H
#include "imx6ul.h"
/*
 * IMX6UL LED延时文件
 * 作者:左忠凯
 */

/* 定义按键值 */
enum keyvalue{
	KEY0_VALUE = 1,
	KEY1_VALUE,
	KEY2_VALUE,
};

void key_init(void);
int key_getvalue(void);


#endif
