/*
 * UDHAL_TIM1.h
 *  TIM1 is used for STM32MCP.h flowcontrol retransmission
 *  Created on:   25 Jan 2021 by Siu Yeung Yik
 *  Last Updated: 25 Jan 2021 by Siu Yeung Yik
 */
#ifndef UDHAL_TIM_UDHAL_TIM_H_
#define UDHAL_TIM_UDHAL_TIM_H_

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
extern void UDHAL_TIM1_init();
extern void UDHAL_TIM1_params_init();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_TIM_UDHAL_TIM_H_ */
