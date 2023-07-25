/******************************************************************************

 @file  UDHAL_TIM4.c

 @brief This library is used for high frequency communication


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "UDHAL/UDHAL_TIM4.h"
#include "periodicCommunication.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>
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
static void UDHAL_TIM4_start();
static void UDHAL_TIM4_setPeriod(uint32_t clockTimeout);
static void UDHAL_TIM4_stop();
static void UDHAL_TIM4_OVClockFxn();
/*********************************************************************
 * Marco
 */
static motorcontrol_timerManager_t timer4 =
{
    UDHAL_TIM4_start,
    UDHAL_TIM4_setPeriod,
    UDHAL_TIM4_stop
};
/*********************************************************************
 *
 * @fn      UDHAL_TIM4_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM4_init()
{
    Error_init(&eb);
    clockTicks = PERIODIC_COMMUNICATION_HF_SAMPLING_TIME * (1000 / Clock_tickPeriod) - 1; // -1 to ensure overflow occurs at PERIODIC_COMMUNICATION_HF_SAMPLING_TIME - not at 1 tick after PERIODIC_COMMUNICATION_HF_SAMPLING_TIME
    ClockHandle = Clock_create(UDHAL_TIM4_OVClockFxn, clockTicks, &clkParams, &eb);
    periodicCommunication_register_hfTimer(&timer4);

}
/*********************************************************************
 *
 * @fn      UDHAL_TIM4_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM4_params_init()
{

    Clock_Params_init(&clkParams);
    clkParams.period = clockTicks;
    clkParams.startFlag = FALSE;
    clkParams.arg = (UArg)0x0000;
    Clock_setTimeout(ClockHandle, clockTicks);
    Clock_setPeriod(ClockHandle, clockTicks);

}
/*********************************************************************
 * @fn      UDHAL_TIM4_start
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM4_start()
{
   // Set the initial timeout
    Clock_start(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM4_setPeriod
 *
 * @brief   It is used by singleButtonUtil.c to set the period of the clock, change the configuration to meet the requirement
 *          It is used to set the timer overflow time, change the configuration to meet the requirement
 *
 * @param   clockTimeout  it takes the time in milliseconds. Please convert it to meet your device
 *
 *
 * @return  none
 */
static void UDHAL_TIM4_setPeriod(uint32_t clockTimeout)
{
    uint32_t ticks = clockTimeout * (1000 / Clock_tickPeriod) - 1; // -1 to ensure overflow occurs at clockTimeout - not at 1 tick after clockTimeout
    Clock_setTimeout(ClockHandle, ticks);
}
/*********************************************************************
 * @fn      UDHAL_TIM4_stop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM4_stop()
{
    Clock_stop(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM4_OVClockFxn
 *
 * @brief   After timeout, retransmission will be sent
 *          You must add STM32MCP_retransmission to this function
 *
 * @param   none
 *
 * @return  none
 */
static void UDHAL_TIM4_OVClockFxn()
{
    periodicCommunication_hf_communication();
    // next step -> go to dataAnalysis.c
}
