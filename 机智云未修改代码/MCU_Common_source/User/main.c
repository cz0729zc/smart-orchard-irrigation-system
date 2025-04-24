/**
************************************************************
* @file         main.c
* @brief        MCU entry file
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
/********************************************************
** Code migration instructions
*
* 1、Serial drive for Module communication,（1、Interrupt receive data to write to the ring buffer ;2、实现uartWrite() Serial send function）
* 2、Serial print function , printf()
* 3、MS timer，gizTimerMs() maintain system time
* 4、MCU Reset function. The module can request MCU reset
* 5、Configure network function,use gizwitsSetMode() , realization module distribution network function
* 6、userHandle() , data acquisition（Uplink logic）
* 7、gizwitsEventProcess() , Specific execution of control commands (Downlink logic)
* 
*
*********************************************************/

#include <string.h>
#include "gizwits_product.h"
#include "common.h"

/**
* @brief main

* In this function, the user - associated initialization and the main logical loop are completed

* @param none
* @return none
*/
int main(void)
{  
    userInit();

    gizwitsInit();
    
    while(1)
    {
        userHandle();
        
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
    }
}

