#include "bsp_gpio.h"
/*
 * IMX6UL GPIO操作函数文件
 * 作者:左忠凯
 */

/*
 * GPIO初始化函数
*/
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
	if(config->direction == kGPIO_DigitalInput) /* GPIO作为输入 */
	{
		base->GDIR &= ~( 1 << pin);
	}
	else	/* 输出 */
	{
		base->GDIR |= 1 << pin;
		gpio_pinwrite(base,pin, config->outputLogic);	/* 设置默认输出电平 */
	}
}


 /* 读取GPIO值 
  * gpio:要读取的GPIO所在组
  * pin:要读取的GPIO的脚号
  */
 int gpio_pinread(GPIO_Type *base, int pin)
 {
	 return (((base->DR) >> pin) & 0x1);
 }


 /* GPIO输出高或者低电平
  * gpio:要读取的GPIO所在组
  * pin:要读取的GPIO的脚号
  * value:要输出的电平,1:输出高电平，0 输出低电平
  */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
	 if (value == 0U)
	 {
		 base->DR &= ~(1U << pin); /* 输出低电平 */
	 }
	 else
	 {
		 base->DR |= (1U << pin); /* 输出高电平 */
	 }
}



