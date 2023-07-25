/*
 * generalPurposeTimer.c
 *
 *  Created on: 12 Jul 2023
 *      Author: User
 */
/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <Board.h>
#include "periodicCommunication.h"
#include "UDHAL/UDHAL.h"
#include "lightControl.h"
#include "powerOnTime.h"
#include "ledControl.h"
#include "motorControl.h"
#include "Dashboard.h"
#include "generalPurposeTimer.h"
#include "UDHAL/UDHAL_I2C.h"
#include "IS31FL3236A/IS31FL3236A.h"
#include "TSL2561/TSL2561.h"

/*********************************************************************
 * @fn      lightControl_initialization
 *
 * @brief   Initialization and initial set up at each Power On.  AUTO mode at every POWER ON
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
uint8_t gpTimer_i2cOpenStatus = 0;
uint8_t gpcheck = 0;

void gpTimer_init()
{

}

/*********************************************************************
* @fn      GPtimer_createTask
*
* @brief   Task creation function for the General Purpose Task Timer.
*
* @param   None.
*
* @return  None.
*********************************************************************/
// Task configuration
Task_Struct     gptTask;
Char            gptTaskStack[GPT_TASK_STACK_SIZE];

void GPtimer_createTask(void)
{
    Task_Params taskParams;

    // Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = gptTaskStack;
    taskParams.stackSize = GPT_TASK_STACK_SIZE;
    taskParams.priority = GPT_TASK_PRIORITY;

    Task_construct(&gptTask, GPtimer_taskFxn, &taskParams, NULL);     //

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
uint16_t    utime_Interval = PERIODIC_COMMUNICATION_HF_SAMPLING_TIME;
uint16_t    GPT_taskCounter = 0;                               // ALS sampling counter
uint32_t    powerOnTimeMS = 0;                          // power on time in milli-seconds

static void GPtimer_taskFxn(UArg a0, UArg a1)
{
    gpTimer_init();
    for (; ;)               // infinite for loop, starting at 1 and without exit condition,
    {
        if (( GPT_taskCounter % NUM_PERIODIC_COMMUNICATION_HF_2_ALS ) == 0)
        {
            lightControl_taskFxn();
        }

        powerOnTimeMS += utime_Interval;
        powerOnTime_cal(powerOnTimeMS);

        // Task delay
        Task_sleep(utime_Interval * 1000 / Clock_tickPeriod);
        GPT_taskCounter++;

    }

}
