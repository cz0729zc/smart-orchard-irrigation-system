/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "main.h"

static uint32_t timerMsCount;

/** Current datapoint */
dataPoint_t currentDataPoint;

extern u8 g_temp;
extern u8 g_humi;
extern u8 g_SoilHumi;
extern u8 g_Light;

extern u8 g_SPK;
extern u8 g_JD;
extern u8 g_LED;

extern u8 g_temp_HT;
extern u8 g_temp_LT;
extern u8 g_humi_HT;
extern u8 g_humi_LT;
extern u8 g_SoilHumi_HT;
extern u8 g_SoilHumi_LT;
extern u8 g_Light_HT;
extern u8 g_Light_LT;

extern WorkMode workMode;
/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
    uint8_t i = 0;
    dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
    moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
    protocolTime_t *ptime = (protocolTime_t *)gizdata;
    
#if MODULE_TYPE
    gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
    moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

    if((NULL == info) || (NULL == gizdata))
    {
        return -1;
    }

    for(i=0; i<info->num; i++)
    {
        switch(info->event[i])
        {
        case EVENT_SPK:
            currentDataPoint.valueSPK = dataPointPtr->valueSPK;
            GIZWITS_LOG("Evt: EVENT_SPK %d \n", currentDataPoint.valueSPK);
            if(0x01 == currentDataPoint.valueSPK)
            {
            //user handle
            g_SPK = 1;
            }
            else
            {
            //user handle  
            g_SPK = 0;  
            }
            break;
        case EVENT_JD:
            currentDataPoint.valueJD = dataPointPtr->valueJD;
            GIZWITS_LOG("Evt: EVENT_JD %d \n", currentDataPoint.valueJD);
            if(0x01 == currentDataPoint.valueJD)
            {
            //user handle
            g_JD = 1; 
            }
            else
            {
            //user handle   
            g_JD = 0;  
            }
            break;
        case EVENT_LED:
            currentDataPoint.valueLED = dataPointPtr->valueLED;
            GIZWITS_LOG("Evt: EVENT_LED %d \n", currentDataPoint.valueLED);
            if(0x01 == currentDataPoint.valueLED)
            {
            //user handle
            g_LED = 1;
            }
            else
            {
            //user handle    
            g_LED = 0;
            }
            break;

        case EVENT_ModeButton:
            currentDataPoint.valueModeButton = dataPointPtr->valueModeButton;
            GIZWITS_LOG("Evt: EVENT_ModeButton %d\n", currentDataPoint.valueModeButton);
            switch(currentDataPoint.valueModeButton)
            {
            case ModeButton_VALUE0:
                //user handle
                workMode = AUTO_MODE;
                break;
            case ModeButton_VALUE1:
                //user handle
                workMode = MANUAL_MODE;
                break;
            default:
                break;
            }
            break;

       case EVENT_TempTH:
            currentDataPoint.valueTempTH = dataPointPtr->valueTempTH;
            GIZWITS_LOG("Evt:EVENT_TempTH %d\n",currentDataPoint.valueTempTH);
            //user handle
            g_temp_HT =  currentDataPoint.valueTempTH;
            break;
        case EVENT_TempLH:
            currentDataPoint.valueTempLH = dataPointPtr->valueTempLH;
            GIZWITS_LOG("Evt:EVENT_TempLH %d\n",currentDataPoint.valueTempLH);
            //user handle
            g_temp_LT =  currentDataPoint.valueTempLH;
            break;
        case EVENT_HumiHT:
            currentDataPoint.valueHumiHT = dataPointPtr->valueHumiHT;
            GIZWITS_LOG("Evt:EVENT_HumiHT %d\n",currentDataPoint.valueHumiHT);
            //user handle
            g_humi_HT =  currentDataPoint.valueHumiHT;
            break;
        case EVENT_HumiLT:
            currentDataPoint.valueHumiLT = dataPointPtr->valueHumiLT;
            GIZWITS_LOG("Evt:EVENT_HumiLT %d\n",currentDataPoint.valueHumiLT);
            //user handle
            g_humi_LT =  currentDataPoint.valueHumiLT;
            break;
        case EVENT_SoilHumiHT:
            currentDataPoint.valueSoilHumiHT = dataPointPtr->valueSoilHumiHT;
            GIZWITS_LOG("Evt:EVENT_SoilHumiHT %d\n",currentDataPoint.valueSoilHumiHT);
            //user handle
            g_SoilHumi_HT =  currentDataPoint.valueSoilHumiHT;
            break;
        case EVENT_SoilHumiLT:
            currentDataPoint.valueSoilHumiLT = dataPointPtr->valueSoilHumiLT;
            GIZWITS_LOG("Evt:EVENT_SoilHumiLT %d\n",currentDataPoint.valueSoilHumiLT);
            //user handle
            g_SoilHumi_LT =  currentDataPoint.valueSoilHumiLT;
            break;
        case EVENT_LightHT:
            currentDataPoint.valueLightHT = dataPointPtr->valueLightHT;
            GIZWITS_LOG("Evt:EVENT_LightHT %d\n",currentDataPoint.valueLightHT);
            //user handle
            g_Light_HT =  currentDataPoint.valueLightHT;
            break;
        case EVENT_LightLT:
            currentDataPoint.valueLightLT = dataPointPtr->valueLightLT;
            GIZWITS_LOG("Evt:EVENT_LightLT %d\n",currentDataPoint.valueLightLT);
            //user handle
            g_Light_LT =  currentDataPoint.valueLightLT;
            break;


        case WIFI_SOFTAP:
            break;
        case WIFI_AIRLINK:
            break;
        case WIFI_STATION:
            break;
        case WIFI_CON_ROUTER:
 
            break;
        case WIFI_DISCON_ROUTER:
 
            break;
        case WIFI_CON_M2M:
 
            break;
        case WIFI_DISCON_M2M:
            break;
        case WIFI_RSSI:
            GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
            break;
        case TRANSPARENT_DATA:
            GIZWITS_LOG("TRANSPARENT_DATA \n");
            //user handle , Fetch data from [data] , size is [len]
            break;
        case WIFI_NTP:
            GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
            break;
        case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",gprsInfoData->Type);
#else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
#endif
        break;
        default:
            break;
        }
    }

    return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
    currentDataPoint.valuetemp = g_temp;//Add Sensor Data Collection
    currentDataPoint.valueHumi = g_humi;//Add Sensor Data Collection
    currentDataPoint.valueSoilHumi = g_SoilHumi;//Add Sensor Data Collection
    currentDataPoint.valueLight = g_Light;//Add Sensor Data Collection

    currentDataPoint.valueTempTH = g_temp_HT;
    currentDataPoint.valueTempLH = g_temp_LT;
    currentDataPoint.valueHumiHT = g_humi_HT;
    currentDataPoint.valueHumiLT = g_humi_LT;
    currentDataPoint.valueSoilHumiHT = g_SoilHumi_HT;
    currentDataPoint.valueSoilHumiLT = g_SoilHumi_LT;
    currentDataPoint.valueLightHT = g_Light_HT;
    currentDataPoint.valueLightLT = g_Light_LT;
    currentDataPoint.valueSPK = g_SPK;
    currentDataPoint.valueJD = g_JD;
    currentDataPoint.valueLED = g_LED;
    currentDataPoint.valueModeButton = workMode;
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    currentDataPoint.valueTempTH = g_temp_HT;
    currentDataPoint.valueTempLH = g_temp_LT;
    currentDataPoint.valueHumiHT = g_humi_HT;
    currentDataPoint.valueHumiLT = g_humi_LT;
    currentDataPoint.valueSoilHumiHT = g_SoilHumi_HT;
    currentDataPoint.valueSoilHumiLT = g_SoilHumi_LT;
    currentDataPoint.valueLightHT = g_Light_HT;
    currentDataPoint.valueLightLT = g_Light_LT;
    currentDataPoint.valuetemp = 0;
    currentDataPoint.valueHumi = 0;
    currentDataPoint.valueSoilHumi = 0;
    currentDataPoint.valueLight = 0;

}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef true
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        USART_SendData(USART1, buf[i]);
		//u1_printf("%02x ", buf[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        if(i >=2 && buf[i] == 0xFF)
        {
			USART_SendData(USART1,0x55);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
    }


    
    return len;
}


