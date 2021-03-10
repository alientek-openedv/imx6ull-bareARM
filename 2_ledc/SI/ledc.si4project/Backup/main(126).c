#include "ledc.h"
/*
 * IMX6UL开发板裸机例程2 LED C语言版本
 * 作者：zuozhongkai
 */

/*
 * 初始化时钟，使能所有的时钟
 */
void clk_enable(void)
{
	CCM_CCGR0 = 0xffffffff;
	CCM_CCGR1 = 0xffffffff;
	CCM_CCGR2 = 0xffffffff;
	CCM_CCGR3 = 0xffffffff;
	CCM_CCGR4 = 0xffffffff;
	CCM_CCGR5 = 0xffffffff;
	CCM_CCGR6 = 0xffffffff;
}

void led_init(void)
{
	/* 1、初始化IO复用 */
	SW_MUX_GPIO1_IO01 = 0x5;	/* 复用为GPIO1_IO01 */
	SW_MUX_UART3_RXDATA =0x5; 	/* 复用为GPIO1_IO25 */

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
	SW_PAD_GPIO1_IO01 = 0X10B0;		
	SW_PAD_UART3_RXDATA = 0X10B0;	

	/* 3、初始化GPIO */
	GPIO1_GDIR = 0X2000002;	/* GPIO1_IO01和IO25都设置为输出 */

	/* 4、设置GPIO1_IO01和IO25输出低电平，打开LED1和LED2 */
	GPIO1_DR = 0X0;
}

void led_on(void)
{
	/* 
	 * 将GPIO1_DR的bit1和bit25清零	 
	*/
	GPIO1_DR &= ~((1<<1)|(1<<25)); 
}

void led_off(void)
{
	/*    
	 * 将GPIO1_DR的bit1和bit25置1
	 */
	GPIO1_DR |= (1<<1)|(1<<25);
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
