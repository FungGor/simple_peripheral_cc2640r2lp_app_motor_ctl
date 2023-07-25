/*
 * STM32MCP.h
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *
 *
 *
 *                                          Before you start using this library, you must read UM1052(Pg161-178).
 *
 *
 *
 *
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *  Created on:   20 Aug 2021 by Siu Yeung Yik
 *  Last Updated: 25 Jan 2022 by Siu Yeung Yik
 */
#ifndef STM32MCP_STM32MCP_H_
#define STM32MCP_STM32MCP_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define STM32MCP_NUMBER_OF_REGISTERS                                                         0x0A
#define STM32MCP_NUMBER_OF_MOTORS                                                            0x01

//The maximum number of node that the buffer can hold
#define STM32MCP_MAXIMUM_NUMBER_OF_NODE                                                      0x05

//The maximum number of retransmission is allowed
#define STM32MCP_MAXIMUM_RETRANSMISSION_ALLOWANCE                                            0x0A

//Maximum rx buffer length
#define STM32MCP_RX_MSG_BUFF_LENGTH                                                          0x0A

//Heart beat period (in ms)
#define STM32MCP_HEARTBEAT_PERIOD                                                            1000

//Communication State
#define STM32MCP_COMMUNICATION_ACTIVE                                                        0x00
#define STM32MCP_COMMUNICATION_DEACTIVE                                                      0x01

//Register read and write permission
#define STM32MCP_REGISTER_PERMIT_READ                                                        0x01
#define STM32MCP_REGISTER_PERMIT_WRITE                                                       0x02

//Frame id
#define STM32MCP_MOTOR_LAST_ID                                                               0x00
#define STM32MCP_MOTOR_1_ID                                                                  0x20
#define STM32MCP_MOTOR_2_ID                                                                  0x40

#define STM32MCP_SET_REGISTER_FRAME_ID                                                       0x01
#define STM32MCP_GET_REGISTER_FRAME_ID                                                       0x02
#define STM32MCP_EXECUTE_COMMAND_FRAME_ID                                                    0x03
#define STM32MCP_GET_BOARD_INFO_FRAME_ID                                                     0x06
#define STM32MCP_EXEC_RAMP_FRAME_ID                                                          0x07
#define STM32MCP_GET_REVUP_DATA_FRAME_ID                                                     0x08
#define STM32MCP_SET_REVUP_DATA_FRAME_ID                                                     0x09
#define STM32MCP_SET_CURRENT_REFERENCES_FRAME_ID                                             0x0A
#define STM32MCP_SET_SYSTEM_CONTROL_CONFIG_FRAME_ID                                          0x0E
#define STM32MCP_SET_DRIVE_MODE_CONFIG_FRAME_ID                                              0x13
#define STM32MCP_SET_DYNAMIC_TORQUE_FRAME_ID                                                 0x14


//payload for frame id (except for set_register_frame ID
#define STM32MCP_GET_REGISTER_FRAME_PAYLOAD_LENGTH                                           0x01
#define STM32MCP_EXECUTE_COMMAND_FRAME_PAYLOAD_LENGTH                                        0x01
#define STM32MCP_EXECUTE_RAMP_FRAME_PAYLOAD_LENGTH                                           0x06
#define STM32MCP_GET_BOARD_INFO_FRAME_PAYLOAD_LENGTH                                         0x00
#define STM32MCP_SET_CURRENT_REFERENCES_FRAME_PAYLOAD_LENGTH                                 0x04
#define STM32MCP_SET_REVUP_DATA_FRAME_PAYLOAD_LENGTH                                         0x09
#define STM32MCP_GET_REVUP_DATA_FRAME_PAYLOAD_LENGTH                                         0x01
#define STM32MCP_SET_SYSTEM_CONTROL_CONFIG_PAYLOAD_LENGTH                                    0x01
#define STM32MCP_SET_DRIVING_MODE_CONFIG_PAYLOAD_LENGTH                                      0x0A
#define STM32MCP_SET_DYNAMIC_TORQUE_FRAME_PAYLOAD_LENGTH                                     0x08

