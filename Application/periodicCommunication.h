/*
 * periodicCommunication.h
 *
 *  Created on: 10 Feb 2022
 *      Author: harrysiu
 */
#ifndef PERIODICCOMMUNICATION_H
#define PERIODICCOMMUNICATION_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "simple_peripheral.h"
//#include <icall.h>
//#include <string.h>
//#include <stdlib.h>
/*********************************************************************
*  EXTERNAL VARIABLES
*/
/*********************************************************************
 * CONSTANTS
 */
//  PERIODIC_COMMUNICATION_HF_SAMPLING_TIME = 400 ms is the optimum time based on tests
#define PERIODIC_COMMUNICATION_HF_SAMPLING_TIME      300    //400    // This must be the same as DATA_ANALYSIS_SAMPLING_TIME = 400ms
// DATA_ANALYSIS_POINTS must be an odd number (due to numerical integration using Simpson's 1/3 rule)
#define DATA_ANALYSIS_POINTS                         17     //13     // Must satisfy the equation:  (DATA_ANALYSIS_POINTS - 1) x DATA_ANALYSIS_SAMPLING_TIME = (21 - 1) * 400 ms = 8000 ms
                                                            // Must satisfy the equation:  (DATA_ANALYSIS_POINTS - 1) x DATA_ANALYSIS_SAMPLING_TIME = (13 - 1) * 400 ms = 4800 ms
                                                            // (17 - 1) * 300 ms = 4800 ms
#define PERIODIC_COMMUNICATION_LF_SAMPLING_TIME      60000
#define PERIODIC_COMMUNICATION_ACTIVATE              0x01
#define PERIODIC_COMMUNICATION_DEACTIVATE            0x00
/*********************************************************************
 * MACROS
 */
typedef void (*motorcontrol_timerStart)(void);
typedef void (*motorcontrol_timerSetPeriod)(uint32_t timerPeriod);
typedef void (*motorcontrol_timerStop)(void);
typedef struct
{
    motorcontrol_timerStart timerStart;
    motorcontrol_timerSetPeriod timerSetPeriod;
    motorcontrol_timerStop  timerStop;
}motorcontrol_timerManager_t;


/*********************************************************************
 * FUNCTIONS
 */
/*
 * Task creation function for the Simple Peripheral.
 */
extern void periodicCommunication_start();
extern void periodicCommunication_stop();
extern void periodicCommunication_toggle();
extern void periodicCommunication_register_hfTimer(motorcontrol_timerManager_t *obj);
extern void periodicCommunication_register_lfTimer(motorcontrol_timerManager_t *obj);
extern void periodicCommunication_registerBLE_Gatt(simplePeripheral_bleCBs_t *obj);
extern void periodicCommunication_hf_communication();
extern void periodicCommunication_lf_communication();
extern uint8_t periodicCommunication_getxlf();
extern uint8_t periodicCommunication_getxhf();
//extern void periodicCommunication_setGatt(uint16_t serviceUUID, uint8_t charteristics, uint8_t payloadLength, uint8_t* payload)

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PERIODICCOMMUNICATION_H */
