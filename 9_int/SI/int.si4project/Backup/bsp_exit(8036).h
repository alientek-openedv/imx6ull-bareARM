#ifndef _BSP_EXIT_H
#define _BSP_EXIT_H
#include "imx6ul.h"
/*
 * IMX6UL 外部中断驱动文件
 * 作者:左忠凯
 */
 
void exit_init(void);						/* 中断初始化 */
void gpio1_io02_irqhandler(void); 			/* 中断处理函数 */

#endif
