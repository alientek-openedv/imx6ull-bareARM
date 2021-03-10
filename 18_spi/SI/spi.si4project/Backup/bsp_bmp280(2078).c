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
#include "bsp_delay.h"
#include "stdio.h"

extern void spi3_init(void);
extern unsigned char spi3_readwrite_byte(unsigned char TxData);

struct bmp280_dev_struc bmp280_dev; 


/*
 * @description	: 初始化BMP280
 * @param		: 无
 * @return 		: 0 初始化成功，其他值 初始化失败
 */
unsigned char bmp280_init(void)
{	
	unsigned char test[6];
	
	unsigned char regvalue;
	gpio_pin_config_t cs_config;
	
	/* 1、初始化片选引脚 */
	IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
	IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0X10B0);

	/* 2、配置片选引脚的GPIO属性 */
	cs_config.direction = kGPIO_DigitalOutput;
	cs_config.outputLogic = 1;
	gpio_init(GPIO1, 20, &cs_config);
	
	/* 3、初始化SPI */
	spi3_init();	

	/* 4、配置BMP280 */
	regvalue = bmp280_read_reg(BMP280_ID_REG);/* 读取ID */
	if(regvalue == BMP280_ID_VALUE)
	{
		printf("bmp280 id = %#X\r\n", regvalue);
	} else {
		return 1;
	}
	
	bmp280_write_reg(BMP280_RESET_REG, BMP280_RESET_VALUE); /* 复位BMP280 */
	bmp280_write_reg(BMP280_CTRLMEAS_REG, 0);				/* 进入睡眠模式才可以写控制寄存器 */

	/* 
	 *配置BMP280控制寄存器CONFIG_REG
	 *bit[7:5]	: 001 normal模式下的T_standby，62.5ms
	 *bit[4:2]	: 111 IIR滤波器的时间常量 X16 
	 *bit[1:0]	: 0 禁止3线SPI模式
	 */
	bmp280_write_reg(BMP280_CONFIG_REG, 0xF4);

	/* 
	 *配置BMP280测量寄存器CTRLMEAS_REG
	 *bit[7:5]	: 010 温度采样率x2	 17bit/0.0025°C
	 *bit[4:2]	: 111 压力采样率x16 20bit/0.16Pa 
	 *bit[1:0]	: 11 normal模式 
	 */
	bmp280_write_reg(BMP280_CTRLMEAS_REG, 0x5F);			

	bmp280_get_calibparam();	/* 计算参数 */

	return 0;
}

/*
 * @description	: 计算BMP280的计算参数
 * @param		: 无
 * @return 		: 无
 */
void bmp280_get_calibparam(void)
{
	unsigned char param_buf[BMP280_TRIMPARAM_LEN];

	/* 读取BMP280的参数 */
	bmp280_read_len(BMP280_TRIMPARAM_FRG, param_buf, BMP280_TRIMPARAM_LEN);

	/* 计算参数 */
	bmp280_dev.calib_param.dig_T1 = (u16)((((u16)((u8)param_buf[1])) << 8) | param_buf[0]);
	bmp280_dev.calib_param.dig_T2 = (s16)((((s16)((s8)param_buf[3])) << 8) | param_buf[2]);
	bmp280_dev.calib_param.dig_T3 = (s16)((((s16)((s8)param_buf[5])) << 8) | param_buf[4]);
	bmp280_dev.calib_param.dig_P1 = (u16)((((u16)((u8)param_buf[7])) << 8) | param_buf[6]);
	bmp280_dev.calib_param.dig_P2 = (s16)((((s16)((s8)param_buf[9])) << 8) | param_buf[8]);
	bmp280_dev.calib_param.dig_P3 = (s16)((((s16)((s8)param_buf[11])) << 8) | param_buf[10]);
	bmp280_dev.calib_param.dig_P4 = (s16)((((s16)((s8)param_buf[13])) << 8) | param_buf[12]);
	bmp280_dev.calib_param.dig_P5 = (s16)((((s16)((s8)param_buf[15])) << 8) | param_buf[14]);
	bmp280_dev.calib_param.dig_P6 = (s16)((((s16)((s8)param_buf[17])) << 8) | param_buf[16]);
	bmp280_dev.calib_param.dig_P7 = (s16)((((s16)((s8)param_buf[19])) << 8) | param_buf[18]);
	bmp280_dev.calib_param.dig_P8 = (s16)((((s16)((s8)param_buf[21])) << 8) | param_buf[20]);
	bmp280_dev.calib_param.dig_P9 = (s16)((((s16)((s8)param_buf[23])) << 8) | param_buf[22]);
}

