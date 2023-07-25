/**********************************************************************************************
 * Filename:       Dashboard.c
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
#include "Dashboard.h"

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

// Dashboard Service UUID
CONST uint8 DashboardUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_SERV_UUID)
};

// Dashboard_Error_Code UUID
CONST uint8 Dashboard_Error_CodeUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_ERROR_CODE_UUID)
};

// Dashboard_Speed_Mode UUID
CONST uint8 Dashboard_Speed_ModeUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_SPEED_MODE_UUID)
};

// Dashboard_Light_Status UUID
CONST uint8 Dashboard_Light_StatusUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_LIGHT_STATUS_UUID)
};

// Dashboard_Light_Mode UUID
CONST uint8 Dashboard_Light_ModeUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_LIGHT_MODE_UUID)
};

// Dashboard_Power_On_Time UUID
CONST uint8 Dashboard_Power_On_TimeUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_POWER_ON_TIME_UUID)
};

// Dashboard_ADCounter UUID
CONST uint8 Dashboard_ADCounterUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(DASHBOARD_ADCOUNTER_UUID)
};
/*********************************************************************
 * LOCAL VARIABLES
 */

static DashboardCBs_t *pAppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Service declaration
static CONST gattAttrType_t DashboardDecl = { ATT_UUID_SIZE, DashboardUUID };


// Characteristic "Dashboard_Error_Code" Properties (for declaration) - Client (App) side
static uint8 Dashboard_Error_CodeProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Dashboard_Error_Code" Value variable
static uint8 Dashboard_Error_CodeVal[DASHBOARD_ERROR_CODE_LEN] = {0};
// Characteristic "Dashboard_Error_Code" CCCD
static gattCharCfg_t *Dashboard_Error_CodeConfig;


// Characteristic "Dashboard_Speed_Mode" Properties (for declaration)
static uint8 Dashboard_Speed_ModeProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Dashboard_Speed_Mode" Value variable
static uint8 Dashboard_Speed_ModeVal[DASHBOARD_SPEED_MODE_LEN] = {1};
// Characteristic "Dashboard_Speed_Mode" CCCD
static gattCharCfg_t *Dashboard_Speed_ModeConfig;


// Characteristic "Dashboard_Light_Status" Properties (for declaration)
static uint8 Dashboard_Light_StatusProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Dashboard_Light_Status" Value variable
static uint8 Dashboard_Light_StatusVal[DASHBOARD_LIGHT_STATUS_LEN] = {0};
// Characteristic "Dashboard_Light_Status" CCCD
static gattCharCfg_t *Dashboard_Light_StatusConfig;


// Characteristic "Dashboard_Light_Mode" Properties (for declaration)
static uint8 Dashboard_Light_ModeProps = GATT_PROP_READ | GATT_PROP_NOTIFY | GATT_PROP_WRITE;
// Characteristic "Dashboard_Light_Mode" Value variable
static uint8 Dashboard_Light_ModeVal[DASHBOARD_LIGHT_MODE_LEN] = {2};
// Characteristic "Dashboard_Light_Mode" CCCD
static gattCharCfg_t *Dashboard_Light_ModeConfig;

// Characteristic "Dashboard_Power_On_Time" Properties (for declaration)
static uint8 Dashboard_Power_On_TimeProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Dashboard_Power_On_Time" Value variable
static uint8 Dashboard_Power_On_TimeVal[DASHBOARD_POWER_ON_TIME_LEN] = {0};
// Characteristic "Dashboard_Power_On_Time" CCCD
static gattCharCfg_t *Dashboard_Power_On_TimeConfig;

// Characteristic "Dashboard_ADCounter" Properties (for declaration)
static uint8 Dashboard_ADCounterProps = GATT_PROP_READ | GATT_PROP_NOTIFY;
// Characteristic "Dashboard_ADCounter" Value variable
static uint8 Dashboard_ADCounterVal[DASHBOARD_ADCOUNTER_LEN] = {0};
// Characteristic "Dashboard_ADCounter" CCCD
static gattCharCfg_t *Dashboard_ADCounterConfig;

/*********************************************************************
*
*
* Profile Attributes - Table
*
*/

