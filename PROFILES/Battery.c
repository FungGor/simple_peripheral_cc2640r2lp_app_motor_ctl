/**********************************************************************************************
 * Filename:       Battery.c
 *
 * Description:    This file contains the implementation of the service.
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


/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "Battery.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
* GLOBAL VARIABLES
*/

// Battery Service UUID
static CONST uint8 BatteryUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATTERY_SERV_UUID), HI_UINT16(BATTERY_SERV_UUID)
};

// Battery_Level UUID
static CONST uint8 Battery_Battery_LevelUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATTERY_BATTERY_LEVEL_UUID), HI_UINT16(BATTERY_BATTERY_LEVEL_UUID)
};
// BATTERY_VOLTAGE UUID
static CONST uint8 Battery_Battery_VoltageUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATTERY_BATTERY_VOLTAGE_UUID), HI_UINT16(BATTERY_BATTERY_VOLTAGE_UUID)
};
// BATTERY_TEMPERATURE UUID
static CONST uint8 Battery_Battery_TemperatureUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATTERY_BATTERY_TEMPERATURE_UUID), HI_UINT16(BATTERY_BATTERY_TEMPERATURE_UUID)
};
// Battery_Error Code UUID
static CONST uint8 Battery_Battery_Error_CodeUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(BATTERY_BATTERY_ERROR_CODE_UUID)
};
// Battery_Statuse UUID
static CONST uint8 Battery_Battery_StatusUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(BATTERY_BATTERY_STATUS_UUID), HI_UINT16(BATTERY_BATTERY_STATUS_UUID)
};
/*********************************************************************
 * LOCAL VARIABLES
 */

static BatteryCBs_t *pAppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Service declaration
static CONST gattAttrType_t BatteryDecl = { ATT_BT_UUID_SIZE, BatteryUUID };

// Characteristic "Battery_Level" Properties (for declaration)
static uint8 Battery_Battery_LevelProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Battery_Level" Value variable
static uint8 Battery_Battery_LevelVal[BATTERY_BATTERY_LEVEL_LEN] = {100};  //{0};
// Characteristic "Battery_Level" CCCD
static gattCharCfg_t *Battery_Battery_LevelConfig;

// Characteristic "BATTERY_VOLTAGE" Properties (for declaration)
static uint8 Battery_Battery_VoltageProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "BATTERY_VOLTAGE" Value variable
static uint8 Battery_Battery_VoltageVal[BATTERY_BATTERY_VOLTAGE_LEN] = {255};  // voltage is in mV
// Characteristic "BATTERY_VOLTAGE" CCCD
static gattCharCfg_t *Battery_Battery_VoltageConfig;

// **** The current battery version does not feedback temperature information - For Future Use Only   *****
// Characteristic "BATTERY_TEMPERATURE" Properties (for declaration)
static uint8 Battery_Battery_TemperatureProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "BATTERY_TEMPERATURE" Value variable
static uint8 Battery_Battery_TemperatureVal[BATTERY_BATTERY_TEMPERATURE_LEN] = {15};
// Characteristic "BATTERY_TEMPERATURE" CCCD
static gattCharCfg_t *Battery_Battery_TemperatureConfig;

// **** The current battery version does not support error code information - For Future Use Only   *****
// Characteristic "BATTERY_ERROR_CODE" Properties (for declaration)
static uint8 Battery_Battery_Error_CodeProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "BATTERY_ERROR_CODE" Value variable
static uint8 Battery_Battery_Error_CodeVal[BATTERY_BATTERY_ERROR_CODE_LEN] = {0};
// Characteristic "BATTERY_ERROR_CODE" CCCD
static gattCharCfg_t *Battery_Battery_Error_CodeConfig;

