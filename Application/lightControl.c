/******************************************************************************

 @file  lightControl.c

 @brief This file contains the Code of light control interfacing

 Group: WCS, BTS
 Target Device: cc2640r2

 ******************************************************************************
 ******************************************************************************
 *****************************************************************************/
/*********************************************************************
 * INCLUDES
 */
#include <Board.h>
#include <stdint.h>

#include "lightControl.h"
#include "powerOnTime.h"
#include "ledControl.h"
#include "motorControl.h"
#include "Dashboard.h"
#include "UDHAL/UDHAL_I2C.h"
#include "TSL2561/TSL2561.h"

/*********************************************************************
 * LOCAL POINTERS
 */
static void (*lightModeArray[3])(void) = {light_MODE_OFF, light_MODE_ON, light_MODE_AUTO};

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint8_t                      bitluxIndex;
static uint8_t                      light_mode;                             // declare as static when not debugging
static uint8_t                      light_mode_Index;
static uint8_t                      light_status = LIGHT_STATUS_INITIAL;    // declare as static when not debugging
static uint8_t                      lightStatusNew = LIGHT_STATUS_INITIAL;
static uint8_t                      lightcontrol_I2CStatus = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
/*********************************************************************
 * @fn      getLightMode
 *
 * @brief   call this function to retrieve the current light mode
 *
 * @param   None
 *
 * @return  light_mode
 *********************************************************************/
uint8_t lightControl_getLightMode(void){
    return light_mode;
}
/*********************************************************************
 * @fn      getLightStatus
 *
 * @brief   call this function to retrieve the current light status
 *
 * @param   None
 *
 * @return  light_status
 *********************************************************************/
uint8_t lightControl_getLightStatus(void){
    return light_status;
}
/*********************************************************************
 * @fn      light_MODE_AUTO
 *
 * @brief   Execute AUTO light mode routine
 *
 * @param   light_status
 *
 * @return  light_status
 *********************************************************************/

void light_MODE_AUTO()
{
    if (bitluxIndex == 0)
    {
        lightStatusNew = LIGHT_STATUS_OFF;
    }
    else if (bitluxIndex == ALS_NUMSAMPLES)
    {
        lightStatusNew = LIGHT_STATUS_ON;
    }

    if (light_status != lightStatusNew)
    {
        light_status = lightStatusNew;
        lightControl_motorControl_lightStatusChg();
    }

}

/*********************************************************************
* @fn      lightControl_taskFxn
*
* @brief   Task creation function for the light Control.
*
* @param   None.
*
* @return  None.
**********************************************************************/
extern void lightControl_taskFxn()
{
    // ********************  light sensor control (in minutes)  *******************************
            // *****************************************************************************************
            if (light_mode == 2)
            {
            // ************   Calls I2C to measure ADC and calculates calibrated lux  ****************
                lightControl_measureLux();
            // ************   Convert ALS_Data to luxIndex and compute sumluxIndex  ******************
                lightControl_ALS_Controller();
            // ****************************   Light Control                      *********************
                light_MODE_AUTO();
            }
}

/*********************************************************************
 * @fn      lightControl_initialization
 *
 * @brief   Initialization and initial set up at each Power On.  AUTO mode at every POWER ON
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/

void lightControl_init( uint8_t i2cOpenStatus )
{
    //powerOnTime_trigger_flag = POWERONTIME_MINUTE_TIME / ALS_SAMPLING_TIME;

    //** I2C must be initiated before lightControl
    //** At every POWER ON (SYSTEM START-UP), "IF I2C communication is SUCCESSFUL", the light control is in auto mode (LIGHT_MODE_INITIAL), light is off (LIGHT_STATUS_INITIAL).
    //** "IF I2C communication is NOT successful, we have to disable auto mode.
    lightcontrol_I2CStatus =  i2cOpenStatus; //UDHAL_I2C_getOpenStatus();

    if (lightcontrol_I2CStatus == 1) {
        light_mode = LIGHT_MODE_AUTO;       // if i2c started successfully, light mode default is AUTO
        light_mode_Index = 2;               // if i2c started successfully, AUTO light mode is available
    }
    else if (lightcontrol_I2CStatus == 0) {
        light_mode = LIGHT_MODE_OFF;        // if i2c did not start successfully, light mode default is OFF
        light_mode_Index = 1;               // if i2c did not start successfully, AUTO light mode will not be available - only ON or OFF light mode is available
    }

    light_status = LIGHT_STATUS_INITIAL;
    lightStatusNew = LIGHT_STATUS_INITIAL;

}

/*********************************************************************************
* The TSL2561 has two channels: (1) Channel 1 sensor measures visible and IR
* lights and (2) channel 2 sensor is more sensitive to IR lights.
**********************************************************************************
* call light sensor -> measure light level (Lux) data
* Read Channel 0 via Read Word Protocol
* ********************************************************************************
* call light sensor -> measure light level (Lux) data
* Read Channel 1 via Read Word Protocol
* ********************************************************************************/
uint32_t luxValue = 0xFFFFFFFF;
void lightControl_measureLux()
{

    TSL2561_readChannel(READCHANNEL0);

    TSL2561_readChannel(READCHANNEL1);

    luxValue = TSL2561_lux();
}

