/*
 * UDHAL_TIM4.h
 *  This library (timer) is used for high frequency communication
 *  Created on:   26 Jan 2021 by Siu Yeung Yik
 *  Last Updated: 26 Jan 2021 by Siu Yeung Yik
 */
#ifndef UDHAL_TIM4_UDHAL_TIM4_H_
#define UDHAL_TIM4_UDHAL_TIM4_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include <xdc/runtime/Error.h>

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * FUNCTIONS
 */
extern void UDHAL_TIM4_init();
extern void UDHAL_TIM4_params_init();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_TIM4_UDHAL_TIM4_H_ */
