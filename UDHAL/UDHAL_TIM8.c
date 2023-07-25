/*
 * UDHAL_TIM8.c
 *  This library is used for lightcontrol.h to counter the ambient light sampling duration
 *  Created on:   06 Nov 2022 by Chee Tong
 *  Last Updated: 06 Nov 2022 by Chee Tong
 */
/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
//#include <Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>
#include "UDHAL/UDHAL_TIM8.h"           //folder/header_file.h
#include "Application/lightControl.h"   //folder/header_file.h
/*********************************************************************
 * LOCAL VARIABLES
 */
static Clock_Handle ClockHandle;
static Clock_Params clkParams;
static uint32_t clockTicks;
static Error_Block eb;

//Hwi_Params hwi_params;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_TIM8_start();
static void UDHAL_TIM8_stop();
static void UDHAL_TIM8_OVClockFxn();
/*********************************************************************
 * Marco
 */
static lightControl_ALStimerManager_t ALS_timer =
{
    UDHAL_TIM8_start,
    UDHAL_TIM8_stop
};
/*********************************************************************
 *
 * @fn      UDHAL_TIM8_init
 *
 * @brief   To initialize the ALS timer and I2C function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM8_init()
{
    Error_init(&eb);    // what is the use of Error?  could it be NULL?
    clockTicks = ALS_SAMPLING_TIME * (1000 / Clock_tickPeriod) - 1;  // -1 to ensure overflow occurs at ALS_SAMPLING_TIME - not at 1 tick after ALS_SAMPLING_TIME
    ClockHandle = Clock_create (UDHAL_TIM8_OVClockFxn, clockTicks, &clkParams, &eb);
    lightControl_registerTimer(&ALS_timer);
}
/*********************************************************************
 *
 * @fn      UDHAL_TIM8_params_init
 *
 * @brief   To initialize the timer and I2C function
 *          Whenever auto mode is selected, ALS sensor automatically starts taking light intensity measurements
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM8_params_init()
{
    Clock_Params_init(&clkParams);
    clkParams.period =  0;  //ALS_SAMPLING_TIME * (1000 / Clock_tickPeriod) - 1;  // subsequent call at this number of ticks // -1 to ensure overflow occurs at ALS_SAMPLING_TIME - not at 1 tick after ALS_SAMPLING_TIME
                            // setting period to "ALS_SAMPLING_TIME * (1000 / Clock_tickPeriod) - 1" does not work!!!  Why doesn't it work?
    clkParams.startFlag = FALSE;
    clkParams.arg = (UArg)0x0000;
}
/*********************************************************************
 * @fn      UDHAL_TIM8_start
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by lightControl in light_MODE_AUTO.
 *
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM8_start()
{
    Clock_start(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM8_stop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by lightControl.
 *          When light mode exits auto mode, use Clock Stop to stop timer and stop sensor from taking measurements.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM8_stop()
{
    Clock_stop(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM8_OVClockFxn
 *
 * @brief   After timeout, this function is called to command ALS sensor to take light intensity measurement
 *
 *
 * @param   none
 *
 * @return  none
 */
static void UDHAL_TIM8_OVClockFxn()
{
    lightControl_timerInterruptHandler();
}
