/******************************************************************************

 @file  UDHAL_TIM.c

 @brief This library is used for singleButton.h to counter the button press duration


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>
#include "UDHAL/UDHAL_TIM3.h"
#include "singleButton/singleButton.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
static Clock_Handle ClockHandle;
static Clock_Params clkParams;
static uint32_t clockTicks;
static Error_Block eb;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_TIM3_start();
static void UDHAL_TIM3_setPeriod(uint32_t clockTimeout);
static void UDHAL_TIM3_stop();
static void UDHAL_TIM3_OVClockFxn();
/*********************************************************************
 * Marco
 */
static singleButton_timerManager_t singleButton_timer =
{
    UDHAL_TIM3_start,
    UDHAL_TIM3_setPeriod,
    UDHAL_TIM3_stop
};
/*********************************************************************
 *
 * @fn      UDHAL_TIM3_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM3_init()
{
    Error_init(&eb);
    clockTicks = SINGLE_BUTTON_TIMER_OV_TIME_LONG * (1000 / Clock_tickPeriod) - 1;  // -1 ensures Overflow occurs at SINGLE_BUTTON_TIMER_OV_TIME_LONG - not 1 tick after SINGLE_BUTTON_TIMER_OV_TIME_LONG
    ClockHandle = Clock_create (UDHAL_TIM3_OVClockFxn, clockTicks, &clkParams, &eb);

    singleButton_registerTimer(&singleButton_timer); // renamed from "timer" to "singleButton_timer"
}
/*********************************************************************
 *
 * @fn      UDHAL_TIM3_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM3_params_init()
{
    Clock_Params_init(&clkParams);
    clkParams.period =  0;
    clkParams.startFlag = FALSE;
    clkParams.arg = (UArg)0x0000;
}
/*********************************************************************
 * @fn      UDHAL_TIM3_start
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM3_start()
{
   // Set the initial timeout
    Clock_start(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM3_setPeriod
 *
 * @brief   It is used by singleButtonUtil.c to set the period of the clock, change the configuration to meet the requirement
 *          It is used to set the timer overflow time, change the configuration to meet the requirement
 *
 * @param   clockTimeout  it takes the time in milliseconds. Please convert it to meet your device
 *
 *
 * @return  none
 */
static void UDHAL_TIM3_setPeriod(uint32_t clockTimeout)
{
    uint32_t ticks = clockTimeout * (1000 / Clock_tickPeriod) - 1; // -1 ensures Overflow occurs at clockTimeout - not 1 tick after clockTimeout
    Clock_setTimeout(ClockHandle, ticks);
}
/*********************************************************************
 * @fn      UDHAL_TIM3_stop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM3_stop()
{
    Clock_stop(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM3_OVClockFxn
 *
 * @brief   After timeout, retransmission will be sent
 *          You must add STM32MCP_retransmission to this function
 *
 * @param   none
 *
 * @return  none
 */
static void UDHAL_TIM3_OVClockFxn()
{
    singleButton_processTimerOv();
}
