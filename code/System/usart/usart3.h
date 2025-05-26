/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口3功能的头文件               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __USART3_H
#define __USART3_H

#include "stdio.h"      //包含需要的头文件
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#include "stm32f10x_usart.h"

#define USART3_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define USART3_TXBUFF_SIZE   1024   //定义串口3 发送缓冲区大小 1024字节

#if  USART3_RX_ENABLE                         //如果使能接收功能
#define USART3_RXBUFF_SIZE   1024              //定义串口3 接收缓冲区大小 1024字节
extern char Usart3_RxCompleted;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart3_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量
#endif

void Usart3_Init(unsigned int);       
void u3_printf(char*,...) ;          
void u3_TxData(unsigned char *data);

#endif

