/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器1功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "timer1.h"  //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：定时器1使能1分钟定时                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM1_ENABLE_60S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);           //使能TIM1时钟	
	
    TIM_DeInit(TIM1);                                             //定时器1寄存器恢复默认值
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=2-1;          //设置定时器重复计数2次（TIM1和8 才有这个功能）
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);                      //使能TIM1溢出中断    
                       	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;              //设置TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
	
	TIM_Cmd(TIM1,ENABLE);                                         //开TIM1   
}


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3
void Timer1_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =(psc-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
							 
}


void TIM1_UP_IRQHandler(void)   //TIM2中断
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//      Time1_GPSDelay5ms += 1;//中断到来
//      TIM_Cmd(TIM1, DISABLE);  //失能TIMx外设
//      TIM_SetCounter(TIM1,0);//清除定时器计数
//      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭接收中断
//////      USART1_RX_Count = 0;//串口接收计数器清零
			


		}
}
