#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

#include "delay.h"
#include "usart1.h"
#include "usart3.h"
#include "main.h"

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

u8 g_temp_HT = 45; // 温度上限
u8 g_temp_LT = 0;  // 温度下限
u8 g_humi_HT = 90; // 湿度上限
u8 g_humi_LT = 20; // 湿度下限
u8 g_SoilHumi_HT = 80; // 土壤湿度上限
u8 g_SoilHumi_LT = 40;   // 土壤湿度下限
u8 g_Light_HT = 80; // 光照上限
u8 g_Light_LT =30;  // 光照下限

u8 Warning_flag = 0; // 警报标志位

// KEY选择
typedef enum {
    NORMAL_MODE = 0,
    SET_TEMP_HIGH,
    SET_TEMP_LOW,
    SET_HUMI_HIGH,
    SET_HUMI_LOW,
    SET_SOIL_HUMI_HIGH,
    SET_SOIL_HUMI_LOW,
    SET_LIGHT_HIGH,
    SET_LIGHT_LOW,
} SystemMode;


SystemMode currentMode = NORMAL_MODE;

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

//fan
#define Fan_GPIO   GPIOB
#define Fan_Pin    GPIO_Pin_6
#define Fan(x)  x? GPIO_WriteBit(Fan_GPIO,Fan_Pin,Bit_SET):GPIO_WriteBit(Fan_GPIO,Fan_Pin,Bit_RESET)

// 函数声明
void Init_Component_IO(void);

