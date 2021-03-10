/***************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名	: 	 bsp_bmp280.
作者	   : 左忠凯
版本	   : V1.0
描述	   : BMP280驱动文件。
其他	   : 无
论坛 	   : www.openedv.com
日志	   : 初版V1.0 2019/1/17 左忠凯创建
***************************************************************/
#include "bsp_bmp280.h"
#include "bsp_spi.h"

/*
 * @description	: 初始化BMP280
 * @param		: 无
 * @return 		: 无
 */
void bmp280_init(void)
{	
	spi3_init();	/* 初始化spi3 */
	
	
}

//读取SPI寄存器值
//reg:要读的寄存器
unsigned char bmp280_Read_Reg(unsigned char reg)
{
	unsigned char reg_val;	    
   	BMP280_CSN(0);                //使能SPI传输		
  	spi3_readwrite_byte(reg);      //发送寄存器号
  	reg_val=spi3_readwrite_byte(0XFF);//读取寄存器内容
 	BMP280_CSN(1);                //禁止SPI传输		
  	return(reg_val);                //返回状态值
}	

