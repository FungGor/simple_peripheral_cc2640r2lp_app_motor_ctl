/******************************************************************************

 @file  STM32MCP.c

 @brief This file contains the Code of motor control interfacing, developers will only need to modify Registers value and Register Server in this file
        "Developers should modify the following data structures if they wish to use more or less registers" will be typed


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include "STM32MCP/STM32MCP.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
static STM32MCP_regAttribute_t  *regAttribute;             // pointer to the first motor controller register attribute
static STM32MCP_CBs_t           *STM32MCP_CBs;
static STM32MCP_timerManager_t  *STM32MCP_timerManager;
static STM32MCP_timerManager_t  *STM32MCP_heartbeatManager;
static STM32MCP_uartManager_t   *STM32MCP_uartManager;
static STM32MCP_rxMsgObj_t      *rxObj;

static STM32MCP_txMsgNode_t     *STM32MCP_headPtr, *STM32MCP_tailPtr;
static uint8_t                  STM32MCP_queueSize;
static uint8_t                  *boardInfoMsg;
static uint8_t                  retransmissionCount = 0;

static uint8_t                  communicationState = STM32MCP_COMMUNICATION_DEACTIVE;

/*********************************************************************
 * Registers value
 * Developers should modify the following data structures if they wish to use more or less registers
 */
static uint8_t STM32MCP_regTargetMotorVal            = 0;
static uint8_t STM32MCP_regFlagsVal[4]               = {0, 0, 0, 0};
static uint8_t STM32MCP_regStatusVal                 = 0;
static uint8_t STM32MCP_regControlModeVal            = 0;
static uint8_t STM32MCP_regTorqueReferenceVal[2]     = {0, 0};
static uint8_t STM32MCP_regFluxReferenceVal[2]       = {0, 0};
static uint8_t STM32MCP_regBusVoltageVal[2]          = {0, 0};
static uint8_t STM32MCP_regHeatSinkTemperatureVal[2] = {0, 0};
static uint8_t STM32MCP_regMotorPowerVal[2]          = {0, 0};
static uint8_t STM32MCP_regSpeedMeasuredVal[4]       = {0, 0, 0, 0};
static uint8_t STM32MCP_regSpeedKP[2]                = {0, 0};
/**********************************************************************
 *  Register Server
 *  Developers should modify the following data structures if they wish to use more or less registers
 */
//Note: Please put the frequently accessed attribute the the top
static STM32MCP_regAttribute_t STM32MCP_registerAttributes[]=
{
    {
         STM32MCP_FLUX_REFERENCE_REG_ID,
         STM32MCP_FLUX_REFERENCE_PAYLOAD_LENGTH,
         STM32MCP_regFluxReferenceVal,
         STM32MCP_REGISTER_PERMIT_READ | STM32MCP_REGISTER_PERMIT_WRITE,
    },
    {
         STM32MCP_TORQUE_REFERENCE_REG_ID,
         STM32MCP_TORQUE_REFERENCE_PAYLOAD_LENGTH,
         STM32MCP_regTorqueReferenceVal,
         STM32MCP_REGISTER_PERMIT_READ | STM32MCP_REGISTER_PERMIT_WRITE,
    },
    {
         STM32MCP_TARGET_MOTOR_REG_ID,
         STM32MCP_TARGET_MOTOR_PAYLOAD_LENGTH,
         &STM32MCP_regTargetMotorVal,
         STM32MCP_REGISTER_PERMIT_READ | STM32MCP_REGISTER_PERMIT_WRITE,
    },
    {
         STM32MCP_FLAGS_REG_ID,
         STM32MCP_FLAGS_PAYLOAD_LENGTH,
         STM32MCP_regFlagsVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_STATUS_REG_ID,
         STM32MCP_STATUS_PAYLOAD_LENGTH,
         &STM32MCP_regStatusVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_CONTROL_MODE_REG_ID,
         STM32MCP_CONTROL_MODE_PAYLOAD_LENGTH,
         &STM32MCP_regControlModeVal,
         STM32MCP_REGISTER_PERMIT_READ | STM32MCP_REGISTER_PERMIT_WRITE,
    },
    {
         STM32MCP_BUS_VOLTAGE_REG_ID,
         STM32MCP_BUS_VOLTAGE_PAYLOAD_LENGTH,
         STM32MCP_regBusVoltageVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_HEATSINK_TEMPERATURE_REG_ID,
         STM32MCP_HEATSINK_TEMPERATURE_PAYLOAD_LENGTH,
         STM32MCP_regHeatSinkTemperatureVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_MOTOR_POWER_REG_ID,
         STM32MCP_MOTOR_POWER_PAYLOAD_LENGTH,
         STM32MCP_regMotorPowerVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_SPEED_MEASURED_REG_ID,
         STM32MCP_SPEED_MEASURED_PAYLOAD_LENGTH,
         STM32MCP_regSpeedMeasuredVal,
         STM32MCP_REGISTER_PERMIT_READ,
    },
    {
         STM32MCP_SPEED_KP_REG_ID,
         STM32MCP_SPEED_KP_PAYLOAD_LENGTH,
         STM32MCP_regSpeedKP,
         STM32MCP_REGISTER_PERMIT_READ | STM32MCP_REGISTER_PERMIT_WRITE,
    },
};
/*********************************************************************
 * LOCAL FUNCTIONS
 */
