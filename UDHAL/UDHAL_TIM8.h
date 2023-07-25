/*
 * UDHAL_TIM8.h
 * This library is used for lightcontrol.h to counter the ambient light sampling duration
 *  Created on:   06 Nov 2022 by Chee Tong
 *  Last Updated: 06 Nov 2022 by Chee Tong
 */

#ifndef UDHAL_UDHAL_TIM8_H_
#define UDHAL_UDHAL_TIM8_H_

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
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * FUNCTIONS
 */
extern void UDHAL_TIM8_init();
extern void UDHAL_TIM8_params_init();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_UDHAL_TIM8_H_ */
