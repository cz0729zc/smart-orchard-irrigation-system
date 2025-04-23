/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口3功能的源文件               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "usart3.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件

#if  USART3_RX_ENABLE                   //如果使能接收功能
char Usart3_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart3_RxCounter = 0;      //定义一个变量，记录串口3总共接收了多少字节的数据
char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //定义一个数组，用于保存串口3接收到的数据   	
#endif

/*-------------------------------------------------*/
/*函数名：初始化串口3发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart3_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
#if USART3_RX_ENABLE                         //如果使能接收功能
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
#endif

#if USART3_RX_ENABLE                                 //如果使能接收功能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //使能串口3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOB时钟
	USART_DeInit(USART3);                                  //串口3寄存器重新设置为默认值
    
    // PB10:TX 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //准备设置PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB10
   
    // PB11:RX 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //准备设置PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
#if USART3_RX_ENABLE               												   //如果使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
#else                                                                              //如果不使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //只发模式
#endif        
    USART_Init(USART3, &USART_InitStructure);                                      //设置串口3	

#if USART3_RX_ENABLE  	         					        //如果使能接收模式
	USART_ClearFlag(USART3, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //设置串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口3中断
#endif  

	USART_Cmd(USART3, ENABLE);                              //使能串口3
}

/*-------------------------------------------------*/
/*函数名：串口3 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char USART3_TxBuff[USART3_TXBUFF_SIZE];  

void u3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART3_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART3_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = USART3_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
}

/*-------------------------------------------------*/
/*函数名：串口3发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u3_TxData(unsigned char *data)
{
	int	i;	
	while((USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		USART3->DR = data[i+1];
		while((USART3->SR&0X40)==0);	
	}
}