//Functions for handling queue
static void    STM32MCP_initQueue();
static uint8_t STM32MCP_getQueueSize();
static uint8_t STM32MCP_queueIsEmpty();
static void    STM32MCP_enqueueMsg(uint8_t *txMsg, uint8_t size);
static void    STM32MCP_dequeueMsg();
static void    STM32MCP_emptyQueue();
//Functional functions
static void    STM32MCP_resetFlowControlhandler();
static uint8_t STM32MCP_calChecksum(uint8_t *txMessage, uint8_t size);
/*********************************************************************
 * @fn      STM32MCP_init
 *
 * @brief   It is used to initialize the used registers in the motor control registers
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_init()
{
    STM32MCP_initQueue();

    //Initialize motor control registers
    uint8_t singleMotorServerSize = sizeof(STM32MCP_registerAttributes);
    regAttribute = (STM32MCP_regAttribute_t *) malloc(STM32MCP_NUMBER_OF_MOTORS * singleMotorServerSize);
    uint8_t n = 0;
    while(n != STM32MCP_NUMBER_OF_MOTORS)
    {
        memcpy(regAttribute + n*singleMotorServerSize , STM32MCP_registerAttributes, singleMotorServerSize);
        n++;
    }
    //free the original array if not used
    //free(STM32MCP_registerAttributes);
    //Initialize the receiving buffer and relevant variables
    rxObj = (STM32MCP_rxMsgObj_t *) malloc(sizeof(STM32MCP_rxMsgObj_t));
    rxObj->currIndex = 0;
    rxObj->payloadLength = 0xFF;
    rxObj->rxMsgBuf = (uint8_t*)malloc(sizeof(uint8_t)*STM32MCP_RX_MSG_BUFF_LENGTH);
    STM32MCP_uartManager->uartOpen();
    STM32MCP_heartbeatManager->timerStart();
}
/*********************************************************************
 * @fn      STM32MCP_registerCBs
 *
 * @brief   It is used to initialize the callback functions such that the library can notify the application when there are updates
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_registerCBs(STM32MCP_CBs_t *object)
{
    STM32MCP_CBs = object;
}
/*********************************************************************
 * @fn      STM32MCP_registerTimer
 *
 * @brief   It is used to register the timer start and timer stop function to allow the library can control the peripherals
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_registerTimer(STM32MCP_timerManager_t *object)
{
    STM32MCP_timerManager = object;
}
/*********************************************************************
 * @fn      STM32MCP_registerHeartbeat
 *
 * @brief   It is used to register the heartbeat start function to allow the library can control the heartbeat
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_registerHeartbeat(STM32MCP_timerManager_t *object)
{
    STM32MCP_heartbeatManager = object;
}
/*********************************************************************
 * @fn      STM32MCP_registerUart
 *
 * @brief   It is used to register the uart TX function to allow the library can control the peripherals
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_registerUart(STM32MCP_uartManager_t *object)
{
    STM32MCP_uartManager = object;
}
/*********************************************************************
 * @fn      STM32MCP_startCommunication
 *
 * @brief   It is used to start the communication
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_startCommunication()
{
    if(communicationState == STM32MCP_COMMUNICATION_DEACTIVE)
    {
        communicationState = STM32MCP_COMMUNICATION_ACTIVE;
    }
}
/*********************************************************************
 * @fn      STM32MCP_startCommunication
 *
 * @brief   It is used to start the communication
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_closeCommunication()
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        communicationState = STM32MCP_COMMUNICATION_DEACTIVE;
        STM32MCP_emptyQueue();
        STM32MCP_timerManager->timerStop();
    }
}
/*********************************************************************
 * @fn      STM32MCP_toggleCommunication
 *
 * @brief   It is used to toggle the communication
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_toggleCommunication()
{
    if(communicationState == STM32MCP_COMMUNICATION_DEACTIVE)
    {
        STM32MCP_startCommunication();
    }
    else if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        STM32MCP_closeCommunication();
    }
}
/*********************************************************************
 * @fn      STM32MCP_findRegister
 *
 * @brief   It is used to find register in the register file
 *
 * @param   motorID:        The motor that will be selected
 *          regID:          The register that you want to read
 *
 * @return  return the starting memory address of that register
 */
