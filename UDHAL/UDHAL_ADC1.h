/*
 *  UDHAL_ADC1.h
 *  ADC1 is assigned for reading brake ADC signals
 *
 *  Created on: 14 Feb 2022
 *      Author: harrysiu
 */

#ifndef UDHAL_UDHAL_ADC1_H_
#define UDHAL_UDHAL_ADC1_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define MC_RT_TIMEOUT_PERIOD     500 //Retransmission time out period. Used by STM32MCP.
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * FUNCTIONS
 */
extern void UDHAL_ADC1_init();
extern void UDHAL_ADC1_params_init();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_UDHAL_ADC1_H_ */
