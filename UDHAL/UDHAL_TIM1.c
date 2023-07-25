/******************************************************************************

 @file  UDHAL_TIM1.c

 @brief TIM1 is used for STM32MCP.h flow control retransmission


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>
#include "STM32MCP/STM32MCP.h"
#include "UDHAL/UDHAL_TIM1.h"
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
static void UDHAL_TIM1_timerStart();
static void UDHAL_TIM1_timerResetCounter();
static void UDHAL_TIM1_timerStop();
static void UDHAL_TIM1_OVClockFxn();
/*********************************************************************
 * Marco
 */
static STM32MCP_timerManager_t timerManager =
{
     UDHAL_TIM1_timerStart,
     UDHAL_TIM1_timerResetCounter,
     UDHAL_TIM1_timerStop
};
/*********************************************************************
 *
 * @fn      UDHAL_TIM1_Init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM1_init()
{
    Error_init(&eb);
    clockTicks = MC_RT_TIMEOUT_PERIOD * (1000 / Clock_tickPeriod) - 1;  // -1 to ensure overflow occurs at MC_RT_TIMEOUT_PERIOD - not at 1 tick after MC_RT_TIMEOUT_PERIOD
    ClockHandle = Clock_create(UDHAL_TIM1_OVClockFxn, clockTicks, &clkParams, &eb);
    STM32MCP_registerTimer(&timerManager);
    Clock_setTimeout(ClockHandle, clockTicks);
}
/*********************************************************************
 *
 * @fn      UDHAL_TIM1_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_TIM1_params_init()
{
    Clock_Params_init(&clkParams);
    clkParams.period = clockTicks;
    clkParams.startFlag = FALSE;
    clkParams.arg = (UArg)0x0000;
}
/*********************************************************************
 * @fn      UDHAL_TIM1_timerStart
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM1_timerStart()
{
   // Set the initial timeout
   Clock_start(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM1_timerStop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM1_timerResetCounter()
{
    Clock_setPeriod(ClockHandle, clockTicks);
    Clock_setTimeout(ClockHandle, clockTicks);
}
/*********************************************************************
 * @fn      UDHAL_TIM1_timerStop
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_TIM1_timerStop()
{
    Clock_stop(ClockHandle);
}
/*********************************************************************
 * @fn      UDHAL_TIM1_OVClockFxn
 *
 * @brief   After timeout, retransmission will be sent
 *          You must add STM32MCP_retransmission to this function
 *
 * @param   none
 *
 * @return  none
 */
static void UDHAL_TIM1_OVClockFxn()
{
    STM32MCP_retransmission();
}
