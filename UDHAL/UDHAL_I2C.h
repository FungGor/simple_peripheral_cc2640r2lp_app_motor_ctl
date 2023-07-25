/*
 * UDHAL_I2C.h
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *
 *
 *
 *                                          Before you start using this library, you must read UM1052(Pg161-178).
 *
 *
 *
 *
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *  Created on:   20 Aug 2021 by Siu Yeung Yik
 *  Last Updated: 22 Oct 2021 by Siu Yeung Yik
 */

#ifndef UDHAL_I2C_UDHAL_I2C_H_
#define UDHAL_I2C_UDHAL_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <ti/drivers/I2C.h>
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
extern void UDHAL_I2C_init(void);
extern void UDHAL_I2C_params_init(void);
extern uint8_t UDHAL_I2C_getOpenStatus(void);
extern void UDHAL_I2C_CallbackFxn(I2C_Handle i2c_Handle, I2C_Transaction *i2c_Transaction, bool i2c_TransferStatus);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UDHAL_I2C_UDHAL_I2C_H_ */