STM32MCP_regAttribute_t *STM32MCP_findRegister(uint8_t motorID, uint8_t regID)
{
    if((motorID >> 5) > STM32MCP_NUMBER_OF_MOTORS)
    {
        return ( (STM32MCP_regAttribute_t *)NULL);
    }
    else if((motorID >> 5) <= 0)
    {
        return ( (STM32MCP_regAttribute_t *)NULL);
    }
    else
    {
        uint8_t n = 0;
        uint8_t singleMotorServerSize = sizeof(STM32MCP_registerAttributes);
        STM32MCP_regAttribute_t *startingAddress = regAttribute + ((motorID >> 5) - 1 ) * singleMotorServerSize;
        while(n != singleMotorServerSize)
        {
            if((startingAddress + n) ->regID == regID)
            {
                return (STM32MCP_regAttribute_t *)(startingAddress + n);
            }
            n++;
        }
    }
    return ( (STM32MCP_regAttribute_t *)NULL);
}
/*********************************************************************
 * @fn      STM32MCP_setRegisterFrame
 *
 * @brief   It is used to write a value into a relevant motor control
            variable. See Set register frame.
 *
 * @param   motorID:        The motor that will be selected
 *          regID:          The register that you want to read
 *          payload:        The payload array
 *
 *
 * @return  queueState      If the message is successfully put into the queue, return true
 */
void STM32MCP_setRegisterFrame(uint8_t motorID, uint8_t regID, uint8_t payloadLength, uint8_t *payload)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (payloadLength + 3));
        txFrame[0] = motorID | STM32MCP_SET_REGISTER_FRAME_ID;
        txFrame[1] = payloadLength;
        txFrame[2] = regID;
        memcpy(txFrame + 3, payload, payloadLength - 1);
        txFrame[payloadLength + 2] = STM32MCP_calChecksum(txFrame, payloadLength + 2);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, payloadLength + 3);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, payloadLength + 3);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_getRegisterFrame
 *
 * @brief   It is used to read a value from a relevant motor control
            variable. See Get register frame.
 *
 * @param   motorID:        The motor that will be selected
 *          regID:          The register that you want to read
 *
 *
 * @return  queueState      If the message is successfully put into the queue, return true
 */
void STM32MCP_getRegisterFrame(uint8_t motorID, uint8_t regID)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_GET_REGISTER_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] = motorID | STM32MCP_GET_REGISTER_FRAME_ID;
        txFrame[1] = STM32MCP_GET_REGISTER_FRAME_PAYLOAD_LENGTH;
        txFrame[2] = regID;
        txFrame[3] = STM32MCP_calChecksum(txFrame, 3);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, 4);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, 4);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_executeCommandFrame
 *
 * @brief   It is used to set the command start/stop...
 *
 * @param   motorID:        The motor that will be selected
 *          commandID:      The command that you will send
 *
 *
 * @return  queueState      If the message is successfully put into the queue, return true
 */
void STM32MCP_executeCommandFrame(uint8_t motorID, uint8_t commandID)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_EXECUTE_COMMAND_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] = motorID | STM32MCP_EXECUTE_COMMAND_FRAME_ID;
        txFrame[1] = STM32MCP_EXECUTE_COMMAND_FRAME_PAYLOAD_LENGTH;
        txFrame[2] = commandID;
        txFrame[3] = STM32MCP_calChecksum(txFrame, 3);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, 4);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, 4);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_getBoardInfo
 *
 * @brief   It is used to retrieve information about the firmware currently running on the microcontroller
 *
 * @param   none
 *
 *
 * @return  queueState      If the message is successfully put into the queue, return true
 */
