/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验21 背光PWM实验
其他	   : 我们使用手机的时候背光都是可以调节的，同样的ZERO-I.MX6U
		 开发板的LCD背光也是可以调节，LCD背光就相当于一个LED灯。
		 LED灯的亮灭可以通过PWM来控制，本实验我们就来学习一下如何
		 通过PWM来控制LCD的背光。
论坛 	   : www.wtmembed.com
日志	   : 初版V1.0 2019/1/21 左忠凯创建
**************************************************************/
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_rtc.h"
#include "bsp_backlight.h"
#include "stdio.h"


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char keyvalue = 0;
	unsigned char i = 0;
	unsigned char state = OFF;
	unsigned char duty = 0;
	
	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 					*/
	delay_init();				/* 初始化延时 					*/
	clk_enable();				/* 使能所有的时钟 					*/
	led_init();					/* 初始化led 					*/
	beep_init();				/* 初始化beep	 				*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 					*/
	backlight_init();			/* 初始化背光PWM			 		*/ 


	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ZERO-IMX6U BACKLIGHT PWM TEST");  
	lcd_show_string(50, 40, 400, 24, 24, (char*)"PWM Duty:   %");  
	tftlcd_dev.forecolor = LCD_BLUE;

	/* 设置默认占空比 10% */
	duty = 10;
	lcd_shownum(158, 40, duty, 3, 24);	
	pwm1_setduty(duty);	
	
	while(1)					
	{
		keyvalue = key_getvalue();
		if(keyvalue == KEY0_VALUE)
		{
			duty += 10;
			if(duty > 100)
				duty = 0;
			lcd_shownum(158, 40, duty, 3, 24);
			pwm1_setduty(duty);
		}
		
		delayms(10);
		i++;
		if(i == 50)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state);	
		}
	}
	return 0;
}
