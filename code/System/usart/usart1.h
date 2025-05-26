#ifndef __USART1_H
#define __USART1_H

#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define USART1_RX_ENABLE     1      // ���չ���ʹ��
#define USART1_TXBUFF_SIZE   1024   // ���ͻ�������С

#if USART1_RX_ENABLE
#define USART1_RXBUFF_SIZE   1024
extern char Usart1_RxCompleted;     // ������ɱ�־
extern unsigned int Usart1_RxCounter; // �����ֽڼ���
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; // ���ջ�����
#endif

void Usart1_Init(unsigned int bound);
void u1_printf(char* fmt, ...);
void u1_TxData(const uint8_t *data, uint16_t length);

#endif

