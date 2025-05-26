#include "beep.h"
#include "delay.h"

// ��ʼ��������
void BEEP_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // ����PA8Ϊ�������
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);

    // Ĭ�Ϲرշ�����
    BEEP_Off();
}

// �򿪷�����
void BEEP_On(void) {
    GPIO_SetBits(BEEP_GPIO, BEEP_PIN); // PA15����ߵ�ƽ
}

// �رշ�����
void BEEP_Off(void) {
    GPIO_ResetBits(BEEP_GPIO, BEEP_PIN); // PA15����͵�ƽ
}

// ��������������
void BEEP_Alarm(uint8_t flag) {
    if (flag) {
        BEEP_On(); // �򿪷�����
    } else {
        BEEP_Off(); // �رշ�����
    }
}