void STM32MCP_getBoardInfo()
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
       //Insert into packet
       uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_GET_BOARD_INFO_FRAME_PAYLOAD_LENGTH + 3));
       txFrame[0] = STM32MCP_MOTOR_LAST_ID | STM32MCP_GET_BOARD_INFO_FRAME_ID;
       txFrame[1] = STM32MCP_GET_BOARD_INFO_FRAME_PAYLOAD_LENGTH;
       txFrame[2] = STM32MCP_calChecksum(txFrame, 2);
       //Insert it into the queue
       if(STM32MCP_queueIsEmpty())
       {
           STM32MCP_timerManager->timerStart();
           STM32MCP_enqueueMsg(txFrame, 3);
           STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
       }
       else
       {
           STM32MCP_enqueueMsg(txFrame, 3);
       }
    }
}
/*********************************************************************
 * @fn      STM32MCP_executeRampFrame
 *
 * @brief   It is used to set ramp final speed and duration
 *
 * @param   motorID:        The motor that will be selected
 *          finalSpeed      The final speed of the motor
 *          duration        The duration of the ramp
 *
 * @return  none
 */
void STM32MCP_executeRampFrame(uint8_t motorID, int32_t finalSpeed, uint16_t duration)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_EXECUTE_RAMP_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] = motorID | STM32MCP_EXEC_RAMP_FRAME_ID;
        txFrame[1] = STM32MCP_EXECUTE_RAMP_FRAME_PAYLOAD_LENGTH;
        txFrame[2] =  finalSpeed        & 0xFF;
        txFrame[3] = (finalSpeed >>  8) & 0xFF;
        txFrame[4] = (finalSpeed >> 16) & 0xFF;
        txFrame[5] = (finalSpeed >> 24) & 0xFF;
        txFrame[6] =  duration          & 0xFF;
        txFrame[7] = (duration   >>  8) & 0xFF;
        txFrame[8] = STM32MCP_calChecksum(txFrame, 8);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, 9);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, 9);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_getRevupDataFrame
 *
 * @brief   It is used to set ramp final speed and duration
 *
 * @param   motorID:        The motor that will be selected
 *          stage:          The current revupstage of the motor
 *
 * @return  none
 */
void STM32MCP_getRevupDataFrame(uint8_t motorID, uint8_t stage)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_GET_REVUP_DATA_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] = motorID | STM32MCP_GET_REVUP_DATA_FRAME_ID;
        txFrame[1] = STM32MCP_GET_REVUP_DATA_FRAME_PAYLOAD_LENGTH;
        txFrame[2] = stage;
        txFrame[3] = STM32MCP_calChecksum(txFrame, 3);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, 4);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, 4);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_setRevupDataFrame
 *
 * @brief   It is used to set ramp final speed and duration
 *
 * @param   motorID:        The motor that will be selected
 *          stage:          The current revupstage of the motor
 *          finalSpeed:     The finalSpeed of the ramp
 *          finalTorque:    The finalTorque of the ramp
 *          duration:       The duration of the ramp
 *
 * @return  none
 */
void STM32MCP_setRevupDataFrame(uint8_t motorID, uint8_t stage, int32_t finalSpeed, int16_t finalTorque, uint16_t duration)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_SET_REVUP_DATA_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] =   motorID | STM32MCP_SET_REVUP_DATA_FRAME_ID;
        txFrame[1] =   STM32MCP_SET_REVUP_DATA_FRAME_PAYLOAD_LENGTH;
        txFrame[2] =   stage;
        txFrame[3] =   finalSpeed         & 0xFF;
        txFrame[4] =  (finalSpeed  >>  8) & 0xFF;
        txFrame[5] =  (finalSpeed  >> 16) & 0xFF;
        txFrame[6] =  (finalSpeed  >> 24) & 0xFF;
        txFrame[7] =   finalTorque        & 0xFF;
        txFrame[8] =  (finalTorque >>  8) & 0xFF;
        txFrame[9] =   duration           & 0xFF;
        txFrame[10] = (duration    >>  8) & 0xFF;
        txFrame[11] = STM32MCP_calChecksum(txFrame, 11);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
           STM32MCP_timerManager->timerStart();
           STM32MCP_enqueueMsg(txFrame, 12);
           STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
           STM32MCP_enqueueMsg(txFrame, 12);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_setCurrentReferencesFrame
 *
 * @brief   It is used to set the Iq and Id of the motor
 *
 * @param   motorID:        The motor that will be selected
 *          torqueReference The torque of the motor
 *          fluxReference   The flux of the motor
 *
 *
 * @return  none
 */
