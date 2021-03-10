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
#include "bsp_touchscreen.h"
#include "stdio.h"


/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char i = 0;
	unsigned char state = OFF;

	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 					*/
	delay_init();				/* 初始化延时 					*/
	clk_enable();				/* 使能所有的时钟 					*/
	led_init();					/* 初始化led 					*/
	beep_init();				/* 初始化beep	 				*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 					*/
	touchscreen_init();			/* 初始化触摸屏			 		*/ 


	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ZERO-IMX6U TOUCH SCREEN TEST");  

	lcd_show_string(50, 40, 400, 24, 24, 	(char*)"TP Num	:");  
	lcd_show_string(50, 70, 200, 24, 24, 	(char*)"Point0 X:");  
	lcd_show_string(50, 100, 200, 24, 24, 	(char*)"Point0 Y:");  
	lcd_show_string(50, 130, 200, 24, 24, 	(char*)"Point1 X:");  
	lcd_show_string(50, 160, 200, 24, 24, 	(char*)"Point1 Y:");  
	lcd_show_string(50, 190, 200, 24, 24, 	(char*)"Point2 X:");  
	lcd_show_string(50, 220, 200, 24, 24, 	(char*)"Point2 Y:");  
	lcd_show_string(50, 250, 200, 24, 24, 	(char*)"Point3 X:");  
	lcd_show_string(50, 280, 200, 24, 24, 	(char*)"Point3 Y:");  
	lcd_show_string(50, 310, 200, 24, 24, 	(char*)"Point4 X:");  
	lcd_show_string(50, 340, 200, 24, 24, 	(char*)"Point4 Y:");  
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{
	#if 0
		/* 中断发生即表示有触摸发生，处理触摸 */
		if(ft5426_dev.intflag == 1)
		{
			ft5426_read_tpcoord();
			lcd_shownum(154, 40, ft5426_dev.point_num , 1, 24);
			lcd_shownum(154, 70, ft5426_dev.x[0], 5, 24);
			lcd_shownum(154, 100, ft5426_dev.y[0], 5, 24);
			lcd_shownum(154, 160, ft5426_dev.x[1], 5, 24);
			lcd_shownum(154, 130, ft5426_dev.y[1], 5, 24);
			lcd_shownum(154, 190, ft5426_dev.x[2], 5, 24);
			lcd_shownum(154, 220, ft5426_dev.y[2], 5, 24);
			lcd_shownum(154, 250, ft5426_dev.x[3], 5, 24);
			lcd_shownum(154, 280, ft5426_dev.y[3], 5, 24);
			lcd_shownum(154, 310, ft5426_dev.x[4], 5, 24);
			lcd_shownum(154, 340, ft5426_dev.y[4], 5, 24);
			ft5426_dev.intflag = 0; /* 标记处理完成 */
		}
	#endif
		lcd_shownum(154, 40, ft5426_dev.point_num , 1, 24);
		lcd_shownum(154, 70, ft5426_dev.x[0], 5, 24);
		lcd_shownum(154, 100, ft5426_dev.y[0], 5, 24);
		lcd_shownum(154, 160, ft5426_dev.x[1], 5, 24);
		lcd_shownum(154, 130, ft5426_dev.y[1], 5, 24);
		lcd_shownum(154, 190, ft5426_dev.x[2], 5, 24);
		lcd_shownum(154, 220, ft5426_dev.y[2], 5, 24);
		lcd_shownum(154, 250, ft5426_dev.x[3], 5, 24);
		lcd_shownum(154, 280, ft5426_dev.y[3], 5, 24);
		lcd_shownum(154, 310, ft5426_dev.x[4], 5, 24);
		lcd_shownum(154, 340, ft5426_dev.y[4], 5, 24);
		
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