//Error code
#define STM32MCP_BAD_FRAME_ID                                                                0x01
#define STM32MCP_WRITE_ON_READ_ONLY                                                          0x02
#define STM32MCP_READ_NOT_ALLOWED                                                            0x03
#define STM32MCP_BAD_TARGET_DRIVE                                                            0x04
#define STM32MCP_OUT_OF_RANGE                                                                0x05
#define STM32MCP_BAD_COMMAND_ID                                                              0x07
#define STM32MCP_OVERRUN_ERROR                                                               0x08
#define STM32MCP_TIMEOUT_ERROR                                                               0x09
#define STM32MCP_BAD_CRC                                                                     0x0A
//#define STM32MCP_BAD_TARGET_DRIVE                                                            0x0B

//Exception code
#define STM32MCP_QUEUE_OVERLOAD                                                              0x01
#define STM32MCP_EXCEED_MAXIMUM_RETRANSMISSION_ALLOWANCE                                     0x02

//Register ID
#define STM32MCP_TARGET_MOTOR_REG_ID                                                         0x00
#define STM32MCP_FLAGS_REG_ID                                                                0x01
#define STM32MCP_STATUS_REG_ID                                                               0x02
#define STM32MCP_CONTROL_MODE_REG_ID                                                         0x03
#define STM32MCP_SPEED_REFERENCE_REG_ID                                                      0x04
#define STM32MCP_SPEED_KP_REG_ID                                                             0x05
#define STM32MCP_SPEED_KI_REG_ID                                                             0x06
#define STM32MCP_SPEED_KD_REG_ID                                                             0x07
#define STM32MCP_TORQUE_REFERENCE_REG_ID                                                     0x08
#define STM32MCP_TORQUE_KP_REG_ID                                                            0x09
#define STM32MCP_TORQUE_KI_REG_ID                                                            0x0A
#define STM32MCP_TORQUE_KD_REG_ID                                                            0x0B
#define STM32MCP_FLUX_REFERENCE_REG_ID                                                       0x0C
#define STM32MCP_FLUX_KP_REG_ID                                                              0x0D
#define STM32MCP_FLUX_KI_REG_ID                                                              0x0E
#define STM32MCP_FLUX_KD_REG_ID                                                              0x0F
#define STM32MCP_OBSERVER_C1_REG_ID                                                          0x10
#define STM32MCP_OBSERVER_C2_REG_ID                                                          0x11
#define STM32MCP_CORDIC_OBSERVER_C1_REG_ID                                                   0x12
#define STM32MCP_CORDIC_OBSERVER_C2_REG_ID                                                   0x13
#define STM32MCP_PPL_KI_REG_ID                                                               0x14
#define STM32MCP_PPL_KP_REG_ID                                                               0x15
#define STM32MCP_FLUX_WEAKENING_KP_REG_ID                                                    0x16
#define STM32MCP_FLUX_WEAKENING_KI_REG_ID                                                    0x17
#define STM32MCP_FLUX_WEAKENING_BUS_VOLTAGE_ALLOWED_PERCENTAGE_REFERENCE_REG_ID              0x18
#define STM32MCP_BUS_VOLTAGE_REG_ID                                                          0x19
#define STM32MCP_HEATSINK_TEMPERATURE_REG_ID                                                 0x1A
#define STM32MCP_MOTOR_POWER_REG_ID                                                          0x1B
#define STM32MCP_DAC_OUT_1_REG_ID                                                            0x1C
#define STM32MCP_DAC_OUT_2_REG_ID                                                            0x1D
#define STM32MCP_SPEED_MEASURED_REG_ID                                                       0x1E
#define STM32MCP_TORQUE_MEASURED_REG_ID                                                      0x1F
#define STM32MCP_FLUX_MEASURED_REG_ID                                                        0x20
#define STM32MCP_FLUX_WEAKENING_BUS_VOLTAGE_ALLOWED_PERCENTAGE_MEASURED_REG_ID               0x21
#define STM32MCP_REVUP_STAGE_NUMBERS_REG_ID                                                  0x22
#define STM32MCP_MAXIMUM_APPLICATION_SPEED_REG_ID                                            0x3F
#define STM32MCP_MINIMUM_APPLICATION_SPEED_REG_ID                                            0x40
#define STM32MCP_IQ_REFERENCE_IN_SPEED_MODE_REG_ID                                           0x41
#define STM32MCP_EXPECTED_BEMF_LEVEL_PLL_REG_ID                                              0x42
#define STM32MCP_OBSERVED_BEMF_LEVEL_PLL_REG_ID                                              0x42
#define STM32MCP_EXPECTED_BEMF_LEVEL_CORDIC_REG_ID                                           0x44
#define STM32MCP_OBSERVED_BEMF_LEVEL_CORDIC_REG_ID                                           0x45
#define STM32MCP_Feedforward_1Q_REG_ID                                                       0x46
#define STM32MCP_Feedforward_1D_REG_ID                                                       0x47
#define STM32MCP_Feedforward_2_REG_ID                                                        0x48
#define STM32MCP_Feedforward_VQ_REG_ID                                                       0x49
#define STM32MCP_Feedforward_VD_REG_ID                                                       0x4A
#define STM32MCP_Feedforward_VQ_PI_OUT_REG_ID                                                0x4B
#define STM32MCP_Feedforward_VD_PI_OUT_REG_ID                                                0x4C
#define STM32MCP_RAMP_FINAL_SPEED_REG_ID                                                     0x5B
#define STM32MCP_RAMP_DURATION_REG_ID                                                        0x5C

