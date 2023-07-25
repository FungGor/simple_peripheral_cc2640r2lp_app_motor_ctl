/******************************************************************************

@file  buzzerControl.c

@brief This file contains the Code of buzzer control


*****************************************************************************/

/*********************************************************************
* INCLUDES
*/
#include <stdlib.h>

#include "dataAnalysis.h"
#include "buzzerControl.h"
/*********************************************************************
* LOCAL VARIABLES
*/
static buzzerControl_timerManager_t *buzzerControl_timerManager;
static buzzerControlCBs_t *buzzer_CBs;
/*********************************************************************
* LOCAL FUNCTIONS
*/
/*********************************************************************
 * @fn      buzzerControl_Init
 *
 * @brief   It is used to initialize the library
 *
 * @param   none
 *
 *
 * @return  none
 */
void buzzerControl_init()
{
    //GPIO_setConfig(Board_GPIO_BUZZER, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
}
/*********************************************************************
 * @fn      buzzerControl_registerClock
 *
 * @brief   It is used to process the button click time when the button is pressed
 *
 * @param   deviceTimer
 *
 * @return  none
 */
void buzzerControl_registerTimer(buzzerControl_timerManager_t *buzzerControlTimer)
{
    buzzerControl_timerManager = buzzerControlTimer;
}
/*********************************************************************
 * @fn      buzzerControl_registerCBs
 *
 * @brief   It informs the application if ......
 *
 * @param   buzzerControlCBs    A set of function pointer that informs the application if .....
 *
 * @return  none
 */
void buzzerControl_registerCBs(buzzerControlCBs_t *buzzerControlCBs)
{
    buzzer_CBs = buzzerControlCBs;
}
/*********************************************************************
 * @fn      buzzerControl_Start
 *
 * @brief   Start buzzer
 *
 * @param   none
 *
 * @return  none
 */
void buzzerControl_Start()
{
    //if avgBatteryVoltage is less than specified value, start timer 9
    buzzerControl_timerManager -> timerStart();
}
/*********************************************************************
 * @fn      buzzerControl_Stop
 *
 * @brief   Stop buzzer
 *
 * @param   none
 *
 * @return  none
 */
void buzzerControl_Stop()
{
    //if avgBatteryVoltage is greater than specified value, stop timer 9
    buzzerControl_timerManager -> timerStop();
}
/*********************************************************************
 * @fn      buzzerControl_processTimerOV
 *
 * @brief   execute timer overflow
 *
 * @param   nona
 *
 * @return  none
 */
void buzzerControl_processTimerOV()
{
    // When timer9  overflows, this func is called to instruct buzzerControl to make a short beep
    // Insert code to instruct buzzerControl to make a short beep here
    buzzerControl_Start();  // then, repeat timer until timerStop is activated
}
