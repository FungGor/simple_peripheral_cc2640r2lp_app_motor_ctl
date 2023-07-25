/*
 * UDHAL.h
 *
 *  Created on: 26 Jan 2022
 *      Author: harrysiu
 */
#ifndef UDHAL_UDHAL_H_
#define UDHAL_UDHAL_H_

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
extern void UDHAL_init();
extern uint8_t UDHAL_getI2CStatus();

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_UDHAL_H_ */
