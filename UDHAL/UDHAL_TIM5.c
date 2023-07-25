/******************************************************************************

 @file  UDHAL_TIM5.c

 @brief This library is used for low frequency communication
 (Timer 5 is not required and will be replaced and removed after amendment is completed)


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>
#include "UDHAL/UDHAL_TIM5.h"
#include "periodicCommunication.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
static Clock_Handle ClockHandle;
static Clock_Params clkParams;
static uint32_t clockTicks;
static Error_Block eb;
//static uint16_t PERIODIC_COMMUNICATION_LF_SAMPLING_TIME;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_TIM5_start();
static void UDHAL_TIM5_setPeriod(uint32_t clockTimeout);
static void UDHAL_TIM5_stop();
static void UDHAL_TIM5_OVClockFxn();
/*********************************************************************
 * Marco
 */
static motorcontrol_timerManager_t timer5 =
{
    UDHAL_TIM5_start,
    UDHAL_TIM5_setPeriod,
    UDHAL_TIM5_stop
};
/*********************************************************************
 *
 * @fn      UDHAL_TIM5_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM5_init()
{
    uint16_t periodicCommunicationLFSamplingTime;
    Error_init(&eb);
    periodicCommunicationLFSamplingTime = PERIODIC_COMMUNICATION_HF_SAMPLING_TIME * (DATA_ANALYSIS_POINTS - 1);
    clockTicks = periodicCommunicationLFSamplingTime * (1000 / Clock_tickPeriod) - 1; // -1 to ensure overflow occurs at PERIODIC_COMMUNICATION_LF_SAMPLING_TIME - not at 1 tick after PERIODIC_COMMUNICATION_LF_SAMPLING_TIME
    ClockHandle = Clock_create(UDHAL_TIM5_OVClockFxn, clockTicks, &clkParams, &eb);
    periodicCommunication_register_lfTimer(&timer5);

}
/*********************************************************************
 *
 * @fn      UDHAL_TIM5_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM5_params_init()
{

    Clock_Params_init(&clkParams);
    clkParams.period = clockTicks;
    clkParams.startFlag = FALSE;
    clkParams.arg = (UArg)0x0000;
    Clock_setTimeout(ClockHandle, clockTicks);
    Clock_setPeriod(ClockHandle, clockTicks);

}
/*********************************************************************
 * @fn      UDHAL_TIM5_start
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM5_start()
{
   // Set the initial timeout
    Clock_start(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM5_setPeriod
 *
 * @brief   It is used by singleButtonUtil.c to set the period of the clock, change the configuration to meet the requirement
 *          It is used to set the timer overflow time, change the configuration to meet the requirement
 *
 * @param   clockTimeout  it takes the time in milliseconds. Please convert it to meet your device
 *
 *
 * @return  none
 */
static void UDHAL_TIM5_setPeriod(uint32_t clockTimeout)
{
    uint32_t ticks = clockTimeout * (1000 / Clock_tickPeriod) - 1;  // -1 to ensure overflow occurs at clockTimeout - not at 1 tick after clockTimeout
    Clock_setTimeout(ClockHandle, ticks);
}
/*********************************************************************
 * @fn      UDHAL_TIM5_stop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM5_stop()
{
    Clock_stop(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM5_OVClockFxn
 *
 * @brief   After timeout, retransmission will be sent
 *          You must add STM32MCP_retransmission to this function
 *
 * @param   none
 *
 * @return  none
 */
static void UDHAL_TIM5_OVClockFxn()
{
    periodicCommunication_lf_communication();
}
