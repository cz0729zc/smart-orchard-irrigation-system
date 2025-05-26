//#include "stm32f10x.h"
//#include "delay.h"  // 假设您已实现微秒和毫秒延时函数
//#include "Lcd12864Char.h"
//#include "stdio.h"

///***********************
// *  LCD底层驱动函数
// ***********************/

// GPIO初始化
//void LCD_GPIO_Config(void) {
//    GPIO_InitTypeDef GPIO_InitStructure;

//    // 开启GPIO时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

//    // 配置PA0-PA7为推挽输出（数据线）
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
//                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    // 配置控制线PC13(RS)、PC14(RW)、PC15(EN)为推挽输出
//    GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//}

//// 写命令函数
//void LCD_WriteCmd(uint8_t cmd) {
//    GPIO_ResetBits(LCD_CTRL_PORT, LCD_RS_PIN); // RS=0：命令模式
//    GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW_PIN); // RW=0：写入

//    // 数据线输出命令
//    LCD_DATA_PORT->ODR = (LCD_DATA_PORT->ODR & 0xFF00) | (cmd & 0x00FF);

//    // 产生EN使能脉冲（>450ns）
//    GPIO_SetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//    delay_us(1);
//    GPIO_ResetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//    delay_us(100); // 等待命令执行
//}

//// 写数据函数
//void LCD_WriteData(uint8_t data) {
//    GPIO_SetBits(LCD_CTRL_PORT, LCD_RS_PIN); // RS=1：数据模式
//    GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW_PIN); // RW=0：写入

//    // 数据线输出数据
//    LCD_DATA_PORT->ODR = (LCD_DATA_PORT->ODR & 0xFF00) | (data & 0x00FF);

//    // 产生EN使能脉冲
//    GPIO_SetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//    delay_us(1);
//    GPIO_ResetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//    delay_us(100);
//}

// // 写命令
// void LCD_WriteCmd(uint8_t cmd) {
//     GPIO_ResetBits(LCD_CTRL_PORT, LCD_RS_PIN); // RS=0
//     GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW_PIN); // RW=0
//    
//     // 写入数据总线
//     GPIO_Write(LCD_DATA_PORT, (GPIO_ReadOutputData(LCD_DATA_PORT) & 0xFF00) | cmd);
//    
//     // 产生EN脉冲
//     GPIO_SetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//     delay_ms(1);
//     GPIO_ResetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//     delay_ms(1);
// }

// // 写数据
// void LCD_WriteData(uint8_t data) {
//     GPIO_SetBits(LCD_CTRL_PORT, LCD_RS_PIN); // RS=1
//     GPIO_ResetBits(LCD_CTRL_PORT, LCD_RW_PIN); // RW=0
//    
//     GPIO_Write(LCD_DATA_PORT, (GPIO_ReadOutputData(LCD_DATA_PORT) & 0xFF00) | data);
//    
//     GPIO_SetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//     delay_ms(1);
//     GPIO_ResetBits(LCD_CTRL_PORT, LCD_EN_PIN);
//     delay_ms(1);
// }
///***********************
// *  LCD高级功能函数
// ***********************/

// LCD初始化
//void LCD_Init(void) {
//    LCD_GPIO_Config();  // 初始化GPIO
//    delay_ms(50);       // 等待LCD上电稳定

//    // 初始化序列（针对ST7920控制器）
//    LCD_WriteCmd(0x38); // 功能设置：8位接口，基本指令集
//    delay_ms(5);
//    LCD_WriteCmd(0x0C); // 显示开，关光标
//    delay_ms(5);
//    LCD_WriteCmd(0x01); // 清屏
//    delay_ms(5);
//    LCD_WriteCmd(0x06); // 输入方式：光标右移
//    delay_ms(5);
//}

// 设置显示位置（x: 0-15, y: 0-3）
//void LCD_SetPosition(uint8_t x, uint8_t y) {
//    uint8_t addr;
//    switch(y) {
//        case 0: addr = 0x80 + x; break; // 第一行
//        case 1: addr = 0x90 + x; break; // 第二行
//        case 2: addr = 0x88 + x; break; // 第三行
//        case 3: addr = 0x98 + x; break; // 第四行
//        default: addr = 0x80;
//    }
//    LCD_WriteCmd(addr);
//}