void STM32MCP_setCurrentReferencesFrame(uint8_t motorID, int16_t torqueReference, int16_t fluxReference)
{
    if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
    {
        //Insert into packet
        uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_SET_CURRENT_REFERENCES_FRAME_PAYLOAD_LENGTH + 3));
        txFrame[0] = motorID | STM32MCP_SET_CURRENT_REFERENCES_FRAME_ID;
        txFrame[1] = STM32MCP_SET_CURRENT_REFERENCES_FRAME_PAYLOAD_LENGTH;
        txFrame[2] = torqueReference & 0xFF;
        txFrame[3] = (torqueReference >> 8) & 0xFF;
        txFrame[4] = fluxReference & 0xFF;
        txFrame[5] = (fluxReference >> 8) & 0xFF;
        txFrame[6] = STM32MCP_calChecksum(txFrame, 6);
        //Insert it into the queue
        if(STM32MCP_queueIsEmpty())
        {
            STM32MCP_timerManager->timerStart();
            STM32MCP_enqueueMsg(txFrame, 7);
            STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        }
        else
        {
            STM32MCP_enqueueMsg(txFrame, 7);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_setSystemControlConfigFrame
 *
 * @brief   It is used for sending system control config frame
 *
 * @param   None
 *
 *
 * @return  None
 */
void STM32MCP_setSystemControlConfigFrame(uint8_t sysCmdId)
{
       //Insert into packet
       uint8_t *txFrame = (uint8_t *) malloc(sizeof(uint8_t) * (STM32MCP_SET_SYSTEM_CONTROL_CONFIG_PAYLOAD_LENGTH + 3));
       txFrame[0] = STM32MCP_MOTOR_LAST_ID | STM32MCP_SET_SYSTEM_CONTROL_CONFIG_FRAME_ID;
       txFrame[1] = STM32MCP_SET_SYSTEM_CONTROL_CONFIG_PAYLOAD_LENGTH;
       txFrame[2] = sysCmdId;
       txFrame[3] = STM32MCP_calChecksum(txFrame, 2);
       //Insert it into the queue
       if(STM32MCP_queueIsEmpty())
       {
           STM32MCP_timerManager->timerStart();
           STM32MCP_enqueueMsg(txFrame, 4);
           STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
       }
       else
       {
           STM32MCP_enqueueMsg(txFrame, 4);
       }
}
/*********************************************************************
 * @fn      STM32MCP_setTorqueRampConfiguration
 *
 * @brief   It is used for configuring driving modes: Ambler, Leisure and Sport with different Speed and Torque limits
 *          as well as acceleration and deceleration ramps.
 *
 * @param   torqueIQ :      maximum allowable torques (s16A)
 *          allowableSpeed: maximum allowable speed  (RPM)
 *          rampRate:       time durations for acceleration / deceleration in milliseconds
 *
 * @return  None
 */
void STM32MCP_setTorqueRampConfiguration(int32_t torqueIQ, int32_t allowableSpeed, uint16_t rampRate)
{
     if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
     {
         //Make a payload and insert into the packet
         uint8_t *txFrame = (uint8_t *)malloc(sizeof(uint8_t)*(STM32MCP_SET_DRIVING_MODE_CONFIG_PAYLOAD_LENGTH+3));
         txFrame[0]   = STM32MCP_MOTOR_1_ID | STM32MCP_SET_DRIVE_MODE_CONFIG_FRAME_ID;
         txFrame[1]   = STM32MCP_SET_DRIVING_MODE_CONFIG_PAYLOAD_LENGTH;
         txFrame[2]   = torqueIQ              & 0xFF;
         txFrame[3]   = (torqueIQ >> 8)        & 0xFF;
         txFrame[4]   = (torqueIQ >> 16)       & 0xFF;
         txFrame[5]   = (torqueIQ >> 24)       & 0xFF;
         txFrame[6]   =  allowableSpeed        & 0xFF;
         txFrame[7]   = (allowableSpeed >> 8)  & 0xFF;
         txFrame[8]   = (allowableSpeed >> 16) & 0xFF;
         txFrame[9]   = (allowableSpeed >> 24) & 0xFF;
         txFrame[10]  =  rampRate              & 0xFF;
         txFrame[11]  = (rampRate >> 8)        & 0xFF;
         txFrame[12]  = STM32MCP_calChecksum(txFrame,12);
         if(STM32MCP_queueIsEmpty())
         {
             STM32MCP_timerManager->timerStart();
             STM32MCP_enqueueMsg(txFrame,STM32MCP_SET_DRIVING_MODE_CONFIG_PAYLOAD_LENGTH+3);
             STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg,STM32MCP_headPtr->size);
         }
         else
         {
             STM32MCP_enqueueMsg(txFrame,STM32MCP_SET_DRIVING_MODE_CONFIG_PAYLOAD_LENGTH+3);
         }
     }
}
/*********************************************************************
 * @fn      STM32MCP_setDynamicCurrent
 *
 * @brief   It is used for changing the IQ instantly in order to change the Motor's speed
 *
 * @param   allowableSpeed:      maximum allowable torques (RPM)
 *          IQValue:             instant Current  (s16A)
 *
 * @return  None
 */
