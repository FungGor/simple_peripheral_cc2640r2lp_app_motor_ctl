/******************************************************************************
 *
 * @file    powerOnTime.c
 *
 * @brief   This file contains the Code of counting the power on time of the device
 *
 *  Created on: 30 Jan 2023
 *      Author: Chee
 *
 *******************************************************************************/
/*********************************************************************
 * INCLUDES
 */
#include "powerOnTime.h"
//#include "lightControl.h"
#include "Dashboard.h"
#include "motorControl.h"

/*********************************************************************
 * LOCAL POINTERS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8_t     powerOnTimeMinute_t = 0;                    // power on time in minutes
//uint16_t powerOnTimeMinute = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      getPowerOnTime
 *
 * @brief   call this function to retrieve the Power On Time
 *
 * @param   None
 *
 * @return  powerOnTimeMinute
 *********************************************************************/
uint16_t powerOnTime_getPowerOnTime(){
    return powerOnTimeMinute_t;
}

/*********************************************************************
 * @fn      powerOnTime_init
 *
 * @brief   Initialization and initial set up at each Power On.
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void powerOnTime_init()
{

}

/*********************************************************************
 * @fn      powerOnTime_cal
 *
 * @brief   Calculate power on time.
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
//uint8_t     powerOnTimeMinute_t = 0;                    // power on time in minutes

void powerOnTime_cal( uint32_t powerOnTime_ms )
{
    // ********************   Power On Time (in minutes)   *************************************
    // *****************************************************************************************
    //powerOnTimeMS += utimeInterval;

    if (( powerOnTime_ms / POWERONTIME_MINUTE_TIME ) > powerOnTimeMinute_t )
    {
        powerOnTimeMinute_t++;
        motorcontrol_setGatt(DASHBOARD_SERV_UUID, DASHBOARD_POWER_ON_TIME, DASHBOARD_POWER_ON_TIME_LEN, (uint8_t *) &powerOnTimeMinute_t);
    }
}

