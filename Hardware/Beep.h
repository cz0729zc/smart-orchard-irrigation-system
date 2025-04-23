#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

// ·äÃùÆ÷Òý½Å¶¨Òå
#define BEEP_PIN GPIO_Pin_15
#define BEEP_GPIO GPIOB

void BEEP_Init(void);

void BEEP_On(void);
void BEEP_Off(void);
void BEEP_Alarm(uint8_t flag);

#endif