//Payload length for regID
#define STM32MCP_TARGET_MOTOR_PAYLOAD_LENGTH                                                 0x02
#define STM32MCP_FLAGS_PAYLOAD_LENGTH                                                        0x05
#define STM32MCP_STATUS_PAYLOAD_LENGTH                                                       0x02
#define STM32MCP_CONTROL_MODE_PAYLOAD_LENGTH                                                 0x02
#define STM32MCP_SPEED_REFERENCE_PAYLOAD_LENGTH                                              0x05
#define STM32MCP_SPEED_KP_PAYLOAD_LENGTH                                                     0x03
#define STM32MCP_SPEED_KI_PAYLOAD_LENGTH                                                     0x03
#define STM32MCP_SPEED_KD_PAYLOAD_LENGTH                                                     0x03
#define STM32MCP_TORQUE_REFERENCE_PAYLOAD_LENGTH                                             0x03
#define STM32MCP_TORQUE_KP_PAYLOAD_LENGTH                                                    0x03
#define STM32MCP_TORQUE_KI_PAYLOAD_LENGTH                                                    0x03
#define STM32MCP_TORQUE_KD_PAYLOAD_LENGTH                                                    0x03
#define STM32MCP_FLUX_REFERENCE_PAYLOAD_LENGTH                                               0x03
#define STM32MCP_FLUX_KP_PAYLOAD_LENGTH                                                      0x03
#define STM32MCP_FLUX_KI_PAYLOAD_LENGTH                                                      0x03
#define STM32MCP_FLUX_KD_PAYLOAD_LENGTH                                                      0x03
#define STM32MCP_OBSERVER_C1_PAYLOAD_LENGTH                                                  0x03
#define STM32MCP_OBSERVER_C2_PAYLOAD_LENGTH                                                  0x03
#define STM32MCP_CORDIC_OBSERVER_C1_PAYLOAD_LENGTH                                           0x03
#define STM32MCP_CORDIC_OBSERVER_C2_PAYLOAD_LENGTH                                           0x03
#define STM32MCP_PPL_KI_PAYLOAD_LENGTH                                                       0x03
#define STM32MCP_PPL_KP_PAYLOAD_LENGTH                                                       0x03
#define STM32MCP_FLUX_WEAKENING_KP_PAYLOAD_LENGTH                                            0x03
#define STM32MCP_FLUX_WEAKENING_KI_PAYLOAD_LENGTH                                            0x03
#define STM32MCP_FLUX_WEAKENING_BUS_VOLTAGE_ALLOWED_PERCENTAGE_REFERENCE_PAYLOAD_LENGTH      0x03
#define STM32MCP_BUS_VOLTAGE_PAYLOAD_LENGTH                                                  0x03
#define STM32MCP_HEATSINK_TEMPERATURE_PAYLOAD_LENGTH                                         0x03
#define STM32MCP_MOTOR_POWER_PAYLOAD_LENGTH                                                  0x03
#define STM32MCP_DAC_OUT_1_PAYLOAD_LENGTH                                                    0x02
#define STM32MCP_DAC_OUT_2_PAYLOAD_LENGTH                                                    0x02
#define STM32MCP_SPEED_MEASURED_PAYLOAD_LENGTH                                               0x05
#define STM32MCP_TORQUE_MEASURED_PAYLOAD_LENGTH                                              0x03
#define STM32MCP_FLUX_MEASURED_PAYLOAD_LENGTH                                                0x03
#define STM32MCP_FLUX_WEAKENING_BUS_VOLTAGE_ALLOWED_PERCENTAGE_MEASURED_PAYLOAD_LENGTH       0x03
#define STM32MCP_REVUP_STAGE_NUMBERS_PAYLOAD_LENGTH                                          0x02
#define STM32MCP_MAXIMUM_APPLICATION_SPEED_PAYLOAD_LENGTH                                    0x05
#define STM32MCP_MINIMUM_APPLICATION_SPEED_PAYLOAD_LENGTH                                    0x05
#define STM32MCP_IQ_REFERENCE_IN_SPEED_MODE_PAYLOAD_LENGTH                                   0x03
#define STM32MCP_EXPECTED_BEMF_LEVEL_PLL_PAYLOAD_LENGTH                                      0x03
#define STM32MCP_OBSERVED_BEMF_LEVEL_PLL_PAYLOAD_LENGTH                                      0x03
#define STM32MCP_EXPECTED_BEMF_LEVEL_CORDIC_PAYLOAD_LENGTH                                   0x03
#define STM32MCP_OBSERVED_BEMF_LEVEL_CORDIC_PAYLOAD_LENGTH                                   0x03
#define STM32MCP_Feedforward_1Q_PAYLOAD_LENGTH                                               0x05
#define STM32MCP_Feedforward_1D_PAYLOAD_LENGTH                                               0x05
#define STM32MCP_Feedforward_2_PAYLOAD_LENGTH                                                0x05
#define STM32MCP_Feedforward_VQ_PAYLOAD_LENGTH                                               0x03
#define STM32MCP_Feedforward_VD_PAYLOAD_LENGTH                                               0x03
#define STM32MCP_Feedforward_VQ_PI_OUT_PAYLOAD_LENGTH                                        0x03
#define STM32MCP_Feedforward_VD_PI_OUT_PAYLOAD_LENGTH                                        0x03
#define STM32MCP_RAMP_FINAL_SPEED_PAYLOAD_LENGTH                                             0x05
#define STM32MCP_RAMP_DURATION_PAYLOAD_LENGTH                                                0x03

