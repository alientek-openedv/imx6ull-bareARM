#ifndef _BSP_EPITTIMER_H
#define _BSP_EPITTIMER_H
#include "imx6ul.h"
/*
 * IMX6UL EPIT定时器驱动文件
 * 作者:左忠凯
 */
 
void epit1_init(unsigned int value);
void epit1_irqhandler(void);

#endif
