/**********************************************************************************************
 * powerOnTime.h
 *
 * Description:    This file contains the Dashboard service definitions and
 *                 prototypes.
 *
 *  Created on: 30 Jan 2023
 *      Author: Chee
 *
 **********************************************************************************************/

#ifndef APPLICATION_POWERONTIME_H_
#define APPLICATION_POWERONTIME_H_
//
#ifdef __cplusplus
extern "C"
{
#endif
//
/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
* CONSTANTS
*/
#define MINUTE_IN_HOUR                                  60  // 60 minutes in an hour
#define POWERONTIME_MINUTE_TIME                         60000

/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
*********************************************************************/
extern void powerOnTime_init();
extern uint16_t powerOnTime_getPowerOnTime();
extern void powerOnTime_cal( uint32_t powerOnTime_ms );

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_POWERONTIME_H_ */
