#include "keys.h"
#include "delay.h"

void Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // ʹ��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // ��ʼ��PB12��PB13��PB14Ϊ��������
    GPIO_InitStruct.GPIO_Pin = MODE_PIN | SETTING_PIN | INCREASE_PIN | DECREASE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

Key_Value Key_Scan(void) {
    static u8 setting_lock = 0, increase_lock = 0, decrease_lock = 0,mode_lock = 0;
    static u32 increase_press_time = 0, decrease_press_time = 0;
	
    Key_Value result = KEY_NONE;
    
	
    // ���PB7(ģʽ��)
    if(GPIO_ReadInputDataBit(MODE_PORT, MODE_PIN) == RESET) {
        if(!mode_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(MODE_PORT, MODE_PIN) == RESET) {
                result = KEY_MODE_PRESSED;
            }
            mode_lock = 1;
        }
    }
    else {
        mode_lock = 0;
    }
	
    // ���PB12(���ü�)
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
    
    // ���PB13(���Ӽ�) - �����������
    if(GPIO_ReadInputDataBit(INCREASE_PORT, INCREASE_PIN) == RESET) {
        if(!increase_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(INCREASE_PORT, INCREASE_PIN) == RESET) {
                result = KEY_INCREASE_PRESSED;
                increase_press_time = 0;
            }
            increase_lock = 1;
        }
        else {
            // ������⣨ÿ100ms����һ�Σ�
            if(++increase_press_time >= 10) { // Լ1������
                if(increase_press_time % ((increase_press_time > 50) ? 2 : 5) == 0) {
                    result = KEY_INCREASE_PRESSED;
                }
            }
        }
    }
    else {
        increase_lock = 0;
        increase_press_time = 0;
    }
    
    // ���PB14(��С��) - �����������
    if(GPIO_ReadInputDataBit(DECREASE_PORT, DECREASE_PIN) == RESET) {
        if(!decrease_lock) {
            delay_ms(15);
            if(GPIO_ReadInputDataBit(DECREASE_PORT, DECREASE_PIN) == RESET) {
                result = KEY_DECREASE_PRESSED;
                decrease_press_time = 0;
            }
            decrease_lock = 1;
        }
        else {
            // ������⣨ÿ100ms����һ�Σ�
            if(++decrease_press_time >= 10) { // Լ1������
                if(decrease_press_time % ((decrease_press_time > 50) ? 2 : 5) == 0) {
                    result = KEY_DECREASE_PRESSED;
                }
            }
        }
    }
    else {
        decrease_lock = 0;
        decrease_press_time = 0;
    }
    
    return result;
}

