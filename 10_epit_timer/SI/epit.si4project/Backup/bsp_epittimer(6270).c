#include "bsp_epittimer.h"
#include "bsp_int.h"
#include "bsp_led.h"
/*
 * IMX6UL EPIT定时器驱动文件
 * 作者:左忠凯
 */

/* 初始化 EPIT定时器,EPIT定时器是32位向下计数器
 * 时钟源使用ipg=66Mhz
 * value：倒计数值,比如要定时500ms, 那么就要设置为
 *
 */
void epit1_init(unsigned int value)
{
	EPIT1->CR = 0;	//先清零
	
	/*
     * CR寄存器:
     * bit25:24 01 时钟源选择Peripheral clock=66MHz
     * bit15:4  0  1分频
     * bit3:	1  当计数器到0的话从LR重新加载数值
     * bit2:	1  比较中断使能
     * bit1:    1  初始计数值来源于LR寄存器值
     * bit0:    0  先关闭EPIT1
     */
	EPIT1->CR = (1<<24 | 1<<3 | 1<<2 | 1<<1);
	EPIT1->LR = value;	//计数值
	EPIT1->CMPR	= 0;	//比较寄存器，当计数器值和此寄存器值相等的话就会产生中断
	
	GIC_EnableIRQ(EPIT1_IRQn);	//使能GIC中对应的中断
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);	//注册中断服务函数	

	EPIT1->CR |= 1<<0;	//使能EPIT1
}


/* 中断处理函数 */
void epit1_irqhandler(void)
{ 
	static unsigned char state = 0;

	state = !state;
	if(EPIT1->SR & (1<<0)) //判断比较事件发生
	{
		led_switch(LED2, state);
	}
	
	EPIT1->SR |= 1<<0; /* 清除中断标志位 */
}

