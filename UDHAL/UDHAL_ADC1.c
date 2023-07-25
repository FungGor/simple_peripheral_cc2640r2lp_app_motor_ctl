/*
 *  UDHAL_ADC1.c
 *  ADC1 is assigned for reading brake ADC signals
 *
 *  Created on: 14 Feb 2022
 *      Author: harrysiu
 */

/*********************************************************************
 * INCLUDES
 */

#include <ti/drivers/ADC.h>
#include <stdint.h>
#include "UDHAL/UDHAL_ADC1.h"
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
static void UDHAL_ADC1_Open();
static void UDHAL_ADC1_Convert(uint16_t *result);
static void UDHAL_ADC1_Close();
/*********************************************************************
 * Marco
 */
static brakeAndThrottle_adcManager_t brake_adcManager =
{
     UDHAL_ADC1_Open,
     UDHAL_ADC1_Convert,
     UDHAL_ADC1_Close
};
/*********************************************************************
 *
 * @fn      UDHAL_ADC1_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_ADC1_init()
{
    ADC_init();
    brakeAndThrottle_registerADC1(&brake_adcManager);
}
/*********************************************************************
 *
 * @fn      UDHAL_ADC1_params_init
 *
 * @brief   To initialize the timer and uart tx function
 *
 * @param   None.
 *
 * @return  None.
 */
void UDHAL_ADC1_params_init()
{
    ADC_Params_init(&params);
    params.isProtected = true;
}
/*********************************************************************
 * @fn      UDHAL_ADC1_Open
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC1_Open()
{
    adcHandle = ADC_open(Board_ADC0, &params);
}
/*********************************************************************
 * @fn      UDHAL_ADC1_Convert
 *
 * @brief   To start the timer for timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC1_Convert(uint16_t *result)
{
    ADC_convert(adcHandle, result);
}
/*********************************************************************
 * @fn      UDHAL_ADC1_Close
 *
 * @brief   To stop the timer for flow control timeout.
 *          This function will be used by STM32MCP flow retransmission.
 *          Put it into the STM32MCP_params_t structure to register as callback function.
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_ADC1_Close()
{
    ADC_close(adcHandle);
}