void STM32MCP_setDynamicCurrent(int32_t allowableSpeed, int32_t IQValue)
{
     if(communicationState == STM32MCP_COMMUNICATION_ACTIVE)
     {
         //Make a payload and insert into the packet
         uint8_t *txFrame = (uint8_t *)malloc(sizeof(uint8_t)*(STM32MCP_SET_DYNAMIC_TORQUE_FRAME_PAYLOAD_LENGTH+3));
         txFrame[0]   = STM32MCP_MOTOR_1_ID | STM32MCP_SET_DYNAMIC_TORQUE_FRAME_ID;
         txFrame[1]   = STM32MCP_SET_DYNAMIC_TORQUE_FRAME_PAYLOAD_LENGTH;
         txFrame[2]  =  allowableSpeed         & 0xFF;
         txFrame[3]  = (allowableSpeed >> 8)   & 0xFF;
         txFrame[4]  = (allowableSpeed >> 16)  & 0xFF;
         txFrame[5]  = (allowableSpeed >> 24)  & 0xFF;
         txFrame[6]  =  IQValue        & 0xFF;
         txFrame[7]  = (IQValue >> 8)  & 0xFF;
         txFrame[8]  = (IQValue >> 16) & 0xFF;
         txFrame[9]  = (IQValue >> 24) & 0xFF;
         txFrame[10] = STM32MCP_calChecksum(txFrame,10);
         if(STM32MCP_queueIsEmpty())
         {
             STM32MCP_timerManager->timerStart();
             STM32MCP_enqueueMsg(txFrame,STM32MCP_SET_DYNAMIC_TORQUE_FRAME_PAYLOAD_LENGTH+3);
             STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg,STM32MCP_headPtr->size);
         }
         else
         {
             STM32MCP_enqueueMsg(txFrame,STM32MCP_SET_DYNAMIC_TORQUE_FRAME_PAYLOAD_LENGTH+3);
         }
     }
}

/*********************************************************************
 * @fn      STM32MCP_setRegisterAttribute
 *
 * @brief   It is used to set motor control register attribute table INSIDE this file
 *
 * @param   motorID: The target motor
 *          regID: The register ID
 *          payloadLength: The length of the payload
 *          payload: The memory address of the payload
 *
 *
 * @return  checksum of the txMessage
 */
void STM32MCP_setRegisterAttribute(uint8_t motorID, uint8_t regID, uint8_t payloadLength, uint8_t *payload)
{
    //Do not do memory allocation, since it points to the server register. If you free it, the whole server will crake down
    STM32MCP_regAttribute_t *MCP_Register = STM32MCP_findRegister(motorID, regID);
    if(MCP_Register != NULL)
    {
        memcpy(MCP_Register->payload, payload, payloadLength);
    }
}
/*********************************************************************
 * @fn      STM32MCP_getRegisterAttribute
 *
 * @brief   It is used to get motor control register attribute table INSIDE this file
 *
 * @param   motorID: The target motor
 *          regID: The register ID
 *
 *
 * @return  A register Attribute
 */
STM32MCP_regAttribute_t *STM32MCP_getRegisterAttribute(uint8_t motorID, uint8_t regID)
{
    //Do not do memory allocation, since it points to the server register. If you free it, the whole server will crake down
    STM32MCP_regAttribute_t *MCP_Register = STM32MCP_findRegister(motorID, regID);
    if(MCP_Register != NULL)
    {
        return MCP_Register;
    }
    return (STM32MCP_regAttribute_t *)NULL;
}
/*********************************************************************
 * @fn      STM32MCP_setBoardInfo
 *
 * @brief   It is used to set the boardInfo (Note: This function does not set new board info in the motor controller.
 *          It is just a local function to set a pointer variable in this file
 *
 * @param   msg: The message
 *          The size of the message
 *
 *
 * @return  none
 */