#define PROTOCOL_DEBUG true

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
    //u1_printf("\r\n**************************************\r\n");
	//g_LED = 1;
    //u1_printf("Gizwits Smart Home\r\n");
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
                currentMode = (SystemMode)((currentMode + 1) % 9);  // 循环切换9种状态
                blink_flag = 0;
            }
            break;
            
        case KEY_INCREASE_PRESSED:
            if(workMode == MANUAL_MODE) {
                g_JD = !g_JD;
            } else {
                switch(currentMode) {
                    case SET_TEMP_HIGH:
                        if(g_temp_HT < 100) g_temp_HT++;
                        break;
                    case SET_TEMP_LOW:
                        if(g_temp_LT < g_temp_HT) g_temp_LT++;
                        break;
                    case SET_HUMI_HIGH:
                        if(g_humi_HT < 100) g_humi_HT++;
                        break;
                    case SET_HUMI_LOW:
                        if(g_humi_LT < g_humi_HT) g_humi_LT++;
                        break;
                    case SET_SOIL_HUMI_HIGH:
                        if(g_SoilHumi_HT < 100) g_SoilHumi_HT++;
                        break;
                    case SET_SOIL_HUMI_LOW:
                        if(g_SoilHumi_LT < g_SoilHumi_HT) g_SoilHumi_LT++;
                        break;
                    case SET_LIGHT_HIGH:
                        if(g_Light_HT < 100) g_Light_HT++;
                        break;
                    case SET_LIGHT_LOW:
                        if(g_Light_LT < g_Light_HT) g_Light_LT++;
                        break;
                    default:
                        break;
                }
            }
            break;
        
        case KEY_DECREASE_PRESSED:
            if(workMode == MANUAL_MODE) {
                g_SPK = !g_SPK;
            } else {
                switch(currentMode) {
                    case SET_TEMP_HIGH:
                        if(g_temp_HT > g_temp_LT) g_temp_HT--;
                        break;
                    case SET_TEMP_LOW:
                        if(g_temp_LT > 0) g_temp_LT--;
                        break;
                    case SET_HUMI_HIGH:
                        if(g_humi_HT > g_humi_LT) g_humi_HT--;
                        break;
                    case SET_HUMI_LOW:
                        if(g_humi_LT > 0) g_humi_LT--;
                        break;
                    case SET_SOIL_HUMI_HIGH:
                        if(g_SoilHumi_HT > g_SoilHumi_LT) g_SoilHumi_HT--;
                        break;
                    case SET_SOIL_HUMI_LOW:
                        if(g_SoilHumi_LT > 0) g_SoilHumi_LT--;
                        break;
                    case SET_LIGHT_HIGH:
                        if(g_Light_HT > g_Light_LT) g_Light_HT--;
                        break;
                    case SET_LIGHT_LOW:
                        if(g_Light_LT > 0) g_Light_LT--;
                        break;
                    default:
                        break;
                }
            }
            break;
            
        default:
            break;
		}

        if(currentMode == NORMAL_MODE)
        {
            sprintf(showchar, "T:%2dC S:%2d/%-2d%%",g_temp, g_SoilHumi, g_SoilHumi_LT);        
            LCD_PrintString(1, 1, showchar);
            sprintf(showchar, "H:%2d%% L:%2d/%-2dLux",g_humi, g_Light, g_Light_LT);        
            LCD_PrintString(2, 1, showchar);
        }
        else{
            // 显示逻辑（带闪烁效果）
            switch(currentMode) {
                case SET_TEMP_HIGH:
                    sprintf(showchar, "Set Temp_H:%2dC ", g_temp_HT);
                    break;
                case SET_TEMP_LOW:
                    sprintf(showchar, "Set Temp_L:%2dC ", g_temp_LT);
                    break;
                case SET_HUMI_HIGH:
                    sprintf(showchar, "Set Humi_H:%2d%%", g_humi_HT);
                    break;
                case SET_HUMI_LOW:
                    sprintf(showchar, "Set Humi_L:%2d%%", g_humi_LT);
                    break;
                case SET_SOIL_HUMI_HIGH:
                    sprintf(showchar, "SoilHumi_H:%2d%%", g_SoilHumi_HT);
                    break;
                case SET_SOIL_HUMI_LOW:
                    sprintf(showchar, "SoilHumi_L:%2d%% ", g_SoilHumi_LT);
                    break;
                case SET_LIGHT_HIGH:
                    sprintf(showchar, "Light_H:%2dLux ", g_Light_HT);
                    break;
                case SET_LIGHT_LOW:
                    sprintf(showchar, "Light_L:%2dLux  ", g_Light_LT);
                    break;
                default:
                    break;
            }
            LCD_PrintString(1, 1, showchar);
            LCD_PrintString(2, 1, "   [Setting]    ");
        }

	   // 控制逻辑
		if(workMode == AUTO_MODE) {
			// 自动模式
            if(g_temp > g_temp_HT || g_temp < g_temp_LT || 
                g_humi > g_humi_HT || g_humi < g_humi_LT) {
                if (Warning_flag == 0)
                {
                    SPK(1);
                    delay_ms(1000);
                    SPK(0);
					if(g_humi > g_humi_HT || g_humi > g_temp_HT)
					{
						g_SPK = 1;
					}
                    Warning_flag = 1;
                }
             } else if (g_temp < g_temp_HT && g_temp > g_temp_LT && 
                g_humi < g_humi_HT && g_humi > g_humi_LT){
                    Warning_flag = 0;
             }
				
			if(g_humi < g_humi_HT && g_humi < g_temp_HT)
			{
				g_SPK = 0;
			}
             
             if(g_SoilHumi < g_SoilHumi_LT) {
                 g_JD = 1;
             } else if(g_SoilHumi > g_SoilHumi_LT) {
                 g_JD = 0;
             }
             
              if(g_Light < g_Light_LT) {
                  g_LED = 1;
              } else if(g_Light >= g_Light_LT) {
                  g_LED = 0;
              }
            //if(currentMode ==  NORMAL_MODE)
			    LCD_PrintString(1,16,"A");
		}
		else
		{
            //if(currentMode ==  NORMAL_MODE)
			    LCD_PrintString(1,16,"M");
		}
			
        // 机智云协议处理
		userHandle();
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
		
		delay_ms(20);
        
        JD(g_JD);
        //SPK(g_SPK);
		Fan(g_SPK);
        LED(!g_LED);
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
	
	//Fan
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = Fan_Pin;
	GPIO_Init(Fan_GPIO,&GPIO_InitStructure);
}

