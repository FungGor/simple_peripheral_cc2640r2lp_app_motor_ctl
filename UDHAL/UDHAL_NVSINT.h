/*******************************************************************
 * UDHAL_NVSINT.h
 *
 * Description:     header file for NVS internal
 *
 *  Created on: 21 Mar 2023
 *      Author: Chee
 *
 *******************************************************************/

#ifndef UDHAL_UDHAL_NVSINT_H_
#define UDHAL_UDHAL_NVSINT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
/* Driver Header files */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
/* Example/Board Header files */
#include "Board.h"

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

extern void UDHAL_NVSINT_init(void);
extern void UDHAL_NVSINT_params_init(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* UDHAL_UDHAL_NVSINT_H_ */
