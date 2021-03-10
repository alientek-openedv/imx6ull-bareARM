#ifndef _BSP_KEYFILTER_H
#define _BSP_KEYFILTER_H
/*
 * IMX6UL 按键消抖处理
 * 作者:左忠凯
 */

void filterkey_init(void);
void filtertimer_init(unsigned int value);
void filtertimer_stop(void);
void filtertimer_restart(unsigned int value);
void filtertimer_irqhandler(void);
void gpio1_0_15_irqhandler(void);

#endif
