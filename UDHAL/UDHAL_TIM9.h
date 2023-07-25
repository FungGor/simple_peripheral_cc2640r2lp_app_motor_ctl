/*
 * UDHAL_TIM9.h
 * This library is used for buzzerControl.h to counter the beeping frequency of the buzzer in battery low state
 *  Created on:   06 Nov 2022 by Chee Tong
 *  Last Updated: 06 Nov 2022 by Chee Tong
 */


#ifndef UDHAL_UDHAL_TIM9_H_
#define UDHAL_UDHAL_TIM9_H_

#ifdef _cplusplus
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
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * FUNCTIONS
 */
extern void UDHAL_TIM9_init();
extern void UDHAL_TIM9_params_init();

/*********************************************************************
*********************************************************************/

#ifdef _cplusplus
}
#endif

#endif /* UDHAL_UDHAL_TIM9_H_ */
