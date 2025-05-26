 #include "lcd1602.h"
 #include "stm32f10x_gpio.h"
 #include "stm32f10x_rcc.h"

 static void delay_us(uint32_t us) {
     us = us * 9; // 72MHz / 8 = 9MHz
     while(us--) {
         __NOP(); __NOP(); __NOP(); __NOP(); 
         __NOP(); __NOP(); __NOP(); __NOP();
     }
 }

 static void delay_ms(uint32_t ms) {
     while(ms--) {
         delay_us(400);
     }
 }

 // λ��ת����������0b00000001 => 0b10000000
uint8_t ReverseByte(uint8_t byte) {
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}
 
 
 // GPIO��ʼ��
 static void GPIO_Config(void) {
     GPIO_InitTypeDef GPIO_InitStruct;
    
     // ����GPIOAʱ��
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
     // ���ÿ����ߣ�RS, RW, EN��
     GPIO_InitStruct.GPIO_Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN;
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(LCD_RS_GPIO, &GPIO_InitStruct);
    
     // ���������ߣ�PA0-PA7��
     GPIO_InitStruct.GPIO_Pin = LCD_DATA_PINS;
     GPIO_Init(LCD_DATA_GPIO, &GPIO_InitStruct);
 }

// д����
void LCD_WriteCommand(uint8_t cmd) {
    GPIO_ResetBits(LCD_RS_GPIO, LCD_RS_PIN); // RS=0
    GPIO_ResetBits(LCD_RW_GPIO, LCD_RW_PIN); // RW=0

    uint8_t reversedCmd = ReverseByte(cmd);
    GPIO_Write(LCD_DATA_GPIO, (GPIO_ReadOutputData(LCD_DATA_GPIO) & 0xFF00) | reversedCmd);

    GPIO_SetBits(LCD_EN_GPIO, LCD_EN_PIN);
    delay_ms(1);
    GPIO_ResetBits(LCD_EN_GPIO, LCD_EN_PIN);
    delay_ms(1);
}

// д����
void LCD_WriteData(uint8_t data) {
    GPIO_SetBits(LCD_RS_GPIO, LCD_RS_PIN); // RS=1
    GPIO_ResetBits(LCD_RW_GPIO, LCD_RW_PIN); // RW=0

    uint8_t reversedData = ReverseByte(data);
    GPIO_Write(LCD_DATA_GPIO, (GPIO_ReadOutputData(LCD_DATA_GPIO) & 0xFF00) | reversedData);

    GPIO_SetBits(LCD_EN_GPIO, LCD_EN_PIN);
    delay_ms(1);
    GPIO_ResetBits(LCD_EN_GPIO, LCD_EN_PIN);
    delay_ms(1);
}

 // ��ʼ��LCD
 void LCD_Init(void) {
     GPIO_Config();
     delay_ms(50); // LCD�ϵ縴λ
    
     LCD_WriteCommand(0x38); // 8λģʽ��2����ʾ
     LCD_WriteCommand(0x0C); // ��ʾ��������
     LCD_WriteCommand(0x06); // �������
     LCD_WriteCommand(0x01); // ����
     delay_ms(2);
 }

 // ���ù��λ��
 void LCD_SetCursor(uint8_t row, uint8_t col) {
 	if(row==1)
 	{
 		LCD_WriteCommand(0x80|(col-1));
 	}
 	else if(row==2)
 	{
 		LCD_WriteCommand(0x80|(col-1+0x40));
 	}
 }

 // ��ʾ�ַ�
 void LCD_PrintChar(uint8_t x,uint8_t y, char Char) {
 	LCD_SetCursor(x,y);
 	LCD_WriteData(Char);
 }

 // ��ʾ����
 void LCD_PrintData(uint8_t x,uint8_t y, uint8_t Char) {
 	LCD_SetCursor(x,y);
 	LCD_WriteData(Char);
 }

 // ��ʾ�ַ���
 void LCD_PrintString(uint8_t x,uint8_t y, char *str) {
 	unsigned char i;
 	LCD_SetCursor(x,y);
 	for(i=0;str[i]!='\0';i++)
 	{
 		LCD_WriteData(str[i]);
 	}
 }

 // ����
 void LCD_Clear(void) {
     LCD_WriteCommand(0x01);
     delay_ms(2);
 }


 void LCD_ClearChar(uint8_t x, uint8_t y) {
     LCD_SetCursor(x, y);
     LCD_WriteData(' '); // д��ո��ַ�
 }
 
 