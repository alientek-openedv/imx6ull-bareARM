/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 mian.c
作者	   : 左忠凯
版本	   : V1.0
描述	   : I.MX6U开发板裸机实验19 SPI实验
其他	   : SPI也是最常用的接口，ZERO开发板上有一个气压计BMP280，
		 这个气压计就是SPI接口的，本实验就来学习如何驱动I.MX6U
		 的SPI接口，并且通过SPI接口读取BMP280的数据值。
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/17 左忠凯创建
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
#include "bsp_bmp280.h"
#include "bsp_spi.h"
#include "stdio.h"


/*
 * @description	: 指定的位置显示整数数据
 * @param - x	: X轴位置
 * @param - y 	: Y轴位置
 * @param - num : 要显示的数据
 * @return 		: 无
 */
void integer_display(unsigned short x, unsigned short y, signed int num)
{
	char buf[200];
	memset(buf, 0, sizeof(buf));
	if(num < 0)
		sprintf(buf, "-%d", -num);
	else 
		sprintf(buf, "%d", num);
	lcd_show_string(x, y, 200, 24, 24, buf); 
}


/*
 * @description	: 指定的位置显示小数数据,比如5123，显示为51.23
 * @param - x	: X轴位置
 * @param - y 	: Y轴位置
 * @param - num : 要显示的数据，实际小数扩大100倍，
 * @return 		: 无
 */
void decimals_display(unsigned short x, unsigned short y, signed int num)
{
	signed int integ; 	/* 整数部分 */
	signed int fract;	/* 小数部分 */
	signed int uncomptemp = num; 

	if(num < 0)
		uncomptemp = -uncomptemp;
	integ = uncomptemp / 100;
	fract = uncomptemp % 100;

	if(num < 0)	/* 负数显示 */
	{
		lcd_showchar(x, y, '-', 24);
		lcd_shownum(x + 12, y, integ, 4, 24);
		lcd_showchar(x + 60, y, '.', 24);
		lcd_shownum(x + 72, y, fract, 2, 24);
	} else {	
		lcd_shownum(x, y, integ, 4, 24);
		lcd_showchar(x + 48, y, '.', 24);
		lcd_shownum(x + 60, y, fract, 2, 24);

	}	
}

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
	bmp280_init();				/* 初始化BMP280		 		*/

	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ZERO-IMX6U SPI TEST");  
	
	lcd_show_string(50, 40, 200, 24, 24, (char*)"Uncomp Temp:");  
	lcd_show_string(50, 70, 200, 24, 24, (char*)"Uncomp Pres:");  
	lcd_show_string(50, 100, 300, 24, 24, (char*)"Actual Temp:       C");  
	lcd_show_string(50, 130, 300, 24, 24, (char*)"Actual Pres:       hPa"); 
	
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{	
		bmp280_dev.uncomp_temp = bmp280_get_uncomptemp();
		bmp280_dev.uncomp_pres = bmp280_get_uncomppres();
		bmp280_dev.actual_temp = bmp280_get_actualtemp(bmp280_dev.uncomp_temp);
		bmp280_dev.actual_pres = bmp280_get_actualpres(bmp280_dev.uncomp_pres);

		integer_display(194, 40, bmp280_dev.uncomp_temp);
		integer_display(194, 70, bmp280_dev.uncomp_pres);
		decimals_display(194, 100, bmp280_dev.actual_temp);
		decimals_display(194, 130, bmp280_dev.actual_pres);
			
		delayms(50);
		i++;

		if(i == 10)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state);	
		}
	}
	return 0;
}