static gattAttribute_t DashboardAttrTbl[] =
{
  // Dashboard Service Declaration
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID },
    GATT_PERMIT_READ,
    0,
    (uint8 *)&DashboardDecl
  },
    // ERROR CODE
    // Dashboard_Error_Code Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Dashboard_Error_CodeProps
    },
      // Dashboard_Error_Code Characteristic Value
      {
        { ATT_UUID_SIZE, Dashboard_Error_CodeUUID },
        GATT_PERMIT_READ,
        0,
        Dashboard_Error_CodeVal
      },
      // Dashboard_Error_Code CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_Error_CodeConfig
      },
      // Dashboard_Error_Code user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Dashboard Error Code"
      },
    // SPEED MODE
    // Dashboard_Speed_Mode Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Dashboard_Speed_ModeProps
    },
      // Dashboard_Speed_Mode Characteristic Value
      {
        { ATT_UUID_SIZE, Dashboard_Speed_ModeUUID },
        GATT_PERMIT_READ,
        0,
        Dashboard_Speed_ModeVal
      },
      // Dashboard_Speed_Mode CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_Speed_ModeConfig
      },
      // Dashboard_Speed_Mode user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Speed Mode"
      },
    // LIGHT STATUS
    // Dashboard_Light_Status Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Dashboard_Light_StatusProps
    },
      // Dashboard_Light_Status Characteristic Value
      {
        { ATT_UUID_SIZE,  Dashboard_Light_StatusUUID},
        GATT_PERMIT_READ,
        0,
        Dashboard_Light_StatusVal
      },
      // Dashboard_Light_Status CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_Light_StatusConfig
      },
      // Dashboard_Light_Status user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Light Status"
      },
    // LIGHT MODE
    // Dashboard_Light_Mode Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Dashboard_Light_ModeProps
    },
      // Dashboard_Light_Mode Characteristic Value
      {
        { ATT_UUID_SIZE, Dashboard_Light_ModeUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,   // Client is given the permission to write
        0,
        Dashboard_Light_ModeVal
      },
      // Dashboard_Light_Mode CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_Light_ModeConfig
      },
      // Dashboard_Light_Mode user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Light Mode"
      },
    // POWER ON TIME
    // Dashboard_Power_On_Time Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &Dashboard_Power_On_TimeProps
    },
      // Dashboard_Power_On_Time Characteristic Value
      {
        { ATT_UUID_SIZE, Dashboard_Power_On_TimeUUID },
        GATT_PERMIT_READ,
        0,
        Dashboard_Power_On_TimeVal
      },
      // Dashboard_Power_On_Time CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_Power_On_TimeConfig
      },
      // Dashboard_Power_On_Time user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Power On Time (minutes)"
      },
    // ADCOUNTER
    // Dashboard_ADCounter Characteristic Declaration
      {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &Dashboard_ADCounterProps
      },
      // Dashboard_ADCounter Characteristic Value
      {
        { ATT_UUID_SIZE, Dashboard_ADCounterUUID },
        GATT_PERMIT_READ,
        0,
        Dashboard_ADCounterVal
      },
      // Dashboard_ADCounter CCCD
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&Dashboard_ADCounterConfig
      },
      // Dashboard_ADCounter user descriptor
      {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        "Data ID"
      }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t Dashboard_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 *pLen, uint16 offset,
                                           uint16 maxLen, uint8 method );
static bStatus_t Dashboard_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                            uint8 *pValue, uint16 len, uint16 offset,
                                            uint8 method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t DashboardCBs =
{
  Dashboard_ReadAttrCB,  // Read callback function pointer
  Dashboard_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*
 * Dashboard_AddService- Initializes the Dashboard service by registering
 *          GATT attributes with the GATT server.
 *
 */
bStatus_t Dashboard_AddService( void )
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  Dashboard_Error_CodeConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_Error_CodeConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_Error_CodeConfig );

  // Allocate Client Characteristic Configuration table
  Dashboard_Speed_ModeConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_Speed_ModeConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_Speed_ModeConfig );

  // Allocate Client Characteristic Configuration table
  Dashboard_Light_StatusConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_Light_StatusConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_Light_StatusConfig );

  // Allocate Client Characteristic Configuration table
  Dashboard_Light_ModeConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_Light_ModeConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_Light_ModeConfig );

  // Allocate Client Characteristic Configuration table
  Dashboard_Power_On_TimeConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_Power_On_TimeConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_Power_On_TimeConfig );

  // Allocate Client Characteristic Configuration table
  Dashboard_ADCounterConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
  if ( Dashboard_ADCounterConfig == NULL )
  {
    return ( bleMemAllocError );
  }
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg( INVALID_CONNHANDLE, Dashboard_ADCounterConfig );

  // Register GATT attribute list and CBs with GATT Server App
  status = GATTServApp_RegisterService( DashboardAttrTbl,
                                        GATT_NUM_ATTRS( DashboardAttrTbl ),
                                        GATT_MAX_ENCRYPT_KEY_SIZE,
                                        &DashboardCBs );

  return ( status );
}

