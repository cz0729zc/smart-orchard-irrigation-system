/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart1.h"
#include "usart2.h"
#include "gizwits_product.h"
#include "gizwits_protocol.h"


extern char Usart1_RxCompleted;     // 接收完成标志
extern char Usart1_RxBuff[]; // 接收缓冲区

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*-------------------------------------------------*/
/*函数名：串口1接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
 /*-------------------------------------------------*/
void USART1_IRQHandler(void) {
      uint8_t value = 0;
      if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
          USART_ClearITPendingBit(USART1,USART_IT_RXNE);
          value = USART_ReceiveData(USART1);
          gizPutData(&value, 1);
      }
}
///*-------------------------------------------------*/
///*函数名：串口2接收中断函数                        */
///*参  数：无                                       */
///*返回值：无                                       */
///*-------------------------------------------------*/
//void USART2_IRQHandler(void) {
//  uint8_t value = 0;
//  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
//      USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//      value = USART_ReceiveData(USART2);
//      gizPutData(&value, 1);
//  }
//}


/*-------------------------------------------------*/
/*函数名：串口3接收中断函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
//void USART3_IRQHandler(void) {
//    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // 清除RXNE标志
//        uint8_t data = USART_ReceiveData(USART3);       // 读取数据（必须执行以清除标志）
//    }
//    if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET) {
//        USART_ClearFlag(USART3, USART_FLAG_ORE);       // 清除ORE标志
//        USART_ReceiveData(USART3);                     // 必须读取DR寄存器
//    }
//}

// void USART3_IRQHandler(void) {
//   uint8_t value = 0;
//   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
//       USART_ClearITPendingBit(USART3,USART_IT_RXNE);
//       value = USART_ReceiveData(USART3);
//       gizPutData(&value, 1);
//   }
// }

//extern uint8_t timer4_flag; // TIM4中断标志
//extern uint32_t g_system_tick;    // 系统运行时间(秒)

/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
       TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
       //g_system_tick++; // 系统滴答计数器递增
       gizTimerMs(); // 调用gizwits协议的定时器函数
   }
}
///*-------------------------------------------------*/
///*函数名：定时器3中断服务函数                      */
///*参  数：无                                       */
///*返回值：无                                       */
///*-------------------------------------------------*/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if	
//		switch(Ping_flag){                               //判断Ping_flag的状态
//			case 0:										 //如果Ping_flag等于0，表示正常状态，发送Ping报文  
//					MQTT_PingREQ(); 					 //添加Ping报文到发送缓冲区  
//					break;
//			case 1:										 //如果Ping_flag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
//					TIM3_ENABLE_2S(); 					 //我们将定时器6设置为2s定时,快速发送Ping报文
//					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区  
//					break;
//			case 2:										 //如果Ping_flag等于2，说明还没有收到服务器回复
//			case 3:				                         //如果Ping_flag等于3，说明还没有收到服务器回复
//			case 4:				                         //如果Ping_flag等于4，说明还没有收到服务器回复	
//					MQTT_PingREQ();  					 //添加Ping报文到发送缓冲区 
//					break;
//			case 5:										 //如果Ping_flag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
//					Connect_flag = 0;                    //连接状态置0，表示断开，没连上服务器
//					TIM_Cmd(TIM3,DISABLE);               //关TIM3 				
//					break;			
//		}
//		Ping_flag++;           		             		 //Ping_flag自增1，表示又发送了一次ping，期待服务器的回复
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //清除TIM3溢出中断标志 	
//	}
//}
/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数                      */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
//void TIM2_IRQHandler(void)
//{	
////	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
////		Read_Sensor();
////		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
////	}
//  
//  
//  static u8 num = 0;
//  
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//  {  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 	
//    

//		Upload_TimeOK = 1;
////      Read_Sensor();//上载数据,0.5秒中断
//		
//			num++;
//      if(num >= 2)
//      {
//        num = 0;
//        Second++;
//				if(Second >= 60)
//				{
//					Second = 0;
//					Minute++;
//					if(Minute >= 60)
//					{
//						Minute = 0;
//						Hour ++;
//					}
//				}
//				
////				Dispaly();
//      }
//			
//			
//	}   
//}




/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
