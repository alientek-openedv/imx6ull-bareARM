#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "imx6ul.h"
/*
 * IMX6UL 串口驱动文件
 * 作者:左忠凯
 */

void uart_init(void);
void uart_disable(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_softreset(UART_Type *base);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);
void putc(unsigned char c);
void puts(char *str);
unsigned char getc(void);
void raise(int sig_nr);



#endif