// Characteristic "BATTERY_STATUS" Properties (for declaration)
static uint8 Battery_Battery_StatusProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "BATTERY_STATUS" Value variable
static uint8 Battery_Battery_StatusVal[BATTERY_BATTERY_STATUS_LEN] = {5};   // = GLOWING_AQUA in dataAnalysis.h
// Characteristic "BATTERY_STATUS" CCCD
static gattCharCfg_t *Battery_Battery_StatusConfig;
/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t BatteryAttrTbl[] =
{
  // Battery Service Declaration
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID },
      GATT_PERMIT_READ,
      0,
      (uint8 *)&BatteryDecl
  },

  // Battery_Level Characteristic Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Battery_Battery_LevelProps
  },
      // Battery_Level Characteristic Value
      {
        { ATT_BT_UUID_SIZE, Battery_Battery_LevelUUID },
          GATT_PERMIT_READ,
          0,
          Battery_Battery_LevelVal
      },
      // Battery_Level CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
          GATT_PERMIT_READ | GATT_PERMIT_WRITE,
          0,
          (uint8 *)&Battery_Battery_LevelConfig
      },
      // BATTERY_Level user descriptor
      {
         {ATT_BT_UUID_SIZE, charUserDescUUID},
          GATT_PERMIT_READ,
          0,
          "Battery Level (%)"
      },

  // BATTERY_VOLTAGE Characteristic Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Battery_Battery_VoltageProps
  },
      // BATTERY_VOLTAGE Characteristic Value
      {
        { ATT_BT_UUID_SIZE, Battery_Battery_VoltageUUID },
          GATT_PERMIT_READ,
          0,
          Battery_Battery_VoltageVal
      },
      // BATTERY_VOLTAGE CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
          GATT_PERMIT_READ | GATT_PERMIT_WRITE,
          0,
          (uint8 *)&Battery_Battery_VoltageConfig
      },
      // BATTERY_VOLTAGE user descriptor
      {
         {ATT_BT_UUID_SIZE, charUserDescUUID},
          GATT_PERMIT_READ,
          0,
          "Battery Voltage (mV)"     // average battery voltage in mV
      },

  // BATTERY_TEMPERATURE Characteristic Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Battery_Battery_TemperatureProps
  },
          // BATTERY_TEMPERATURE Characteristic Value
      {
        { ATT_BT_UUID_SIZE, Battery_Battery_TemperatureUUID },
          GATT_PERMIT_READ,
          0,
          Battery_Battery_TemperatureVal
      },
      // BATTERY_TEMPERATURE CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
          GATT_PERMIT_READ | GATT_PERMIT_WRITE,
          0,
          (uint8 *)&Battery_Battery_TemperatureConfig
      },
      // BATTERY_TEMPERATURE user descriptor
      {
         {ATT_BT_UUID_SIZE, charUserDescUUID},
          GATT_PERMIT_READ,
          0,
          "Battery Temperature (C)"
      },

  // BATTERY_STATUS Characteristic Declaration
  {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Battery_Battery_StatusProps
  },
           // BATTERY_STATUS Characteristic Value
          {
            { ATT_BT_UUID_SIZE, Battery_Battery_StatusUUID },
              GATT_PERMIT_READ,
              0,
              Battery_Battery_StatusVal
          },
          // BATTERY_STATUS CCCD
          {
             { ATT_BT_UUID_SIZE, clientCharCfgUUID },
               GATT_PERMIT_READ | GATT_PERMIT_WRITE,
               0,
               (uint8 *)&Battery_Battery_StatusConfig
          },
          // BATTERY_STATUS user descriptor
          {
             {ATT_BT_UUID_SIZE, charUserDescUUID},
             GATT_PERMIT_READ,
             0,
             "Battery Status"
          },
  // BATTERY_ERROR_CODE Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &Battery_Battery_Error_CodeProps
    },
            // BATTERY_ERROR_CODE Characteristic Value
        {
          { ATT_UUID_SIZE, Battery_Battery_Error_CodeUUID },
            GATT_PERMIT_READ,
            0,
            Battery_Battery_Error_CodeVal
        },
        // BATTERY_ERROR_CODE CCCD
        {
          { ATT_BT_UUID_SIZE, clientCharCfgUUID },
            GATT_PERMIT_READ | GATT_PERMIT_WRITE,
            0,
            (uint8 *)&Battery_Battery_Error_CodeConfig
        },
        // BATTERY_ERROR_CODE user descriptor
        {
           {ATT_BT_UUID_SIZE, charUserDescUUID},
            GATT_PERMIT_READ,
            0,
            "Battery Error Code"
        }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t Battery_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 *pLen, uint16 offset,
                                           uint16 maxLen, uint8 method );