//Command list
#define STM32MCP_START_MOTOR_COMMAND_ID                                                      0x01
#define STM32MCP_STOP_MOTOR_COMMAND_ID                                                       0x02
#define STM32MCP_STOP_RAMP_COMMAND_ID                                                        0x03
#define STM32MCP_START_STOP_COMMAND_ID                                                       0x06
#define STM32MCP_FAULT_ACK_COMMAND_ID                                                        0x07
#define STM32MCP_ENCODER_ALIGN_COMMAND_ID                                                    0x08

//system command ID
#define STM32MCP_POWER_OFF                                                                   0x00
#define STM32MCP_POWER_ON                                                                    0x01
#define STM32MCP_HEARTBEAT                                                                   0x02
#define STM32MCP_GET_SERIAL_NUMBER                                                           0x03
#define STM32MCP_TAIL_LIGHT_ON                                                               0x04
#define STM32MCP_TAIL_LIGHT_OFF                                                              0x05
#define STM32MCP_TOGGLE_TAIL_LIGHT                                                           0x06

#define STM32MCP_SYSTEM_MAXIMUM_VOLTAGE                                                      42000  // for 37000 mV battery, max V is 42000 mV - we will assume 42000 mV for all calculations
#define STM32MCP_SYSTEM_MIMIMUM_VOLTAGE                                                      29000  // for 37000 mV battery, min usable V is 29000 mV - below this will damage the battery pack
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * @Structure STM32MCP_rxMsg
 *
 * @brief     When the motor controller sends the received message back, it stores the message here
 *
 * @data      rxMsg - The memory message to the received message, the size of the message is the second index rxMsg[1]
 *            motorID - The motor ID described in STM32MCP
 *            frameID - The frame ID described in STM32MCP
 */
