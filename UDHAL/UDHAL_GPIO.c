/*
 *  @file  UDHAL_GPIO.c
 *  @brief This file contain the functions about
 *  Created on: 26 Jan 2022
 *      Author: harrysiu
 */
/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>
#include "UDHAL/UDHAL_GPIO.h"
#include "singleButton/singleButton.h"
#include "Board.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8_t UDHAL_GPIO_read(uint8_t index);
static void UDHAL_GPIO_write(uint8_t index, uint8_t value);
static void UDHAL_GPIO_InterruptFxn(uint_least8_t index);  //GPIO interrupt callback, change it to meet your device requirement
/*********************************************************************
 * Marco
 */
/*********************************************************************
 * @fn      UDHAL_GPIO_init
 *
 * @brief   It is used to initialize the used registers in the motor control registers
 *
 * @param   None
 *
 * @return  None
 */
extern void UDHAL_GPIO_init()
{
    GPIO_init();
}
/*********************************************************************
 * @fn      UDHAL_GPIO_params_init
 *
 * @brief   It is used to initialize the used registers in the motor control registers
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_GPIO_params_init()
{
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_BOTH_EDGES);    //
    GPIO_setCallback(Board_GPIO_BUTTON0, UDHAL_GPIO_InterruptFxn);                      // GPIO Callback -> pin -> interrupt function
    GPIO_enableInt(Board_GPIO_BUTTON0);
}
/*********************************************************************
 * @fn      UDHAL_GPIO_write
 *
 * @brief   To write the logic Level of the GPIO
 *
 * @param   index: The pin number
 *          value: Either High or Low
 *
 * @return  None
 */
static uint8_t UDHAL_GPIO_read(uint8_t index)
{
     return GPIO_read(index);
}

/*********************************************************************
 * @fn      UDHAL_GPIO_write
 *
 * @brief   To write the logic Level of the GPIO
 *
 * @param   index: The pin number
 *          value: Either High or Low
 *
 * @return  None
 */
static void UDHAL_GPIO_write(uint8_t index, uint8_t value)
{
    GPIO_write(index, value);
}

/*********************************************************************
 * @fn      UDHAL_GPIO_InterruptFxn
 *
 * @brief   Whenever the button is pressed, it goes here. You may change the function name if your device has strict naming on ISR.
 *          However, two action must be performed.
 *          1. Add the function singleButton_processButtonEvt(uint8_t logicLevel) here/
 *          2. Read the GPIO logic level and pass it to this function
 *
 * @param   none
 *
 *
 * @return  none
 */
uint8_t count = 0;
uint8_t logicLevel;
static void UDHAL_GPIO_InterruptFxn(uint_least8_t index)
{
   count++;
   switch(index)
   {
   case Board_GPIO_BUTTON0:
        logicLevel = UDHAL_GPIO_read(Board_GPIO_BUTTON0);
        singleButton_processButtonEvt(logicLevel);
        break;
   default:
       break;
   }
}
