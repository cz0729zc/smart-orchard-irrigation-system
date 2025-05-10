#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

#include "delay.h"
#include "usart1.h"
#include "usart3.h"

#include "Timer4.h"

#include "LCD1602.h"
#include "Keys.h"
#include "DHT11.h"
#include "AD.h"

#include "gizwits_product.h"
#include "gizwits_protocol.h"

// 全局变量
u8 g_temp;
u8 g_humi;
u8 g_SoilHumi;
u8 g_Light;
bool g_SPK = 0; // 蜂鸣器状态
bool g_JD = 0;  // 继电器状态
bool g_LED = 0; // LED状态

u8 SoilHumi_threshold = 50;  // 默认阈值
u8 Light_threshold = 50;

// KEY选择
typedef enum {
    NORMAL_MODE = 0,
    SET_SOIL_HUMI,
    SET_LIGHT
} SystemMode;

SystemMode currentMode = NORMAL_MODE;

typedef enum {
    AUTO_MODE = 0,
    MANUAL_MODE
} WorkMode;

WorkMode workMode = AUTO_MODE;  // 默认自动模式

u8 blink_flag = 0;  // 用于闪烁指示当前设置项
#define V0_ADC 190   // 洁净空气ADC值
#define REF_ADC 2700 // 1000ppm时的ADC值（需标定）
#define REF_PPM 1000.0f


// 继电器
#define JD_GPIO   GPIOB
#define JD_Pin    GPIO_Pin_15
#define JD(x)  x? GPIO_WriteBit(JD_GPIO,JD_Pin,Bit_SET):GPIO_WriteBit(JD_GPIO,JD_Pin,Bit_RESET)

//蜂鸣器
#define SPK_GPIO   GPIOB
#define SPK_Pin    GPIO_Pin_9
#define SPK(x)  x? GPIO_WriteBit(SPK_GPIO,SPK_Pin,Bit_SET):GPIO_WriteBit(SPK_GPIO,SPK_Pin,Bit_RESET)

//LED
#define LED_GPIO   GPIOA
#define LED_Pin    GPIO_Pin_8
#define LED(x)  x? GPIO_WriteBit(LED_GPIO,LED_Pin,Bit_SET):GPIO_WriteBit(LED_GPIO,LED_Pin,Bit_RESET)

// 函数声明
void Init_Component_IO(void);