/*
 * @description	: 获取BMP280原始的温度数据值
 * @param		: 无
 * @return 		: 原始的温度数据值
 */
signed int bmp280_get_uncomptemp(void)
{
	unsigned char temp_value[3];
	signed int uncomp_temp = 0;
	bmp280_read_len(BMP280_TEMPMSB_REG, temp_value, 3);
	
	uncomp_temp = (s32)((((u32)(temp_value[0])) << 12) 
				| (((u32)(temp_value[1])) << 4)
				| ((u32)temp_value[2] >> 4));
	return uncomp_temp;
}

/*
 * @description	: 获取BMP280原始的压力数据值
 * @param		: 无
 * @return 		: 原始的温度数据值
 */
signed int bmp280_get_uncomppres(void)
{
	unsigned char pres_value[3];
	signed int uncomp_pres = 0;

	bmp280_read_len(BMP280_PRESMSB_REG, pres_value, 3);
	
	uncomp_pres = (s32)((((u32)(pres_value[0])) << 12) 
				| (((u32)(pres_value[1])) << 4)
				| ((u32)pres_value[2] >> 4));
	return uncomp_pres;
}


/*
 * @description	: 读取BMP280寄存器值
 * @param - reg	: 要读取的寄存器地址
 * @return 		: 读取到的寄存器值
 */
unsigned char bmp280_read_reg(unsigned char reg)
{
	unsigned char reg_val;	  

	/* BMP280在使用SPI接口的时候寄存器地址
	 * 只有低7位有效,寄存器地址最高位是读/写标志位
	 * 读的时候要为1，写的时候要为0。
	 */
	reg |= 0x80; 	
	
   	BMP280_CSN(0);               			/* 使能SPI传输	 		*/
  	spi3_readwrite_byte(reg);     			/* 发送寄存器地址  		*/ 
  	reg_val = spi3_readwrite_byte(0XFF);	/* 读取寄存器的值 			*/
 	BMP280_CSN(1);                			/* 禁止SPI传输 			*/
  	return(reg_val);               	 		/* 返回读取到的寄存器值 */
}	

/*
 * @description	: 读取BMP280连续多个寄存器
 * @param - reg	: 要读取的寄存器地址
 * @return 		: 读取到的寄存器值
 */
void bmp280_read_len(unsigned char reg, unsigned char *buf, unsigned char len)
{  
	unsigned char i;

	/* BMP280在使用SPI接口的时候寄存器地址
	 * 只有低7位有效,寄存器地址最高位是读/写标志位
	 * 读的时候要为1，写的时候要为0。
	 */
	reg |= 0x80; 
	
   	BMP280_CSN(0);               			/* 使能SPI传输	 		*/
  	spi3_readwrite_byte(reg);				/* 发送寄存器地址  		*/   	   
 	for(i = 0; i < len; i++)				/* 顺序读取寄存器的值 			*/
 	{
		buf[i]=spi3_readwrite_byte(0XFF);	
	}
 	BMP280_CSN(1);                			/* 禁止SPI传输 			*/
}	

/*
 * @description	 : 写BMP280指定寄存器
 * @param - reg	 : 要读取的寄存器地址
 * @param - value: 要写入的值
 * @return 		 : 无
 */
void bmp280_write_reg(unsigned char reg, unsigned char value)
{
	/* BMP280在使用SPI接口的时候寄存器地址
	 * 只有低7位有效,寄存器地址最高位是读/写标志位
	 * 读的时候要为1，写的时候要为0。
	 */
	reg &= ~0x80; 

   	BMP280_CSN(0);               			/* 使能SPI传输	 		*/
  	spi3_readwrite_byte(reg);     			/* 发送寄存器地址  		*/ 
  	spi3_readwrite_byte(value);				/* 发送要写入的值 			*/
 	BMP280_CSN(1);                			/* 禁止SPI传输 			*/
}	

