#include "stm32f10x.h"
#include "usart1.h"

#if USART1_RX_ENABLE
char Usart1_RxCompleted = 0;
unsigned int Usart1_RxCounter = 0;
char Usart1_RxBuff[USART1_RXBUFF_SIZE];
#endif

// USART1初始化
void Usart1_Init(unsigned int bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if USART1_RX_ENABLE
    NVIC_InitTypeDef NVIC_InitStructure;
#endif

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // PA9-TX 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10-RX 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
#if USART1_RX_ENABLE
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
#else
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
#endif

    USART_Init(USART1, &USART_InitStructure);

#if USART1_RX_ENABLE
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif

    USART_Cmd(USART1, ENABLE);
}

// 格式化输出函数
__align(8) char USART1_TxBuff[USART1_TXBUFF_SIZE];

void u1_printf(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(USART1_TxBuff, fmt, ap);
    va_end(ap);

    uint16_t length = strlen(USART1_TxBuff);
    for(uint16_t i = 0; i < length; i++)
    {
        USART_SendData(USART1, USART1_TxBuff[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

// 原始数据发送
void u1_TxData(const uint8_t *data, uint16_t length) {
	//USART_SendData(USART1,*data);
    for(uint16_t i = 0; i < length; i++) {
        while((USART1->SR & 0X40) == 0); // 等待发送寄存器空
        USART1->DR = data[i];
    }
	
}