void STM32MCP_setBoardInfo(uint8_t *msg, uint8_t size)
{
    free(boardInfoMsg);
    boardInfoMsg = (uint8_t *) malloc(sizeof(uint8_t) * size);
    memcpy(boardInfoMsg, msg, size);
}
/*********************************************************************
 * @fn      STM32MCP_flowControlHandler
 *
 * @brief   It is used for handling flow control and data packing
 *          Configure Uart interrupt to receive one byte
 *          pass the byte to this function
 *
 * @param   receivedByte: The byte received from uart receiver
 *          rxObj:      An object representing the received message and associate variables
 *
 *
 * @return  none
 */
void STM32MCP_flowControlHandler(uint8_t receivedByte)
{
    if(rxObj->currIndex < STM32MCP_RX_MSG_BUFF_LENGTH - 1)
       {
           //starting Frame processing
           if(rxObj->currIndex == 0x00)
           {
               if(STM32MCP_headPtr != NULL)
               {
                   //Reset the timer Counter
                   STM32MCP_timerManager->timerStop();
                   STM32MCP_timerManager->timerResetCounter();
                   STM32MCP_timerManager->timerStart();
                   if((receivedByte == 0xFF) || (receivedByte == 0xF0))
                   {
                       rxObj->rxMsgBuf[rxObj->currIndex++] = receivedByte;
                   }
                   //waiting for retransmission
               }
           }
           //Payload Length frame processing
           else if(rxObj->currIndex == 0x01)
           {
               //Reset the timer Counter
               STM32MCP_timerManager->timerStop();
               STM32MCP_timerManager->timerResetCounter();
               STM32MCP_timerManager->timerStart();
               rxObj->rxMsgBuf[rxObj->currIndex] = receivedByte;
               rxObj->payloadLength = rxObj->rxMsgBuf[rxObj->currIndex++];
           }
           //Data frame processing
           else if(rxObj->currIndex < rxObj->payloadLength + 0x03)
           {
               //Reset the timer Counter
               STM32MCP_timerManager->timerStop();
               STM32MCP_timerManager->timerResetCounter();
               STM32MCP_timerManager->timerStart();
               rxObj->rxMsgBuf[rxObj->currIndex++] = receivedByte;
           }

           //checksum frame processing
           if(rxObj->currIndex == rxObj->payloadLength + 0x03)
           {
               if(STM32MCP_calChecksum(rxObj->rxMsgBuf, rxObj->payloadLength + 2) == receivedByte)
               {
                   if(rxObj->rxMsgBuf[0] == 0xF0)
                   {
                       STM32MCP_timerManager->timerStop();
                       STM32MCP_timerManager->timerResetCounter();
                       STM32MCP_CBs->rxMsgCb(rxObj->rxMsgBuf,STM32MCP_headPtr);
                       //Next transmission
                       STM32MCP_dequeueMsg();
                       if(!STM32MCP_queueIsEmpty())
                       {
                           STM32MCP_timerManager->timerStart();
                           if(STM32MCP_headPtr != NULL)
                           {
                               STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
                           }
                           retransmissionCount = 0;
                       }
                   }
                   else if(rxObj->rxMsgBuf[0] == 0xFF)
                   {
                       if(STM32MCP_headPtr != NULL)
                       {
                           STM32MCP_CBs->erMsgCb(rxObj->rxMsgBuf[2]);
                       }
                       STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
                       //waiting for retransmission
                   }
               }
               STM32MCP_resetFlowControlhandler();
               //waiting for retransmission
           }
       }
       else
       {
           STM32MCP_resetFlowControlhandler();
           //Waiting for retransmission
       }
}
/*********************************************************************
 * @fn      STM32MCP_retransmission
 *
 * @brief   It is used for retransmission when timer overflows
 *
 * @param   None
 *
 *
 * @return  None
 */
void STM32MCP_retransmission()
{
    if(STM32MCP_headPtr!=NULL)
    {
        STM32MCP_timerManager->timerStart();
        STM32MCP_uartManager->uartWrite(STM32MCP_headPtr->txMsg, STM32MCP_headPtr->size);
        STM32MCP_resetFlowControlhandler();
        retransmissionCount ++;
        if(retransmissionCount >= STM32MCP_MAXIMUM_RETRANSMISSION_ALLOWANCE)
        {
            STM32MCP_CBs->exMsgCb(STM32MCP_EXCEED_MAXIMUM_RETRANSMISSION_ALLOWANCE);
        }
    }
}
/*********************************************************************
 * @fn      STM32MCP_initQueue
 *
 * @brief   To get set the head and tailPtr
 *
 * @param   None
 *
 *
 * @return  The size of the queue
 */