/*
 * Dashboard_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
bStatus_t Dashboard_RegisterAppCBs( DashboardCBs_t *appCallbacks )
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
 * Dashboard_SetParameter - Set a Dashboard parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t Dashboard_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case DASHBOARD_ERROR_CODE:
        if ( len == DASHBOARD_ERROR_CODE_LEN )
        {
        memcpy(Dashboard_Error_CodeVal, value, len);
        // Try to send notification.
        GATTServApp_ProcessCharCfg( Dashboard_Error_CodeConfig, (uint8_t *)&Dashboard_Error_CodeVal, FALSE,
                                    DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                    INVALID_TASK_ID,  Dashboard_ReadAttrCB);
        }
        else
        {
        ret = bleInvalidRange;
        }
        break;
    case DASHBOARD_SPEED_MODE:
        if ( len == DASHBOARD_SPEED_MODE_LEN )
        {
          memcpy(Dashboard_Speed_ModeVal, value, len);
          // Try to send notification.
          GATTServApp_ProcessCharCfg( Dashboard_Speed_ModeConfig, (uint8_t *)&Dashboard_Speed_ModeVal, FALSE,
                                      DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                      INVALID_TASK_ID,  Dashboard_ReadAttrCB);
        }
        else
        {
          ret = bleInvalidRange;
        }
        break;
    case DASHBOARD_LIGHT_STATUS:
        if ( len == DASHBOARD_LIGHT_STATUS_LEN )
        {
          memcpy(Dashboard_Light_StatusVal, value, len);
          // Try to send notification.
          GATTServApp_ProcessCharCfg( Dashboard_Light_StatusConfig, (uint8_t *)&Dashboard_Light_StatusVal, FALSE,
                                      DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                      INVALID_TASK_ID,  Dashboard_ReadAttrCB);
        }
        else
        {
          ret = bleInvalidRange;
        }
        break;
    case DASHBOARD_LIGHT_MODE:
        if ( len == DASHBOARD_LIGHT_MODE_LEN )
        {
          memcpy(Dashboard_Light_ModeVal, value, len);
          // Try to send notification.
          GATTServApp_ProcessCharCfg( Dashboard_Light_ModeConfig, (uint8_t *)&Dashboard_Light_ModeVal, FALSE,
                                      DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                      INVALID_TASK_ID,  Dashboard_ReadAttrCB);
        }
        else
        {
          ret = bleInvalidRange;
        }
        break;
    case DASHBOARD_POWER_ON_TIME:
            if ( len == DASHBOARD_POWER_ON_TIME_LEN )
            {
              memcpy(Dashboard_Power_On_TimeVal, value, len);
              // Try to send notification.
              GATTServApp_ProcessCharCfg( Dashboard_Power_On_TimeConfig, (uint8_t *)&Dashboard_Power_On_TimeVal, FALSE,
                                          DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                          INVALID_TASK_ID,  Dashboard_ReadAttrCB);
            }
            else
            {
              ret = bleInvalidRange;
            }
            break;
    case DASHBOARD_ADCOUNTER:
            if ( len == DASHBOARD_ADCOUNTER_LEN )
            {
              memcpy(Dashboard_ADCounterVal, value, len);
              // Try to send notification.
              GATTServApp_ProcessCharCfg( Dashboard_ADCounterConfig, (uint8_t *)&Dashboard_ADCounterVal, FALSE,
                                          DashboardAttrTbl, GATT_NUM_ATTRS( DashboardAttrTbl ),
                                          INVALID_TASK_ID,  Dashboard_ReadAttrCB);
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
 * Dashboard_GetParameter - Get a Dashboard parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
bStatus_t Dashboard_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
  case DASHBOARD_LIGHT_MODE:
          memcpy(value, Dashboard_Light_ModeVal, DASHBOARD_LIGHT_MODE_LEN);
        break;
  case DASHBOARD_LIGHT_STATUS:
          memcpy(value, Dashboard_Light_StatusVal, DASHBOARD_LIGHT_STATUS_LEN);
        break;
  case DASHBOARD_SPEED_MODE:
          memcpy(value, Dashboard_Speed_ModeVal, DASHBOARD_SPEED_MODE_LEN);
        break;
  case DASHBOARD_POWER_ON_TIME:
          memcpy(value, Dashboard_Power_On_TimeVal, DASHBOARD_POWER_ON_TIME_LEN);
        break;
  case DASHBOARD_ADCOUNTER:
          memcpy(value, Dashboard_ADCounterVal, DASHBOARD_ADCOUNTER_LEN);
        break;
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  return ret;
}
/*********************************************************************
 * @fn          Dashboard_ReadAttrCB
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
static bStatus_t Dashboard_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint16 *pLen, uint16 offset,
                                       uint16 maxLen, uint8 method )
{
  bStatus_t status = SUCCESS;
  // See if request is regarding the Dashboard_Error_Code Characteristic Value
  if (! memcmp(pAttr->type.uuid, Dashboard_Error_CodeUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_ERROR_CODE_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_ERROR_CODE_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Dashboard_Speed_Mode Characteristic Value
  else if (! memcmp(pAttr->type.uuid, Dashboard_Speed_ModeUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_SPEED_MODE_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_SPEED_MODE_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Dashboard_Light_Status Characteristic Value
  else if (! memcmp(pAttr->type.uuid, Dashboard_Light_StatusUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_LIGHT_STATUS_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_LIGHT_STATUS_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Dashboard_Light_Mode Characteristic Value
  else if (! memcmp(pAttr->type.uuid, Dashboard_Light_ModeUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_LIGHT_MODE_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_LIGHT_MODE_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Dashboard_Power_On_Time Characteristic Value
  else if (! memcmp(pAttr->type.uuid, Dashboard_Power_On_TimeUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_POWER_ON_TIME_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_POWER_ON_TIME_LEN - offset);  // Transmit as much as possible
      memcpy(pValue, pAttr->pValue + offset, *pLen);
    }
  }
  // See if request is regarding the Dashboard_ADCounter Characteristic Value
  else if (! memcmp(pAttr->type.uuid, Dashboard_ADCounterUUID, pAttr->type.len) )
  {
    if ( offset > DASHBOARD_ADCOUNTER_LEN )  // Prevent malicious ATT ReadBlob offsets.
    {
      status = ATT_ERR_INVALID_OFFSET;
    }
    else
    {
      *pLen = MIN(maxLen, DASHBOARD_ADCOUNTER_LEN - offset);  // Transmit as much as possible
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
 * @fn      Dashbpard_WriteAttrCB
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
static bStatus_t Dashboard_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                        uint8 *pValue, uint16 len, uint16 offset,
                                        uint8 method )
{
  bStatus_t status  = SUCCESS;
  uint8_t   paramID = 0xFF;

  // See if request is regarding a Client Characterisic Configuration
  if ( ! memcmp(pAttr->type.uuid, clientCharCfgUUID, pAttr->type.len) )
  {
    // Allow only notifications.
    status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                             offset, GATT_CLIENT_CFG_NOTIFY);
  }
  else if(! memcmp(pAttr->type.uuid, Dashboard_Light_ModeUUID, pAttr->type.len))
  {
      if ( offset + len > DASHBOARD_LIGHT_MODE_LEN )
      {
            status = ATT_ERR_INVALID_OFFSET;
      }
      else
      {
         // Copy pValue into the variable we point to from the attribute table.
        memcpy(pAttr->pValue + offset, pValue, len);

        // Only notify application if entire expected value is written
        if ( offset + len == DASHBOARD_LIGHT_MODE_LEN)
          paramID = DASHBOARD_LIGHT_MODE;
      }
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
    if ( pAppCBs && pAppCBs->pfnChangeCb )
      pAppCBs->pfnChangeCb( paramID ); // Call app function from stack task context.

  return status;
}