// 显示字符串
//void LCD_PrintString(char *str) {
//    while(*str) {
//        LCD_WriteData(*str++);
//    }
//}

///**
// * @brief 在LCD指定位置显示字符串
// * @param x 列位置 (0-15)
// * @param y 行位置 (0-3)
// * @param str 要显示的字符串
// */
//void LCD_PrintStringAt(uint8_t x, uint8_t y, char *str) {
//    // 设置显示位置
//    uint8_t addr;
//    switch(y) {
//        case 0: addr = 0x80 + x; break; // 第一行
//        case 1: addr = 0x90 + x; break; // 第二行
//        case 2: addr = 0x88 + x; break; // 第三行
//        case 3: addr = 0x98 + x; break; // 第四行
//        default: addr = 0x80;    // 默认第一行
//    }
//    LCD_WriteCmd(addr);
//    
//    // 显示字符串
//    while(*str) {
//        LCD_WriteData(*str++);
//    }
//}

//void LCD_WriteDataAt(uint8_t x, uint8_t y, uint8_t data) {
//    // 设置显示位置
//    uint8_t addr;
//    switch(y) {
//        case 0: addr = 0x80 + x; break; // 第一行
//        case 1: addr = 0x90 + x; break; // 第二行
//        case 2: addr = 0x88 + x; break; // 第三行
//        case 3: addr = 0x98 + x; break; // 第四行
//        default: addr = 0x80;    // 默认第一行
//    }
//    LCD_WriteCmd(addr);
//    
//    // 显示字符
//    LCD_WriteData(data);
//}

// 清屏函数
//void LCD_Clear(void) {
//    LCD_WriteCmd(0x01); // 发送清屏命令
//    delay_ms(2);        // 等待清屏完成（需要较长时间）
//}


// 辅助函数：在LCD上显示数字
//void LCD_ShowNum(uint8_t x, uint8_t y, uint16_t num, uint8_t len) {
//    char buf[10];
//    sprintf(buf, "%*d", len, num);
//    LCD_PrintStringAt(x, y, buf);
//}

// 辅助函数：在LCD上显示字符串
//void LCD_ShowString(uint8_t x, uint8_t y, const char *str) {
//    LCD_PrintStringAt(x, y, (char *)str);
//}

// 辅助函数：在LCD上显示字符
//void LCD_ShowChar(uint8_t x, uint8_t y, char ch) {
//    char buf[2] = {ch, '\0'};
//    LCD_PrintStringAt(x, y, buf);
//}

///*********************绘图函数**************/

///**
//  * @brief  设置LCD12864显示坐标
//  * @param  x: x轴坐标0~127, y: y轴坐标0~63
//  * @retval 无
//  */
//static void lcd_setXY(unsigned char x, unsigned char y)
//{
//	 if (y >= 32)
//	 {
//		 /* 下半屏 */
//		 LCD_WriteCmd(0x80 + (y - 32));	// y坐标
//		 LCD_WriteCmd(0x88 + (x>>4));	// x坐标
//	 }
//	 else
//	 {
//		 /* 上半屏 */
//		 LCD_WriteCmd(0x80 + y);		
//		 LCD_WriteCmd(0x80 + (x>>4));	
//	 }
//}
// 
///**
//  * @brief  清空屏幕
//  * @param  无
//  * @retval 无
//  */
//void lcd_clear(void)
//{
//	unsigned char x, y; 
//	
//	LCD_WriteCmd(0x34);    // 切换到扩展指令集   
//	
//	for(y=0; y<64; y++)
//	{   
//		lcd_setXY(0, y);	// 设置显示坐标
//  		for(x=0; x<8; x++)
//  		{	
//			/* 因为x轴一个地址对应16个像素点，可以连续发送两个字节数据 */
//  		    LCD_WriteData(0x00);
//			LCD_WriteData(0x00);
//  		}	
//	}
//	 
//    LCD_WriteCmd(0x36);    // 打开绘图显示
//	LCD_WriteCmd(0x30);    // 回到基本指令集
//}