static bStatus_t Battery_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                            uint8 *pValue, uint16 len, uint16 offset,
                                            uint8 method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t BatteryCBs =
{
  Battery_ReadAttrCB,  // Read callback function pointer
  Battery_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*
 * Battery_AddService- Initializes the Battery service by registering
 *          GATT attributes with the GATT server.
 *
 */
bStatus_t Battery_AddService( void )
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table for Battery Level
  Battery_Battery_LevelConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Battery_Battery_LevelConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Battery_Battery_LevelConfig );

  // Allocate Client Characteristic Configuration table for Battery Voltage
  Battery_Battery_VoltageConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Battery_Battery_VoltageConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Battery_Battery_VoltageConfig );

  // Allocate Client Characteristic Configuration table for Battery Temperature
  Battery_Battery_TemperatureConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Battery_Battery_TemperatureConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Battery_Battery_TemperatureConfig );

  // Allocate Client Characteristic Configuration table for Battery Error
  Battery_Battery_Error_CodeConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Battery_Battery_Error_CodeConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Battery_Battery_Error_CodeConfig );

  // Allocate Client Characteristic Configuration table for Battery Status
  Battery_Battery_StatusConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Battery_Battery_StatusConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Battery_Battery_StatusConfig );

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( BatteryAttrTbl,
                                        GATT_NUM_ATTRS( BatteryAttrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &BatteryCBs );

  return ( status );
}

