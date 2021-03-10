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
#include "bsp_mpu9250.h"
#include "bsp_spi.h"
#include "stdio.h"


/*
 * @description	: 指定的位置显示数据
 * @param - x	: X轴位置
 * @param - y 	: Y轴位置
 * @param - num : 要显示的数据
 * @return 		: 无
 */
void num_display(unsigned short x, unsigned short y, short num)
{
	char buf[100];
	memset(buf, 0, sizeof(buf));
	if(num < 0)
		sprintf(buf, "-%d", -num);
	else 
		sprintf(buf, "%d", num);
	lcd_show_string(x, y, 200, 24, 24, buf); 
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


	short ax, ay, az;	        /* 加速度原始数据 			*/
	short gx, gy, gz;        	/* 陀螺仪原始数据  		*/
	short mx, my, mz;        	/* 磁力计原始数据  		*/

	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	led_init();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 			*/
	mpu9250_init();				/* 初始化MPU9250 		*/
	spi3_init();				/* 初始化SPI		 	*/

	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ZERO-IMX6UL IIC TEST");  
	
	lcd_show_string(50, 40, 200, 24, 24, (char*)"Acc X:");  
	lcd_show_string(50, 70, 200, 24, 24, (char*)"Acc Y:");  
	lcd_show_string(50, 100, 200, 24, 24, (char*)"Acc Z:");
		
	lcd_show_string(50, 130, 200, 24, 24, (char*)"Gyr X:");  
	lcd_show_string(50, 160, 200, 24, 24, (char*)"Gyr Y:");  
	lcd_show_string(50, 190, 200, 24, 24, (char*)"Gyr Z:"); 

	lcd_show_string(50, 220, 200, 24, 24, (char*)"Mag X:"); 
	lcd_show_string(50, 250, 200, 24, 24, (char*)"Mag Y:");  
	lcd_show_string(50, 280, 200, 24, 24, (char*)"Mag Z:"); 
	
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{
		mpu_get_accelerometer(&ax, &ay, &az);
		mpu_get_gyroscope(&gx, &gy, &gz);
		mpu_get_magnetometer(&mx, &my, &mz);	
		
		delayms(10);
		i++;

		/* 100ms显示一次 */
		if(i%10 == 0)
		{
			/*显示加速度计原始数据 */
			num_display(122, 40, ax);
			num_display(122, 70, ay);
			num_display(122, 100, az);
		
			/*显示加陀螺仪原始数据 */
			num_display(122, 130, gx);
			num_display(122, 160, gy);
			num_display(122, 190, gz);

			/*显示加磁力计原始数据 */
			num_display(122, 220, mx);
			num_display(122, 250, my);
			num_display(122, 280, mz);
		}
		if(i == 50)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state);	
		}
	}
	return 0;
}
