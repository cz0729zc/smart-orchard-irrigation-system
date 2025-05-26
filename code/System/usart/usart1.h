#ifndef __USART1_H
#define __USART1_H

#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define USART1_RX_ENABLE     1      // 接收功能使能
#define USART1_TXBUFF_SIZE   1024   // 发送缓冲区大小

#if USART1_RX_ENABLE
#define USART1_RXBUFF_SIZE   1024
extern char Usart1_RxCompleted;     // 接收完成标志
extern unsigned int Usart1_RxCounter; // 接收字节计数
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; // 接收缓冲区
#endif

void Usart1_Init(unsigned int bound);
void u1_printf(char* fmt, ...);
void u1_TxData(const uint8_t *data, uint16_t length);

#endif