typedef struct
{
    uint8_t frameID;
    uint8_t rxPayloadLength;
    uint8_t txPayloadLength;
    uint8_t *rxPayload;
    uint8_t *txPayload;
}STM32MCP_rxMsg_t;
/*********************************************************************
 * @Structure STM32MCP_exMsg
 *
 * @brief     When the motor controller sends the exception, it stores the message here
 *
 * @data      exceptionCode - The exception code described in STM32MCP
 */
typedef struct
{
    uint8_t exceptionCode;
}STM32MCP_exMsg_t;
/*********************************************************************
 * @Structure STM32MCP_erMsg_t
 *
 * @brief     When the motor controller sends the error back, it stores the message here
 *
 * @data      exceptionCode - The error code described in STM32MCP
 */
typedef struct
{
    uint8_t errorCode;
}STM32MCP_erMsg_t;
/*********************************************************************
 * @Structure STM32MCP_regAttribute_t
 *
 * @brief     It is used to define a stm32 motor control register
 *            Please see UM1052 (Pg 161 - 178) for more details
 *
 * @data      regID:            The register ID
 *            payloadLength:    The length of the payload
 *            payload:          The pointer of the payload
 *            permission:       Read/Write permission
 */
typedef struct
{
    uint8_t regID;
    uint8_t payloadLength;
    uint8_t *payload;
    uint8_t permission;
} STM32MCP_regAttribute_t;
/*********************************************************************
 * @Structure STM32MCP_txMsgNode
 *
 * @brief     It defines a message Node to be transmit.
 *            Multiple node will be linked together
 *            to form a FIFO transmit buffer
 *
 * @data      txMsg:   The pointer to an array of bytes
 *            size:    The size of the array
 *            next:    The pointer of the next tx message node
 */
typedef struct STM32MCP_txMsgNode_t STM32MCP_txMsgNode_t;
struct STM32MCP_txMsgNode_t
{
    uint8_t *txMsg;
    uint8_t size;
    STM32MCP_txMsgNode_t *next;
};
/*********************************************************************
 * @Structure STM32MCP_rxMsgObj_t
 *
 * @brief     It stores the array of received bytes, payload length
 *            and the current index of the received byte. When all
 *            bytes are received, it will go through CRC checking
 *            and pass the message to upper layer.
 *
 * @data      rxMsgBuf:      An array of received bytes
 *            currIndex:     The current index of the received byte
 *            payloadLength: The expected payload length of the data
 */
typedef struct
{
    uint8_t *rxMsgBuf;
    uint8_t currIndex;
    uint8_t payloadLength;
} STM32MCP_rxMsgObj_t;
/*********************************************************************
 * @Structure STM32MCP_CBs_t
 *
 * @brief     It defines a set of function pointer that the server
 *            wants to point to the application functions
 *
 * @data      rxHandler:  Called when received data passed CRC checking, it will be passed to rxMsgCb
 *            exHandler:  Called when there are exception
 *            erHandler:  Called when there are error
 */
typedef void (*rxHandler)(uint8_t *rxMsg, STM32MCP_txMsgNode_t *STM32MCP_txMsgNode);
typedef void (*exHandler)(uint8_t exceptionCode);
typedef void (*erHandler)(uint8_t errorCode);
typedef struct
{
    rxHandler       rxMsgCb;
    exHandler       exMsgCb;
    erHandler       erMsgCb;
}STM32MCP_CBs_t;
/*********************************************************************
 * @Structure STM32MCP_uartManager_t
 *
 * @brief     It defines a set of function pointer that the server
 *            wants to point to the application functions
 *
 * @data      uartOpen: Called when the application wants to initialize the uart peripheral
 *            uartWrite: Called when the application wants to write to the uart peripheral
 *            uartRead: Called when the application wants to read from the uart peripheral
 *            uartClose: Called when the application wants to terminate the uart peripheral
 */
