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


extern char Usart1_RxCompleted;     // ������ɱ�־
extern char Usart1_RxBuff[]; // ���ջ�����

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
/*������������1�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
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
///*������������2�����жϺ���                        */
///*��  ������                                       */
///*����ֵ����                                       */
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
/*������������3�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//void USART3_IRQHandler(void) {
//    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // ���RXNE��־
//        uint8_t data = USART_ReceiveData(USART3);       // ��ȡ���ݣ�����ִ���������־��
//    }
//    if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET) {
//        USART_ClearFlag(USART3, USART_FLAG_ORE);       // ���ORE��־
//        USART_ReceiveData(USART3);                     // �����ȡDR�Ĵ���
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

//extern uint8_t timer4_flag; // TIM4�жϱ�־
//extern uint32_t g_system_tick;    // ϵͳ����ʱ��(��)

/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
   {
       TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
       //g_system_tick++; // ϵͳ�δ����������
       gizTimerMs(); // ����gizwitsЭ��Ķ�ʱ������
   }
}
///*-------------------------------------------------*/
///*����������ʱ��3�жϷ�����                      */
///*��  ������                                       */
///*����ֵ����                                       */
///*-------------------------------------------------*/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
//		switch(Ping_flag){                               //�ж�Ping_flag��״̬
//			case 0:										 //���Ping_flag����0����ʾ����״̬������Ping����  
//					MQTT_PingREQ(); 					 //���Ping���ĵ����ͻ�����  
//					break;
//			case 1:										 //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
//					TIM3_ENABLE_2S(); 					 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
//					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ�����  
//					break;
//			case 2:										 //���Ping_flag����2��˵����û���յ��������ظ�
//			case 3:				                         //���Ping_flag����3��˵����û���յ��������ظ�
//			case 4:				                         //���Ping_flag����4��˵����û���յ��������ظ�	
//					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ����� 
//					break;
//			case 5:										 //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
//					Connect_flag = 0;                    //����״̬��0����ʾ�Ͽ���û���Ϸ�����
//					TIM_Cmd(TIM3,DISABLE);               //��TIM3 				
//					break;			
//		}
//		Ping_flag++;           		             		 //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);      //���TIM3����жϱ�־ 	
//	}
//}
/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
//void TIM2_IRQHandler(void)
//{	
////	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
////		Read_Sensor();
////		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 	
////	}
//  
//  
//  static u8 num = 0;
//  
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//  {  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 	
//    

//		Upload_TimeOK = 1;
////      Read_Sensor();//��������,0.5���ж�
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
