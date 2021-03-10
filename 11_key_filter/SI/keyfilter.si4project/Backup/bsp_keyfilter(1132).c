#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_led.h"
#include "bsp_keyfilter.h"

/* 初始化按键
 * KEY0 ->  GPIO1_IO04
 * KEY1 ->	GPIO1_IO03
 * KEY2 ->	GPIO1_IO02
 */
void filterkey_init(void)
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
	
	/* 3、初始化GPIO为中断 */

	key_config.direction = kGPIO_DigitalInput;
	key_config.interruptMode = kGPIO_IntFallingEdge;
	key_config.outputLogic = 1;
	gpio_init(GPIO1,4, &key_config);
	gpio_init(GPIO1,3, &key_config);
	gpio_init(GPIO1,2, &key_config);

	GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);					//使能GIC中对应的中断
	system_register_irqhandler(GPIO1_Combined_0_15_IRQn, (system_irq_handler_t)gpio1_0_15_irqhandler, NULL);	//注册中断服务函数
	gpio_enableint(GPIO1, 1 << 2);	//使能GPIO1_IO02的中断功能
	gpio_enableint(GPIO1, 1 << 3);	//使能GPIO1_IO03的中断功能
	gpio_enableint(GPIO1, 1 << 4);	//使能GPIO1_IO04的中断功能

	filtertimer_init(66000000/100);//初始化定时器,10ms
	
}


/*
 * 初始化用于消抖的定时器
 * 默认关闭定时器
 */
void filtertimer_init(unsigned int value)
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
	system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)filtertimer_irqhandler, NULL);	//注册中断服务函数		

}


/*
 * 关闭定时器
 */
void filtertimer_stop(void)
{
	EPIT1->CR &= ~(1<<0);	//关闭定时器
}

/*
 * 重启定时器
 */
void filtertimer_restart(unsigned int value)
{
	EPIT1->CR &= ~(1<<0);	//先关闭定时器
	EPIT1->LR = value;		//计数值
	EPIT1->CR |= (1<<0);	//打开定时器
}


/* 
 *中断处理函数 
 */
void filtertimer_irqhandler(void)
{ 
	static unsigned char state = OFF;

	if(EPIT1->SR & (1<<0)) //判断比较事件发生
	{
		filtertimer_stop();	//关闭定时器
		if(gpio_pinread(GPIO1, 4) == 0) 		/* KEY0 */
		{
			state = ON;
			led_switch(LED2,state);	//打开LED2
		}
		else if(gpio_pinread(GPIO1, 3) == 0) 	/* KEY1 */
		{
			state = OFF;
			led_switch(LED2,state); //关闭LED2
		}
		else if(gpio_pinread(GPIO1, 2) == 0)
		{
			state = !state;
			led_switch(LED2,state); //打开LED2闪烁
		}
	}
	
	EPIT1->SR |= 1<<0; /* 清除中断标志位 */
}


/* 
 * gpio中断
 */
void gpio1_0_15_irqhandler(void)
{ 
	filtertimer_restart(66000000/100);//开启定时器

	/* 清除中断标志位 */
	gpio_clearintflags(GPIO1, 1 << 2);
	gpio_clearintflags(GPIO1, 1 << 3); 
	gpio_clearintflags(GPIO1, 1 << 4);
}

