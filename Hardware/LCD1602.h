 #ifndef __LCD1602_H
 #define __LCD1602_H

 #include "stm32f10x.h"

 // 引脚定义
 #define LCD_RS_GPIO    GPIOC
 #define LCD_RS_PIN     GPIO_Pin_13

 #define LCD_RW_GPIO    GPIOC
 #define LCD_RW_PIN     GPIO_Pin_14

 #define LCD_EN_GPIO     GPIOC
 #define LCD_EN_PIN      GPIO_Pin_15

 #define LCD_DATA_GPIO  GPIOA
 #define LCD_DATA_PINS  (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                         GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)

 // 函数声明
 void LCD_Init(void);
 void LCD_WriteCommand(uint8_t cmd);
 void LCD_WriteData(uint8_t data);
 void LCD_SetCursor(uint8_t row, uint8_t col);
 void LCD_PrintString(uint8_t x,uint8_t y, char *str);
 void LCD_PrintChar(uint8_t x,uint8_t y, char Char);
 void LCD_PrintData(uint8_t x,uint8_t y, uint8_t Char);
 void LCD_Clear(void);
 void LCD_ClearChar(uint8_t x, uint8_t y);
 #endif