/*
 * Battery_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t Battery_RegisterAppCBs( BatteryCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    pAppCBs = appCallbacks;

    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}

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
bStatus_t Battery_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case BATTERY_BATTERY_LEVEL:
      if ( len == BATTERY_BATTERY_LEVEL_LEN )
      {
        memcpy(Battery_Battery_LevelVal, value, len);  // here, value overwrites Battery_Battery_LevelVal. Battery_Battery_LevelVal is a variable, but why is "value" a pointer address
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Battery_Battery_LevelConfig, (uint8_t *)&Battery_Battery_LevelVal, FALSE,
                                    BatteryAttrTbl, GATT_NUM_ATTRS( BatteryAttrTbl ),
                                    INVALID_TASK_ID,  Battery_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    case BATTERY_BATTERY_VOLTAGE:
      if ( len == BATTERY_BATTERY_VOLTAGE_LEN )
      {
        memcpy(Battery_Battery_VoltageVal, value, len); // here, value overwrites Battery_Battery_VoltageVal. Battery_Battery_VoltageVal is a variable, but why is "value" a pointer address
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Battery_Battery_VoltageConfig, (uint8_t *)&Battery_Battery_VoltageVal, FALSE,
                                    BatteryAttrTbl, GATT_NUM_ATTRS( BatteryAttrTbl ),
                                    INVALID_TASK_ID,  Battery_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    case BATTERY_BATTERY_TEMPERATURE:
      if ( len == BATTERY_BATTERY_TEMPERATURE_LEN )
      {
        memcpy(Battery_Battery_TemperatureVal, value, len); // here, value overwrites Battery_Battery_TemperatureVal. Battery_Battery_TemperatureVal is a variable, but why is "value" a pointer address
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Battery_Battery_TemperatureConfig, (uint8_t *)&Battery_Battery_TemperatureVal, FALSE,
                                    BatteryAttrTbl, GATT_NUM_ATTRS( BatteryAttrTbl ),
                                    INVALID_TASK_ID,  Battery_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    case BATTERY_BATTERY_ERROR_CODE:
      if ( len == BATTERY_BATTERY_ERROR_CODE_LEN )
      {
        memcpy(Battery_Battery_Error_CodeVal, value, len); // here, value overwrites Battery_Battery_Error_CodeVal. Battery_Battery_Error_CodeVal is a variable, but why is "value" a pointer address
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Battery_Battery_Error_CodeConfig, (uint8_t *)&Battery_Battery_Error_CodeVal, FALSE,
                                    BatteryAttrTbl, GATT_NUM_ATTRS( BatteryAttrTbl ),
                                    INVALID_TASK_ID,  Battery_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    case BATTERY_BATTERY_STATUS:
      if ( len == BATTERY_BATTERY_STATUS_LEN )
      {
        memcpy(Battery_Battery_StatusVal, value, len); // here, value overwrites Battery_Battery_StatusVal. Battery_Battery_StatusVal is a variable, but why is "value" a pointer address
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Battery_Battery_StatusConfig, (uint8_t *)&Battery_Battery_StatusVal, FALSE,
                                    BatteryAttrTbl, GATT_NUM_ATTRS( BatteryAttrTbl ),
                                    INVALID_TASK_ID,  Battery_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    default:
      ret = INVALIDPARAMETER;
      break;

  }
  return ret;
}
/*
 * Battery_GetParameter - Get a Battery parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t Battery_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}
/*********************************************************************
 * @fn          Battery_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t Battery_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint16 *pLen, uint16 offset,
                                       uint16 maxLen, uint8 method )
{
    bStatus_t status = SUCCESS;
    // See if request is regarding the Battery_Level Characteristic Value
    if (! memcmp(pAttr->type.uuid, Battery_Battery_LevelUUID, pAttr->type.len) )
    {
      if ( offset > BATTERY_BATTERY_LEVEL_LEN )  // Prevent malicious ATT ReadBlob offsets.
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        *pLen = MIN(maxLen, BATTERY_BATTERY_LEVEL_LEN - offset);  // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
      }
    }
    // See if request is regarding the Battery_Voltage Characteristic Value
    else if (! memcmp(pAttr->type.uuid, Battery_Battery_VoltageUUID, pAttr->type.len) )
    {
      if ( offset > BATTERY_BATTERY_VOLTAGE_LEN )  // Prevent malicious ATT ReadBlob offsets.
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        *pLen = MIN(maxLen, BATTERY_BATTERY_VOLTAGE_LEN - offset);  // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
      }
    }
    // See if request is regarding the Battery_Temperature Characteristic Value
    else if (! memcmp(pAttr->type.uuid, Battery_Battery_TemperatureUUID, pAttr->type.len) )
    {
      if ( offset > BATTERY_BATTERY_TEMPERATURE_LEN )  // Prevent malicious ATT ReadBlob offsets.
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        *pLen = MIN(maxLen, BATTERY_BATTERY_TEMPERATURE_LEN - offset);  // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
      }
    }
    // See if request is regarding the Battery_Error_Code Characteristic Value
    else if (! memcmp(pAttr->type.uuid, Battery_Battery_Error_CodeUUID, pAttr->type.len) )
    {
      if ( offset > BATTERY_BATTERY_ERROR_CODE_LEN )  // Prevent malicious ATT ReadBlob offsets.
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        *pLen = MIN(maxLen, BATTERY_BATTERY_ERROR_CODE_LEN - offset);  // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
      }
    }
    // See if request is regarding the Battery_Status Characteristic Value
    else if (! memcmp(pAttr->type.uuid, Battery_Battery_StatusUUID, pAttr->type.len) )
    {
      if ( offset > BATTERY_BATTERY_STATUS_LEN )  // Prevent malicious ATT ReadBlob offsets.
      {
        status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
        *pLen = MIN(maxLen, BATTERY_BATTERY_STATUS_LEN - offset);  // Transmit as much as possible
        memcpy(pValue, pAttr->pValue + offset, *pLen);
      }
    }
    else
    {
      // If we get here, that means you've forgotten to add an if clause for a
      // characteristic value attribute in the attribute table that has READ permissions.
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
    }

    return status;
}


/*********************************************************************
 * @fn      Battery_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t Battery_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                        uint8 *pValue, uint16 len, uint16 offset,
                                        uint8 method )
{
  bStatus_t status  = SUCCESS;
  uint8_t   paramID = 0xFF;
  // See if request is regarding a Client Characteristic Configuration
  if ( ! memcmp(pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len) )
  {
    // Allow only notifications.
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                             offset, GATT_CLIENT_CFG_NOTIFY);
  }
  else
  {
    // If we get here, that means you've forgotten to add an if clause for a
    // characteristic value attribute in the attribute table that has WRITE permissions.
    status = ATT_ERR_ATTR_NOT_FOUND;
  }
  // Let the application know something changed (if it did) by using the
  // callback it registered earlier (if it did).
  if (paramID != 0xFF)
  {
    if ( pAppCBs && pAppCBs->pfnChangeCb )
    {
      pAppCBs->pfnChangeCb( paramID ); // Call app function from stack task context.
    }
  }
  return status;
}
