#include "stm32f10x.h"
#include "delay.h"

// 全局变量
u8 g_temp;
u8 g_humi;
int threshold = 100;  // 默认阈值

#define V0_ADC 190   // 洁净空气ADC值
#define REF_ADC 2700 // 1000ppm时的ADC值（需标定）
#define REF_PPM 1000.0f

// 函数声明

int main(void)
{
//    DHT11_Init();
//    AD_Init();           // AD需要放在串口前初始化不然串口失灵
	
//	TIM4_Init(1000-1, 72-1);  // 初始化1ms定时为机智云协议提供时基
//	TIM_Cmd(TIM4, ENABLE);    // 启动定时器
	


//    //机智云初始化
//    userInit();
//    delay_ms(300);
//    gizwitsInit();
//    delay_ms(300);

//    gizwitsSetMode(WIFI_SOFTAP_MODE);
    
    while(1) {
        //ret = DHT11_Read_Data(&g_temp, &g_humi);
		
		
//        // 机智云协议处理
//        userHandle();
//        gizwitsHandle((dataPoint_t *)&currentDataPoint);

        //u1_printf("Temp: %d Humi: %d PM25: %0.2f MQ2: %0.2f MQ135: %0.2f\r\n", 
        //         g_temp, g_humi, g_PM25, g_MQ2, g_MQ135);
        delay_ms(100);
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
