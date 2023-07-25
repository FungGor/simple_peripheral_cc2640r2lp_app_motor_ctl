/*
 *  UDHAL_ADC2.h
 *  ADC2 is assigned for reading throttle ADC signals
 *
 *  Created on: 14 Feb 2022
 *      Author: harrysiu
 */

#ifndef UDHAL_UDHAL_ADC2_H_
#define UDHAL_UDHAL_ADC2_H_

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
extern void UDHAL_ADC2_init();
extern void UDHAL_ADC2_params_init();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_UDHAL_ADC2_H_ */
