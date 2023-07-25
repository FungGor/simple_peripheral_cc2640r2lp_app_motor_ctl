/**********************************************************************************************
 * Filename:       Controller.h
 *
 * Description:    This file contains the Controller service definitions and
 *                 prototypes.
 *
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************************************/

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
* CONSTANTS
*/
// Service UUID
#define CONTROLLER_SERV_UUID                        0x7800

//  Characteristic definition
#define CONTROLLER_VOLTAGE                          0
#define CONTROLLER_VOLTAGE_UUID                     0x2B18    //0x2AE1
#define CONTROLLER_VOLTAGE_LEN                      2

//  Characteristic definition
#define CONTROLLER_CURRENT                          1
#define CONTROLLER_CURRENT_UUID                     0x2AEE
#define CONTROLLER_CURRENT_LEN                      2

//  Characteristic definition
#define CONTROLLER_HEAT_SINK_TEMPERATURE            2
#define CONTROLLER_HEAT_SINK_TEMPERATURE_UUID       0x2A6E
#define CONTROLLER_HEAT_SINK_TEMPERATURE_LEN        1

//  Characteristic definition
#define CONTROLLER_ERROR_CODE                       3
#define CONTROLLER_ERROR_CODE_UUID                  0x7801
#define CONTROLLER_ERROR_CODE_LEN                   1

//  Characteristic definition
#define CONTROLLER_MOTOR_RPM                        4
#define CONTROLLER_MOTOR_RPM_UUID                   0x7802
#define CONTROLLER_MOTOR_RPM_LEN                    2

//  Characteristic definition
#define CONTROLLER_MOTOR_SPEED                      5
#define CONTROLLER_MOTOR_SPEED_UUID                 0x7803
#define CONTROLLER_MOTOR_SPEED_LEN                  2

//  Characteristic definition
#define CONTROLLER_TOTAL_DISTANCE_TRAVELLED         6
#define CONTROLLER_TOTAL_DISTANCE_TRAVELLED_UUID    0x7804
#define CONTROLLER_TOTAL_DISTANCE_TRAVELLED_LEN     4

//  Characteristic definition
#define CONTROLLER_TOTAL_ENERGY_CONSUMPTION         7
#define CONTROLLER_TOTAL_ENERGY_CONSUMPTION_UUID    0x7805
#define CONTROLLER_TOTAL_ENERGY_CONSUMPTION_LEN     4

//  Characteristic definition
#define CONTROLLER_TOTAL_ENERGY_EFFICIENCY          8
#define CONTROLLER_TOTAL_ENERGY_EFFICIENCY_UUID     0X7806
#define CONTROLLER_TOTAL_ENERGY_EFFICIENCY_LEN      2

//  Characteristic definition
#define CONTROLLER_RANGE                            9
#define CONTROLLER_RANGE_UUID                       0x7807
#define CONTROLLER_RANGE_LEN                        4

//  Characteristic definition
#define CONTROLLER_CO2SAVED                         10
#define CONTROLLER_CO2SAVED_UUID                    0x7808
#define CONTROLLER_CO2SAVED_LEN                     4

//  Characteristic definition
#define CONTROLLER_INSTANT_ECONOMY                  11
#define CONTROLLER_INSTANT_ECONOMY_UUID             0x7809
#define CONTROLLER_INSTANT_ECONOMY_LEN              2

//  Characteristic definition
#define CONTROLLER_MOTOR_TEMPERATURE                12
#define CONTROLLER_MOTOR_TEMPERATURE_UUID           0x2A1C
#define CONTROLLER_MOTOR_TEMPERATURE_LEN            1

// Controller Error Codes
#define CONTROLLER_NORMAL                           20
#define PHASE_CURRENT_ABNORMAL                      21
#define MOSFET_ABNORMAL                             22
#define OPAMP_ABNORAML                              23
#define CURRENT_ABNORMAL                            24
#define HEATSINK_TEMPERATURE_ABNORMAL               25
//  Motor Error Codes
#define MOTOR_NORMAL                                30
#define HALL_SENSOR_ABNORMAL                        31
#define MOTOR_TEMPERATURE_ABNORMAL                  32
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */
// Callback when a characteristic value has changed
typedef void (*ControllerChange_t)( uint8 paramID );

typedef struct
{
  ControllerChange_t        pfnChangeCb;  // Called when characteristic value changes
} ControllerCBs_t;

/*********************************************************************
 * API FUNCTIONS
 */
/*
 * Controller_AddService- Initializes the Battery service by registering
 *          GATT attributes with the GATT server.
 *
 */
extern bStatus_t Controller_AddService( void );

/*
 * Controller_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Controller_RegisterAppCBs( ControllerCBs_t *appCallbacks );

/*
 * Controller_SetParameter - Set a Battery parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Controller_SetParameter( uint8 param, uint8 len, void *value );

/*
 * Controller_GetParameter - Get a Battery parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Controller_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _CONTROLLER_H_ */