static void STM32MCP_initQueue()
{
    STM32MCP_headPtr = STM32MCP_tailPtr = NULL;
    STM32MCP_queueSize = 0;
}
/*********************************************************************
 * @fn      STM32MCP_getQueueSize
 *
 * @brief   To get the current size of the queue
 *
 * @param   None
 *
 *
 * @return  The size of the queue
 */
static uint8_t STM32MCP_getQueueSize()
{
    return STM32MCP_queueSize;
}
/*********************************************************************
 * @fn      STM32MCP_queueIsEmpty
 *
 * @brief   To check if he txMsg queue is empty
 *
 * @param   None
 *
 *
 * @return  None
 */
static uint8_t STM32MCP_queueIsEmpty()
{
    if(STM32MCP_headPtr == NULL && STM32MCP_tailPtr == NULL)
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}
/*********************************************************************
 * @fn      STM32MCP_enqueueMsg
 *
 * @param   *txMsg  The memory address of the fist byte of uart tx message
 *          size    The size of the uart tx message in number of bytes
 *
 *
 * @return  None
 */
static void STM32MCP_enqueueMsg(uint8_t *txMsg, uint8_t size)
{
    if(STM32MCP_getQueueSize() <= STM32MCP_MAXIMUM_NUMBER_OF_NODE)
    {
       STM32MCP_txMsgNode_t *temp;
       temp = (STM32MCP_txMsgNode_t *)malloc(sizeof(STM32MCP_txMsgNode_t));
       temp->txMsg = txMsg;
       temp->size = size;
       temp->next = NULL;
       if(STM32MCP_tailPtr == NULL)
       {
           STM32MCP_headPtr = STM32MCP_tailPtr = temp;
       }
       else
       {
           STM32MCP_tailPtr->next = temp;
           STM32MCP_tailPtr = temp;
       }
       STM32MCP_queueSize++;
    }
    else
    {
       //Throw expection
       STM32MCP_CBs->exMsgCb(STM32MCP_QUEUE_OVERLOAD);
       free(txMsg);
    }
}
/*********************************************************************
 * @fn      STM32MCP_dequeueMsg
 *
 * @brief   It is used for dequeuing the txMsg queue
 *
 * @param   None
 *
 *
 * @return  None
 */
static void STM32MCP_dequeueMsg()
{
    STM32MCP_txMsgNode_t *temp;
    if(STM32MCP_headPtr == NULL)
    {
        return;
    }
    else
    {
        temp = STM32MCP_headPtr;
        STM32MCP_headPtr = STM32MCP_headPtr->next;
        if(STM32MCP_headPtr == NULL)
        {
            STM32MCP_tailPtr = NULL;
        }
        STM32MCP_queueSize--;
        free(temp->txMsg);
        free(temp);
    }
}
/*********************************************************************
 * @fn      STM32MCP_emptyQueue
 *
 * @brief   It is used for empty the txMsg queue
 *
 * @param   None
 *
 *
 * @return  None
 */
static void STM32MCP_emptyQueue()
{
    while(!STM32MCP_queueIsEmpty())
    {
        STM32MCP_dequeueMsg();
    }
}
/*********************************************************************
 * @fn      STM32MCP_resetFlowControlhandler
 *
 * @brief   It is used for resetting the current Index and payloadLength
 *
 * @param   receivedByte: The byte received from uart receiver
 *          rxObj:      An object representing the received message and associate variables
 *
 *
 * @return  none
 */
static void STM32MCP_resetFlowControlhandler()
{
    rxObj->currIndex = 0x00;
    rxObj->payloadLength = 0xFF;
}
/*********************************************************************
 * @fn      STM32MCP_calChecksum
 *
 * @brief   It is used to calculate the checksum
 *
 * @param   *txMsg  The memory address of the fist byte of uart tx message
 *          size    The size of the uart tx message in number of bytes
 *
 *
 * @return  checksum of the txMessage
 */
static uint8_t STM32MCP_calChecksum(uint8_t *msg, uint8_t size)
{
    uint16_t total = 0;
    uint8_t n = 0;
    while(n != size)
    {
        total += msg[n];
        n++;
    }
    return (total & 0xFF) + ((total >> 8) & 0xFF);
}
