#include "keys.h"
#include "delay.h"

void Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 初始化PB12、PB13、PB14为上拉输入
    GPIO_InitStruct.GPIO_Pin = SETTING_PIN | INCREASE_PIN | DECREASE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

Key_Value Key_Scan(void) {
    static u8 setting_lock = 0, increase_lock = 0, decrease_lock = 0;
    Key_Value result = KEY_NONE;
    
    // 检测PB12(设置键)
    if(GPIO_ReadInputDataBit(SETTING_PORT, SETTING_PIN) == RESET) {
        if(!setting_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(SETTING_PORT, SETTING_PIN) == RESET) {
                result = KEY_SETTING_PRESSED;
            }
            setting_lock = 1;
        }
    }
    else {
        setting_lock = 0;
    }
    
    // 检测PB13(增加键)
    if(GPIO_ReadInputDataBit(INCREASE_PORT, INCREASE_PIN) == RESET) {
        if(!increase_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(INCREASE_PORT, INCREASE_PIN) == RESET) {
                result = KEY_INCREASE_PRESSED;
            }
            increase_lock = 1;
        }
    }
    else {
        increase_lock = 0;
    }
    
    // 检测PB14(减小键)
    if(GPIO_ReadInputDataBit(DECREASE_PORT, DECREASE_PIN) == RESET) {
        if(!decrease_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(DECREASE_PORT, DECREASE_PIN) == RESET) {
                result = KEY_DECREASE_PRESSED;
            }
            decrease_lock = 1;
        }
    }
    else {
        decrease_lock = 0;
    }
    
    return result;
}

