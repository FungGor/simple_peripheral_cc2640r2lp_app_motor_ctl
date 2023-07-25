/*********************************************************************************************
 * buzzerControl.h
 *
 * Description:
 *
 *
 *  Created on: 14 Nov 2022
 *      Author: Chee
 *
 *********************************************************************************************/

#ifndef APPLICATION_BUZZERCONTROL_H_
#define APPLICATION_BUZZERCONTROL_H_

#ifdef _cplusplus
extern "C"
{
#endif
//
/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stddef.h>
/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
* CONSTANTS
*/
// Battery low beeps starts after the following duration (in milliseconds)
#define BUZZER_BATTERYLOW_BEEP_DURATION                50

//  BUZZER repeat beeps at the following period (in milliseconds)
#define BUZZER_BATTERYLOW_BEEP_PERIOD                  30000


typedef void (*buzzerControl_timerStart)(void);
typedef void (*buzzerControl_timerStop)(void);
typedef struct{
    buzzerControl_timerStart timerStart;    // this is a function pointer
    buzzerControl_timerStop timerStop;      // this is a function pointer
}buzzerControl_timerManager_t;


typedef void (*buzzerControlCB_t)(uint8_t messageID);
typedef struct{
    buzzerControlCB_t buzzerControlCB_t;    // this is a function pointer
}buzzerControlCBs_t;

extern void buzzerControl_init();
extern void buzzerControl_registerTimer(buzzerControl_timerManager_t *deviceTimer);
extern void buzzerControl_registerCBs(buzzerControlCBs_t *buzzerControlCBs);
extern void buzzerControl_processTimerOV();
extern void buzzerControl_Start( void );
extern void buzzerControl_Stop( void );

#ifdef _cplusplus
}
#endif

#endif /* APPLICATION_BUZZERCONTROL_H_ */
