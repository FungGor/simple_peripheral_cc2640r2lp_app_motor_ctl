/*
 *  UDHAL_ADC2.c
 *  ADC2 is assigned for reading throttle ADC signals
 *
 *  Created on: 14 Feb 2022
 *      Author: harrysiu
 */

/*********************************************************************
 * INCLUDES
 */
#include <ti/drivers/ADC.h>
#include <stdint.h>
#include "UDHAL/UDHAL_ADC2.h"
#include "brakeAndThrottle.h"
#include "Board.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
static ADC_Handle adcHandle;
static ADC_Params params;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_ADC2_Open();
static void UDHAL_ADC2_Convert(uint16_t *result);
static void UDHAL_ADC2_Close();
/*********************************************************************
 * Marco
 */
static brakeAndThrottle_adcManager_t brake_adcManager =
{
     UDHAL_ADC2_Open,
     UDHAL_ADC2_Convert,
     UDHAL_ADC2_Close
};
/*********************************************************************
 *
 * @fn      UDHAL_ADC2_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_ADC2_init()
{
    ADC_init();
    brakeAndThrottle_registerADC2(&brake_adcManager);
}
/*********************************************************************
 *
 * @fn      UDHAL_ADC2_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_ADC2_params_init()
{
    ADC_Params_init(&params);
    params.isProtected = true;
}
/*********************************************************************
 * @fn      UDHAL_ADC2_Open
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC2_Open()
{
    adcHandle = ADC_open(Board_ADC1, &params);
}
/*********************************************************************
 * @fn      UDHAL_ADC2_Convert
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC2_Convert(uint16_t *result)
{
    ADC_convert(adcHandle, result);
}
/*********************************************************************
 * @fn      UDHAL_ADC2_Close
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC2_Close()
{
    ADC_close(adcHandle);
}
