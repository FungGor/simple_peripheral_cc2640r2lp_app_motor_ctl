#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "simple_peripheral.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <icall.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/GPIO.h>

/*********************************************************************
*  EXTERNAL VARIABLES
*/
/*********************************************************************
 * CONSTANTS
 */
#define SBP_MC_GATT_EVT                        0x0020
#define SBP_MC_ADV_EVT                         0x0040

/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * FUNCTIONS
 */
extern void motorcontrol_init(void);
extern void motorcontrol_registerCB(simplePeripheral_bleCBs_t *obj);
extern void motorcontrol_setGatt(uint16_t serviceUUID, uint8_t charteristics, uint8_t payloadLength, uint8_t* payload);
extern void motorcontrol_speedModeChgCB(uint16_t torqueIQ, uint16_t allowableSpeed, uint16_t rampRate);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEPERIPHERAL_H */
