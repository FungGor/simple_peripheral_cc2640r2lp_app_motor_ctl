/******************************************************************************

@file  singleButton.c

@brief This file contains the Code of single button control


*****************************************************************************/

/*********************************************************************
* INCLUDES
*/

#include "singleButton.h"
#include "lightControl.h"
/*********************************************************************
* LOCAL VARIABLES
*/
static uint8_t buttonState = SINGLE_BUTTON_WAITING_STATE;
uint32_t timerPeriod;
uint8_t risingEdgeCount = 0;    // make this static if not debugging
uint8_t fallingEdgeCount = 0;   // make this static if not debugging
singleButton_timerManager_t *singleButton_timerManager; //singleButton_timerManager

static singleButtonCBs_t *singleButtonCBs;

/*********************************************************************
* LOCAL FUNCTIONS
*/
/*********************************************************************
 * @fn      singleButton_Init
 *
 * @brief   It is used to initialize the library
 *
 * @param   none
 *
 *
 * @return  none
 */
void singleButton_init()
{

}
/*********************************************************************
 * @fn      singleButton_registerClock
 *
 * @brief   It is used to process the button click time when the button is pressed
 *
 * @param   deviceTimer     A set of function pointer contain the timer function of specific device(example, STM32, TI chips)
 *
 *
 * @return  none
 */
void singleButton_registerTimer(singleButton_timerManager_t *singleButtonTimer)
{
    singleButton_timerManager = singleButtonTimer;
}
/*********************************************************************
 * @fn      singleButton_registerCBs
 *
 * @brief   It informs the application if the button is clicked
 *
 * @param   singleButton_CBs    A set of function pointer that informs the application if the button is clicked
 *
 *
 * @return  none
 */
void singleButton_registerCBs(singleButtonCBs_t *singleButtoncb)
{
    singleButtonCBs = singleButtoncb;
}

/*********************************************************************
 * @fn      singleButton_processButtonEvt
 *
 * @brief   It is used to process the data when the button is pressed
 *
 * @param   logicLevel It takes the logic level of the GPIO to decide whether it is a falling edge or rising edge
 *
 *
 * @return  none
 */
void singleButton_processButtonEvt(uint8_t logicLevel)
{
    if(logicLevel == 0)
    {
        fallingEdgeCount++;
    }
    if(fallingEdgeCount == 0)    // Ignores the rising edge after a long press
    {
        risingEdgeCount = 0;
        return;
    }
    if(logicLevel == 1)
    {
        risingEdgeCount++;
    }

    switch(buttonState)
    {
    case SINGLE_BUTTON_WAITING_STATE:
        {
            buttonState = SINGLE_BUTTON_EXECUTING_STATE;
            timerPeriod = SINGLE_BUTTON_TIMER_OV_TIME_LONG;
            singleButton_timerManager->timerSetPeriod(timerPeriod);
            singleButton_timerManager->timerStart();
            break;
        }
    case SINGLE_BUTTON_EXECUTING_STATE:
        {
            timerPeriod = SINGLE_BUTTON_TIMER_OV_TIME_SHORT;
            singleButton_timerManager->timerStop();
            singleButton_timerManager->timerSetPeriod(timerPeriod);
            singleButton_timerManager->timerStart();
            break;
        }
    default:
       break;
    }
}

/*********************************************************************
 * @fn      singleButton_processTimerOv
 *
 * @brief   Use this function on the timer overflow interrupt/callback
 *
 * @param   none
 *
 *
 * @return  none
 */
uint8_t buttonEvent = 0x00;

void singleButton_processTimerOv()
{
    // TOGGLE POWER ON/OFF
    if (risingEdgeCount == 0 && fallingEdgeCount == 1)
    {
        buttonEvent = 0x01;
    }
    // Change Light Mode
    else if (risingEdgeCount == 1 && fallingEdgeCount == 1)
    {
        buttonEvent = 0x02;                             //callback -> lightControl_change();
    }
    // TOGGLE BLE Advertising
    else if (risingEdgeCount == 1 && fallingEdgeCount == 2)
    {
        buttonEvent = 0x03;
    }
    // CHANGE SPEED MODE
    else if (risingEdgeCount == 2 && fallingEdgeCount == 2)
    {
        buttonEvent = 0x04;
    }
    // TOGGLE UNITS METRIC/IMPERIAL
    else if (risingEdgeCount == 3 && fallingEdgeCount == 3)
    {
        buttonEvent = 0x05;
    }
    // DO NOTHING
    else
    {
        buttonEvent = 0x00;
    }

    timerPeriod = SINGLE_BUTTON_TIMER_OV_TIME_LONG;     // resets to "SINGLE_BUTTON_TIMER_OV_TIME_LONG" after each overflow
    risingEdgeCount = 0;                                // reset to 0
    fallingEdgeCount = 0;                               // reset to 0
    buttonState = SINGLE_BUTTON_WAITING_STATE;          // reset to 0

    singleButtonCBs -> singleButtonCB_t(buttonEvent);

}