int main(void)
{
	int ret;
	char showchar[32];
	int AD_SoilHumi,AD_LightValue;
	
    LCD_Init();        // LCD初始化
	Init_Component_IO(); //通用GPIO初始化
	Key_Init();
    DHT11_Init();
    AD_Init();           // AD需要放在串口前初始化不然串口失灵
	Usart1_Init(9600);   //串口调试
	Usart3_Init(9600);   //机智云
	
	TIM4_Init(1000-1, 72-1);  // 初始化1ms定时为机智云协议提供时基
	TIM_Cmd(TIM4, ENABLE);    // 启动定时器
	

	LCD_PrintString(1,1,"System loading...");
    //机智云初始化
    userInit();
    delay_ms(300);
    gizwitsInit();
    delay_ms(300);
	
    gizwitsSetMode(WIFI_SOFTAP_MODE);
    LCD_Clear();
    while(1) {
		// 读取传感器数据（原有代码不变）
		ret = DHT11_Read_Data(&g_temp, &g_humi);
		AD_SoilHumi = AD_Value[0];
		AD_LightValue = AD_Value[1];
		g_SoilHumi = 100 - (AD_SoilHumi * 100 / 4095);
		g_Light = 100 - (AD_LightValue * 100 /4095);

		// 按键扫描和处理
    Key_Value key = Key_Scan();
    switch(key) {
        case KEY_MODE_PRESSED:
            workMode = (workMode == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;
            break;
            
        case KEY_SETTING_PRESSED:
            if(workMode == MANUAL_MODE) {
                // 手动模式下按键1控制LED
                g_LED = !g_LED;
            } else {
                // 自动模式下保持原有设置功能
                if(currentMode == NORMAL_MODE) {
                    currentMode = SET_SOIL_HUMI;
                } else if(currentMode == SET_SOIL_HUMI) {
                    currentMode = SET_LIGHT;
                } else {
                    currentMode = NORMAL_MODE;
                }
                blink_flag = 0;
            }
            break;
            
        case KEY_INCREASE_PRESSED:
            if(workMode == MANUAL_MODE) {
                // 手动模式下按键2控制继电器
                g_JD = !g_JD;
            } else {
                // 自动模式下保持原有增加功能
                if(currentMode == SET_SOIL_HUMI && SoilHumi_threshold < 100) {
                    SoilHumi_threshold++;
                } else if(currentMode == SET_LIGHT && Light_threshold < 100) {
                    Light_threshold++;
                }
            }
            break;
            
        case KEY_DECREASE_PRESSED:
            if(workMode == MANUAL_MODE) {
                // 手动模式下按键3控制蜂鸣器
                g_SPK = !g_SPK;
            } else {
                // 自动模式下保持原有减少功能
                if(currentMode == SET_SOIL_HUMI && SoilHumi_threshold > 0) {
                    SoilHumi_threshold--;
                } else if(currentMode == SET_LIGHT && Light_threshold > 0) {
                    Light_threshold--;
                }
            }
            break;
            
        default:
            break;
		}

		// 显示逻辑（带闪烁效果）
		if(currentMode == NORMAL_MODE) {
			// 正常显示模式
			sprintf(showchar, "T:%2dC S:%2d/%-2d%%", 
					g_temp, g_SoilHumi, SoilHumi_threshold);        
			LCD_PrintString(1, 1, showchar);

			sprintf(showchar, "H:%2d%% L:%2d/%-2dLux", 
					g_humi, g_Light, Light_threshold);        
			LCD_PrintString(2, 1, showchar);
		} else {
			// 设置模式（带闪烁）
				if(currentMode == SET_SOIL_HUMI) {
					sprintf(showchar, "Set SoilHumi:%2d%%", SoilHumi_threshold);
					LCD_PrintString(2, 1, showchar);
					LCD_PrintString(1, 1, "                ");  // 清空第二行
				} else {
					sprintf(showchar, "Set Light: %2dLux", Light_threshold);
					LCD_PrintString(2, 1, showchar);
					LCD_PrintString(1, 1, "                ");  // 清空第一行
				}
		}

	   // 控制逻辑
		if(workMode == AUTO_MODE) {
			// 自动模式 - 原有逻辑
			if(g_SoilHumi < SoilHumi_threshold) {
				g_JD = 1;
			} else {
				g_JD = 0;
			}
			
			if(g_Light < Light_threshold) {
				g_LED = 0;
			} else {
				g_LED = 1;
			}
			LCD_PrintString(1,16,"A");
		}
		else
		{
			LCD_PrintString(1,16,"M");
		}
			
        // 机智云协议处理
        userHandle();
        gizwitsHandle((dataPoint_t *)&currentDataPoint);

        JD(g_JD);
        SPK(g_SPK);
        LED(g_LED);
        //u1_printf("Temp: %d Humi: %d PM25: %0.2f MQ2: %0.2f MQ135: %0.2f\r\n", 
        //         g_temp, g_humi, g_PM25, g_MQ2, g_MQ135);
        delay_ms(10);
    }
}

float adc_to_ppm(uint16_t adc_value)
{
    float V0 = (V0_ADC * 3.3f) / 4096.0f;       // 基准电压
    float V_ref = (REF_ADC * 3.3f) / 4096.0f;   // 参考电压
    float V_out = (adc_value * 3.3f) / 4096.0f; // 当前电压

    float ppm = ((V_out - V0) / (V_ref - V0)) * REF_PPM;
    return (ppm > 0) ? ppm : 0; // 浓度不为负
}

void Init_Component_IO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;

	//蜂鸣器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPK_Pin;
	GPIO_Init(SPK_GPIO,&GPIO_InitStructure);
	
	//蜂鸣器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = JD_Pin;
	GPIO_Init(JD_GPIO,&GPIO_InitStructure);
	
	//LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = LED_Pin;
	GPIO_Init(LED_GPIO,&GPIO_InitStructure);
}

