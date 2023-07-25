/**********************************************************************************************
 * Filename:       Battery.h
 *
 * Description:    This file contains the Battery service definitions and
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


#ifndef _BATTERY_H_
#define _BATTERY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
* CONSTANTS
*/
// Service UUID
#define BATTERY_SERV_UUID                       0x180F

//  Characteristic definition - in percentage
#define BATTERY_BATTERY_LEVEL                   0
#define BATTERY_BATTERY_LEVEL_UUID              0x2A19
#define BATTERY_BATTERY_LEVEL_LEN               1

//  Characteristic defines <- should display battery voltage in mV
#define BATTERY_BATTERY_VOLTAGE                 1
#define BATTERY_BATTERY_VOLTAGE_UUID            0x2B18
#define BATTERY_BATTERY_VOLTAGE_LEN             2

//  Characteristic defines <- currently no battery temperature sensor / measurement
#define BATTERY_BATTERY_TEMPERATURE             2
#define BATTERY_BATTERY_TEMPERATURE_UUID        0x2A6E
#define BATTERY_BATTERY_TEMPERATURE_LEN         1

//  Characteristic defines <- currently no battery error code
#define BATTERY_BATTERY_ERROR_CODE              3
#define BATTERY_BATTERY_ERROR_CODE_UUID         0x5800    // To be assigned/created UUID code
#define BATTERY_BATTERY_ERROR_CODE_LEN          1

//  Characteristic defines -> display battery status from 1 to 5
#define BATTERY_BATTERY_STATUS                  4
#define BATTERY_BATTERY_STATUS_UUID             0x2A1B
#define BATTERY_BATTERY_STATUS_LEN              1

// Battery Error Code
#define BATTERY_NORMAL                          10
#define BMS_COMMUNICATION_ERROR                 11
#define BATTERY_TEMPERATURE_ABNORMAL            12
#define BATTERY_OVER_CURRENT                    13

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
typedef void (*BatteryChange_t)( uint8 paramID );

typedef struct
{
  BatteryChange_t        pfnChangeCb;  // Called when characteristic value changes
} BatteryCBs_t;

/*********************************************************************
 * API FUNCTIONS
 */
/*
 * Battery_AddService- Initializes the Battery service by registering
 *          GATT attributes with the GATT server.
 *
 */
extern bStatus_t Battery_AddService( void );

/*
 * Battery_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Battery_RegisterAppCBs( BatteryCBs_t *appCallbacks );

/*
 * Battery_SetParameter - Set a Battery parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Battery_SetParameter( uint8 param, uint8 len, void *value );

/*
 * Battery_GetParameter - Get a Battery parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Battery_GetParameter( uint8 param, void *value );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _BATTERY_H_ */