///**
//  * @brief  显示图片，注意显示起点坐标固定是(0, 0)
//  * @param  无
//  * @retval 无
//  */
//void lcd_draw_picture(const unsigned char *data)
//{
//	unsigned char x, y; 
//	
//	LCD_WriteCmd(0x34);    // 切换到扩展指令集   
//	
//	for(y=0; y<64; y++)
//	{   
//		lcd_setXY(0, y);	// 设置显示坐标
//  		for(x=0; x<8; x++)
//  		{	
//  		    LCD_WriteData(*data++);
//			LCD_WriteData(*data++);
//  		}	
//	}
//	 
//    LCD_WriteCmd(0x36);    // 打开绘图显示
//	LCD_WriteCmd(0x30);    // 回到基本指令集
//}

///* x轴按位显示的位码表 */
//static const unsigned char set_pix_bit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

///* 显示缓冲区 */
//static unsigned char disp_buff[8][64] = {0};

///**
//  * @brief  在LCD显示范围内绘制任意点
//			x坐标范围是：0~127，y坐标范围是：0~63
//  * @param  x: x轴坐标, y: y轴坐标, color: 颜色值，1：点亮像素，0：不点亮
//  * @retval 无
//  */
//void lcd_draw_dots(unsigned char x, unsigned char y, unsigned char color)
//{
//	LCD_WriteCmd(0x34);       // 切换到扩展指令集 
//	
//	/* 超出显示范围，退出函数 */
//	if ((x >= 128) || (y >= 64))
//        return;
//	
//	/* 设置x, y坐标 */
//	lcd_setXY(x, y);
//	
//	/* 填充显示缓冲区数据 */
//	if (color == 1)
//	{
//		/* 点亮某点像素 */
//		disp_buff[x>>3][y] |= 0x00;	
//		disp_buff[(x>>3)+1][y] |= set_pix_bit[x & 0x07];
//	}
//	else
//	{	
//		/* 熄灭某点像素 */
//		disp_buff[x>>3][y] |= 0x00;
//		disp_buff[(x>>3)+1][y] &= ~set_pix_bit[x & 0x07];
//	}
//	
//	/* 输出数据到LCD显示 */
//	if ((x >> 3) % 2 != 0)	// 判断x轴0~15个显示数据的奇偶性
//	{
//		/* 奇数 */
//		LCD_WriteData(disp_buff[x>>3][y]);
//		LCD_WriteData(disp_buff[(x>>3)+1][y]);
//	}
//	else
//	{
//		/* 偶数 */
//		LCD_WriteData(disp_buff[(x>>3)+1][y]);
//		LCD_WriteData(disp_buff[x>>3][y]);
//	}
// 
//    LCD_WriteCmd(0x36);        // 打开绘图显示
//	LCD_WriteCmd(0x30);        // 回到基本指令集
//}


///**
//  * @brief  绘制水平线
//  * @param  x0: x轴起点, y0: y0轴起点, x1: x轴结束点, color: 颜色值，1点亮像素，0不点亮
//  * @retval 无
//  */
// void lcd_draw_Hline(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char color)
// {
//     /* 超出显示范围，退出函数 */
//     if ((x0 >= 128) || (y0 >= 64) || (x1 >= 128))
//         return;
//     
//     for ( ; x1>=x0; x0++)
//         lcd_draw_dots(x0, y0, color);
// }
// 
///**
//  * @brief  绘制垂直线
//  * @param  x0: x轴起点, y0: y轴起点, y1: y轴结束点, color: 颜色值，1点亮像素，0不点亮
//  * @retval 无
//  */
// void lcd_draw_Vline(unsigned char x0, unsigned char y0, unsigned char y1, unsigned char color)
// {
//     /* 超出显示范围，退出函数 */
//     if ((x0 >= 128) || (y0 >= 64) || (y1 >= 64))
//         return;
//     
//     for ( ; y1>=y0; y0++)
//         lcd_draw_dots(x0, y0, color);
// }
// 



///*******************************************************/


