/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验3 STM32模式的LED灯开发
		 使用STM32开发模式来编写LED灯驱动，学习如何从STM32来转入
		 I.MX6U的裸机开发，同时也通过本实验了解STM32库的运行方式。
其他	   : 无
日志	   : 初版V1.0 2019/1/3 左忠凯创建
**************************************************************/
#include "ledc.h"
#include "imx6ul.h"

/*
 * IMX6UL开发板裸机例程3 LED STM32模式
 * 作者：zuozhongkai
 */

/*
 * 初始化时钟，使能所有的时钟
 */
void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

void led_init(void)
{
	/* 1、初始化IO复用 */
	IOMUX_SW_MUX->GPIO1_IO01 = 0X5;			/* 复用为GPIO1_IO01 */
	IOMUX_SW_MUX->UART3_RX_DATA = 0X5;		/* 复用为GPIO1_IO25 */	


	/* 2、、配置GPIO1_IO01和GPIO1_IO25的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
    IOMUX_SW_PAD->GPIO1_IO01 = 0X10B0;
	IOMUX_SW_PAD->UART3_RX_DATA = 0X10B0;


	/* 3、初始化GPIO */
	GPIO1->GDIR = 0X2000002;	/* GPIO1_IO01和IO25都设置为输出 */
	GPIO1->DR = 0X0;			/* 4、设置GPIO1_IO01和IO25输出低电平，打开LED1和LED2 */	
}

void led_on(void)
{
	/* 
	 * 将GPIO1_DR的bit1和bit25清零	 
	*/
	GPIO1->DR &= ~((1<<1)|(1<<25)); 
}

void led_off(void)
{
	/*    
	 * 将GPIO1_DR的bit1和bit25置1
	 */
	GPIO1->DR |= (1<<1)|(1<<25); 
}

/*
 * 短时间延时函数,在396Mhz的主频下
 * 延时时间大约为1ms
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 *延时函数
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}

int main(void)
{
	clk_enable();		//使能所有的时钟
	led_init();			//初始化led

	while(1)			//死循环
	{	
		led_off();		//打开LED
		delay(500);

		led_on();
		delay(500);
	}

	return 0;
}
