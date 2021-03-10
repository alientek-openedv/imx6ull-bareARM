#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_gpio.h
作者	   : 左忠凯
版本	   : V1.0
描述	   : 按键驱动文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/4 左忠凯创建
***************************************************************/

/*
 * IMX6UL LED延时文件
 * 作者:左忠凯
 */

/* 初始化按键
 * KEY0 ->  GPIO1_IO04
 * KEY1 ->	GPIO1_IO03
 * KEY2 ->	GPIO1_IO02
 */
void key_init(void)
{	
	gpio_pin_config_t key_config;
	
	/* 1、初始化IO复用 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO02_GPIO1_IO02,0);	/* 复用为GPIO1_IO02 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);	/* 复用为GPIO1_IO03 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO04_GPIO1_IO04,0);	/* 复用为GPIO1_IO04 */

	/* 2、、配置GPIO1_IO02、GPIO1_IO03和GPIO1_IO04的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 11 默认22K上拉
	 *bit [13]: 1 pull功能
	 *bit [12]: 1 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 000 关闭输出
	 *bit [0]: 0 低转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO02_GPIO1_IO02,0xF080);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0xF080);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO04_GPIO1_IO04,0xF080);
	
	/* 3、初始化GPIO */
	//GPIO1->GDIR &= ~((1 << 2)|| (1 << 3) || (1 << 4));	/* GPIO1_IO02,IO03和IO04都设置为输入 */	
	key_config.direction = kGPIO_DigitalInput;
	gpio_init(GPIO1,4, &key_config);
	gpio_init(GPIO1,3, &key_config);
	gpio_init(GPIO1,2, &key_config);
	
}

/* 获取按键值 
 * 返回值:0 没有按键按下，其他值:对应的按键值
 */
int key_getvalue(void)
{
	int ret = 0;

	delay(10);	//延时消抖
	if(gpio_pinread(GPIO1, 4) == 0) 		/* KEY0 */
		ret = KEY0_VALUE;
	else if(gpio_pinread(GPIO1, 3) == 0) 	/* KEY1 */
		ret = KEY1_VALUE;
	else if(gpio_pinread(GPIO1, 2) == 0)	/* KEY2 */
		ret = KEY2_VALUE;
	else
		ret = 0;
	
	return ret;
	
}
