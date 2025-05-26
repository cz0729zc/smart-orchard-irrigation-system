#ifndef __KEYS_H
#define __KEYS_H

#include "stm32f10x.h"

// 引脚定义
#define SETTING_PIN     GPIO_Pin_12  // PB12
#define SETTING_PORT    GPIOB
#define SETTING_CLK     RCC_APB2Periph_GPIOB

#define INCREASE_PIN    GPIO_Pin_13  // PB13
#define INCREASE_PORT   GPIOB
#define INCREASE_CLK    RCC_APB2Periph_GPIOB

#define DECREASE_PIN    GPIO_Pin_14  // PB14
#define DECREASE_PORT   GPIOB
#define DECREASE_CLK    RCC_APB2Periph_GPIOB

//PB7引脚定义
#define MODE_PIN       GPIO_Pin_7   // PB7
#define MODE_PORT      GPIOB
#define MODE_CLK       RCC_APB2Periph_GPIOB

// 按键功能定义
typedef enum {
    KEY_NONE = 0,
    KEY_SETTING_PRESSED,    // PB12按键按下(进入阈值设置界面)
    KEY_INCREASE_PRESSED,   // PB13按键按下(增加)
    KEY_DECREASE_PRESSED,    // PB14按键按下(减小)
	 KEY_MODE_PRESSED        // PB7按键按下
} Key_Value;

void Key_Init(void);
Key_Value Key_Scan(void);

#endif