typedef void (*uartOpen)(void);
typedef void (*uartRead)(uint8_t *message, uint8_t size);
typedef void (*uartWrite)(uint8_t *message, uint8_t size);
typedef void (*uartClose)(void);
typedef struct
{
    uartOpen uartOpen;
    uartRead uartRead;
    uartWrite uartWrite;
    uartClose uartClose;
}STM32MCP_uartManager_t;
/*********************************************************************
 * @Structure STM32MCP_timerManager_t
 *
 * @brief     It defines a set of function pointer that the server
 *            wants to point to the application functions
 *
 * @data      timerStart: Called when the server wants to start the retransmission timer
 *            timerResetCounter: Reset the counter to zero
 *            timerStop:  Called when the server wants to stop the retransmission timer
 */
typedef void (*STM32MCP_timerStart)(void);
typedef void (*STM32MCP_timerResetCounter)(void);
typedef void (*STM32MCP_timerStop)(void);
typedef struct
{
    STM32MCP_timerStart             timerStart;
    STM32MCP_timerResetCounter      timerResetCounter;
    STM32MCP_timerStop              timerStop;
}STM32MCP_timerManager_t;
/*********************************************************************
 * FUNCTIONS
 */
/*==========Mandatory Function has to be added=========*/
extern void STM32MCP_init();
extern void STM32MCP_registerCBs(STM32MCP_CBs_t *STM32MCP_CBs);//Use it in the main function
extern void STM32MCP_registerHeartbeat(STM32MCP_timerManager_t *object);
extern void STM32MCP_registerTimer(STM32MCP_timerManager_t *STM32MCP_timerManager);//Added to peripherals already, just use UDHAL_init()
extern void STM32MCP_registerUart(STM32MCP_uartManager_t *STM32MCP_uartManager);//Added to peripherals already, just use UDHAL_init()
/*==========Mandatory Function has to be added=========*/
extern void STM32MCP_startCommunication();
extern void STM32MCP_closeCommunication();
extern void STM32MCP_toggleCommunication();
/*==============================================================*/
extern STM32MCP_regAttribute_t *STM32MCP_findRegister(uint8_t motorID, uint8_t regID);
/*=========================================================API functions=============================================================*/
extern void STM32MCP_setRegisterFrame(uint8_t motorID, uint8_t regID, uint8_t payloadLength, uint8_t *payload);
extern void STM32MCP_getRegisterFrame(uint8_t motorID, uint8_t regID);
extern void STM32MCP_executeCommandFrame(uint8_t motorID, uint8_t commandID);
extern void STM32MCP_getBoardInfo();
extern void STM32MCP_executeRampFrame(uint8_t motorID, int32_t finalSpeed, uint16_t duration);
extern void STM32MCP_getRevupDataFrame(uint8_t motorID, uint8_t stage);
extern void STM32MCP_setRevupDataFrame(uint8_t motorID, uint8_t stage, int32_t finalSpeed, int16_t finalTorque, uint16_t duration);
extern void STM32MCP_setCurrentReferencesFrame(uint8_t motorID, int16_t torqueReference, int16_t fluxReference);
extern void STM32MCP_setSystemControlConfigFrame(uint8_t sysCmdId);
/*=========================================================E-SCOOTER Control Functions================================================*/
extern void STM32MCP_setTorqueRampConfiguration(int32_t torqueIQ, int32_t allowableSpeed, uint16_t rampRate);
extern void STM32MCP_setDynamicCurrent(int32_t allowableSpeed, int32_t IQValue);
/*====================================================================================================================================*/
/*=================================================Functions to set the internal registers============================================*/
extern void STM32MCP_setRegisterAttribute(uint8_t motorID, uint8_t regID, uint8_t payloadLength, uint8_t *payload);
extern STM32MCP_regAttribute_t *STM32MCP_getRegisterAttribute(uint8_t motorID, uint8_t regID);
extern void STM32MCP_setBoardInfo(uint8_t *msg, uint8_t size);
/*====================================================================================================================================*/
/*===============================================Functions to be added to callback functions==========================================*/
extern void STM32MCP_flowControlHandler(uint8_t receivedByte);//Add to UART Rx ISR or UART RX callback
extern void STM32MCP_retransmission();
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* STM32MCP_STM32MCP_H_ */