/*********************************************************************
 * @fn      light_MODE_OFF
 *
 * @brief   Execute OFF light mode routine
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void light_MODE_OFF(){
    if (light_status != LIGHT_STATUS_OFF) {
        light_status = LIGHT_STATUS_OFF;
        lightControl_motorControl_lightStatusChg();
    }
}
/*********************************************************************
 * @fn      light_MODE_ON
 *
 * @brief   Execute ON light mode routine
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void light_MODE_ON(){
    if (light_status != LIGHT_STATUS_ON) {
        light_status = LIGHT_STATUS_ON;
        lightControl_motorControl_lightStatusChg();
    }
}
/*********************************************************************
 * EXTERN FUNCTIONS
 */
/*********************************************************************
 * @fn      lightControl_motorControl_lightStatusChg
 *
 * @brief   Notify light status change
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void lightControl_motorControl_lightStatusChg(void){
    // switch LED display brightness depending on light status
    uint8_t Board_GPIO_LED_state;   // for Launchpad only
    uint8_t ledPower;
    switch(light_status)
    {
    case LIGHT_STATUS_ON:
            {
                Board_GPIO_LED_state = Board_GPIO_LED_ON;   // for Launchpad only
                ledPower = LED_POWER_LIGHT_ON;
                break;
            }
    case LIGHT_STATUS_OFF:
            {
                Board_GPIO_LED_state = Board_GPIO_LED_OFF;  // for Launchpad only
                ledPower = LED_POWER_LIGHT_OFF;
                break;
            }
    default:
        break;
    }
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_state);  // for Launchpad only
    motorcontrol_setGatt(DASHBOARD_SERV_UUID, DASHBOARD_LIGHT_STATUS, DASHBOARD_LIGHT_STATUS_LEN, (uint8_t *) &light_status);
    ledControl_setLEDPower(ledPower);
    ledControl_setLightStatus(light_status);

}

/*********************************************************************
 * @fn      lightControl_Change
 *
 * @brief   Toggles and change to the next light mode, then call to execute light mode change
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void lightControl_lightModeChange()
{
    light_mode++;

    if(light_mode > light_mode_Index)
    {
        light_mode = 0;
    }
    motorcontrol_setGatt(DASHBOARD_SERV_UUID, DASHBOARD_LIGHT_MODE, DASHBOARD_LIGHT_MODE_LEN, (uint8_t *) &light_mode);
    ledControl_setLightMode(light_mode);
    (*lightModeArray[light_mode])();

}

/*********************************************************************
 * @fn      lightControl_ALS_Controller
 *
 * @brief   Execute ON light mode routine for auto mode. Put it inside I2C interrupt
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
static uint8_t  luxIndex = 0;
static uint8_t  luxBit[ALS_NUMSAMPLES] = {0};

void lightControl_ALS_Controller( void )
{
    // ************* convert ALS_Data to luxIndex  ***************************
    uint8_t ii;
    bitluxIndex = 0;        // Resets bitluxIndex to 0

    // If the measured lux value is less than the LUX Threshold, luxBit = 1 (ON), else luxBit = 0 (OFF)
    luxBit[luxIndex] = (luxValue < LUX_THRESHOLD);
    /********  Sum 3 consecutive luxBit values (= bitluxIndex)
     * *****   if sum = 3 ---> AUTO light ON
     * *****   if sum = 0 ---> AUTO light OFF
     * *****   else ---> do nothing
     ****************************************************************/
    for (ii = 0; ii < ALS_NUMSAMPLES; ii++){
        bitluxIndex += luxBit[ii];
    }

    luxIndex++;
    // reset LuxIndex to 0 if LuxIndex is greater than ALS_NUMSAMPLES
    if (luxIndex > ALS_NUMSAMPLES){
        luxIndex = 0;
    }

}
