/*
 * 启动文件
 */
.global _start  /* 全局标号 */
.global s_delay

_start:

	ldr sp,=0X80200000	//设置栈指针
	b main				//跳转到main函数

