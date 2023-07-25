/*
 * singleButton.h
 *
 *  Created on: 18 Jan 2022
 *      Author: harrysiu
 */

#ifndef SINGLEBUTTON_H_
#define SINGLEBUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
//Constants for timing duration
#define SINGLE_BUTTON_TIMER_OV_TIME_LONG           1500  //Time expressed in ms, overflow time interval for single long press
#define SINGLE_BUTTON_TIMER_OV_TIME_SHORT          500   //Time expressed in ms, overflow time interval for short press

//Button state
#define SINGLE_BUTTON_WAITING_STATE                0x00
#define SINGLE_BUTTON_EXECUTING_STATE              0x01

//constants for SINGLE_BUTTON processed messageID
#define SINGLE_BUTTON_SINGLE_LONG_PRESS_MSG        0x01
#define SINGLE_BUTTON_SINGLE_SHORT_PRESS_MSG       0x02
#define SINGLE_BUTTON_SINGLE_SHORT_LONG_PRESS_MSG  0x03
#define SINGLE_BUTTON_DOUBLE_SHORT_PRESS_MSG       0x04
#define SINGLE_BUTTON_TREBLE_SHORT_PRESS_MSG       0x05
#define SINGLE_BUTTON_UNDEFINED_MSG                0x00
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * @Structure singleButton_timing_manager_t
 *
 * @brief     It handles the timer from the application
 *
 * @data      timerStart      //A function that can start the timer
 *            timerStop       //A function that can stop the timer
 *            timerSetPeriod  //A function set the period of the timer
 */
typedef void (*singleButton_timerStart)(void);
typedef void (*singleButton_timerSetPeriod)(uint32_t timerPeriod);
typedef void (*singleButton_timerStop)(void);
typedef struct
{
    singleButton_timerStart timerStart;
    singleButton_timerSetPeriod timerSetPeriod;
    singleButton_timerStop  timerStop;
}singleButton_timerManager_t;
/*********************************************************************
 * @Structure singleButtonCBs_t
 *
 * @brief     It stores the application callback function(s) memory address(es)
 *
 * @data
 */
typedef void (*singleButtonCB_t)(uint8_t messageID);
typedef struct
{
    singleButtonCB_t singleButtonCB_t;
}singleButtonCBs_t;
/*********************************************************************
 * FUNCTIONS
 */
extern void singleButton_init();
extern void singleButton_registerTimer(singleButton_timerManager_t *singleButtonTimer);//Register in peripherals already
extern void singleButton_registerCBs(singleButtonCBs_t *singleButtonCBs);//Register in main function.
extern void singleButton_processButtonEvt(uint8_t logicLevel);
extern void singleButton_processTimerOv();

#ifdef __cplusplus
}
#endif

#endif /* SINGLEBUTTON_H_ */
