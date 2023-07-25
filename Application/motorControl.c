/******************************************************************************

 @file  motorControl.c

 @brief This file contains the Code of motor control interfacing


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <ti/drivers/I2C.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/GPIO.h>

#include "UDHAL/UDHAL.h"
#include "STM32MCP/STM32MCP.h"
#include "Controller.h"
#include "Dashboard.h"
#include "Battery.h"
#include "motorControl.h"
#include "util.h"
#include "periodicCommunication.h"
#include "simple_peripheral.h"
#include "brakeAndThrottle.h"
#include "ledControl.h"
#include "lightControl.h"
#include "buzzerControl.h"
#include "powerOnTime.h"
#include "dataAnalysis.h"
#include "singleButton/singleButton.h"
#include "peripheral.h"
#include "TSL2561/TSL2561.h"

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
 * GLOBAL VARIABLES
 */
/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t motorcontrol_i2cOpenStatus = 0;
static simplePeripheral_bleCBs_t *motorcontrol_bleCBs;
static uint8_t motorControl_getGAPRole_taskCreate_flag = 0;
/**********************************************************************
 *  Local functions
 */
static void motorcontrol_processGetRegisterFrameMsg(uint8_t *txPayload, uint8_t txPayloadLength, uint8_t *rxPayload, uint8_t rxPayloadLength);

static void motorcontrol_rxMsgCb(uint8_t *rxMsg, STM32MCP_txMsgNode_t *STM32MCP_txMsgNode);
static void motorcontrol_exMsgCb(uint8_t exceptionCode);
static void motorcontrol_erMsgCb(uint8_t errorCode);

//static void motorcontrol_brakeAndThrottleCB(uint16_t allowableSpeed, uint16_t throttlePercent, uint8_t errorMsg);
static void motorcontrol_brakeAndThrottleCB(uint16_t allowableSpeed, uint16_t IQValue, uint8_t errorMsg);
static void motorcontrol_controllerCB(uint8_t paramID);
static void motorcontrol_dashboardCB(uint8_t paramID);
static void motorcontrol_singleButtonCB(uint8_t messageID);

static void motorcontrol_getGAPROLE(void);

//uint16_t throttle_Percent;

//extern void motorcontrol_setGatt(uint16_t serviceUUID, uint8_t charteristics, uint8_t payloadLength, uint8_t* payload);
/*********************************************************************
 * TYPEDEFS
 */
static STM32MCP_CBs_t STM32MCP_CBs =
{
     motorcontrol_rxMsgCb,
     motorcontrol_exMsgCb,
     motorcontrol_erMsgCb
};

static ControllerCBs_t ControllerCBs =
{
     motorcontrol_controllerCB
};

static DashboardCBs_t DashboardCBs =
{
     motorcontrol_dashboardCB
};

static singleButtonCBs_t singleButtonCBs =
{
     motorcontrol_singleButtonCB
};

static brakeAndThrottle_CBs_t brakeAndThrottle_CBs =
{
     motorcontrol_brakeAndThrottleCB                    // brake and throttle callback -> what happens when brake & throttle interrupts
};

//static lightControl_CBs_t lightControl_CBs =
//{
//     motorcontrol_lightControlCB
//};

//static buzzerControl_CBs_t buzzerControl_CBs =
//{
//     motorcontrol_buzzerControlCB
//};

/*********************************************************************
 * @fn      MC_init
 *
 * @brief   Initialization function for the Motor Control Task.
 *
 * @param   none
 *
 * @return  none
 */
uint8_t mccheck = 0;
void motorcontrol_init(void)
{
// Activate NVS_internal to Recall the last set of saved data in memory
//    NVS_init();

    UDHAL_init();
    mccheck = 1;

    Controller_RegisterAppCBs(&ControllerCBs);
    mccheck = 2;

    Dashboard_RegisterAppCBs(&DashboardCBs);
    mccheck = 3;

    STM32MCP_init();
    STM32MCP_registerCBs(&STM32MCP_CBs);
    STM32MCP_startCommunication();    // Not activated
    mccheck = 4;

    periodicCommunication_start();
    mccheck = 5;

    dataAnalysis_init();                // Initiate data analytics
    mccheck = 6;

    brakeAndThrottle_init();
    brakeAndThrottle_registerCBs(&brakeAndThrottle_CBs);
    brakeAndThrottle_start();
    mccheck = 7;

    singleButton_init();
    singleButton_registerCBs(&singleButtonCBs);
    mccheck = 8;

    buzzerControl_init();               // Initiate buzzer on dashboard
    mccheck = 9;

    motorcontrol_i2cOpenStatus = UDHAL_getI2CStatus();
// if i2c open successfully, motorcontrol_i2cOpenStatus = 1,
// if i2c opens unsuccessfully, motorcontrol_i2cOpenStatus = 0 -> do not activate TSL2561 light sensor and Led Display
    if (motorcontrol_i2cOpenStatus == 1) // TSL2561 and ledDisplay require I2C.  If I2C did not open successfully, TSL2561 and ledDisplay will not be initialized
    {
        TSL2561_init();
        //ledControl_init();
    }
    mccheck = 10;

    lightControl_init( motorcontrol_i2cOpenStatus );                            // UDHAL_I2C must be initiated before ightControl_init();
    mccheck = 11;

    powerOnTime_init();           // merged with lightControl 20230705
    mccheck = 12;

// If GAPRole_createTask initiated and called successfully, gapRole_getGAPRole_taskCreate_flag() returns 1
// motorControl_getGAPRole_taskCreate_flag
    motorControl_getGAPRole_taskCreate_flag = gapRole_getGAPRole_taskCreate_flag();
    mccheck = 13;

}

/*********************************************************************
 * @fn      motorcontrol_registerCB
 *
 * @brief   When the motor controller sends the get register frame message
 *
 * @param   STM32MCP_rxMsg_t - The memory message to the received message, the size of the message is the second index rxMsg[1]
 *
 * @return  None.
 */
void motorcontrol_registerCB(simplePeripheral_bleCBs_t *obj)
{
    motorcontrol_bleCBs = obj;
}

/*********************************************************************
 * @fn      motorcontrol_processGetRegisterFrameMsg
 *
 * @brief   When the motor controller sends the get register frame message
 *
 * @param   STM32MCP_rxMsg_t - The memory message to the received message, the size of the message is the second index rxMsg[1]
 *
 * @return  None.
 */
static void motorcontrol_processGetRegisterFrameMsg(uint8_t *txPayload, uint8_t txPayloadLength, uint8_t *rxPayload, uint8_t rxPayloadLength)
{
    uint8_t regID = txPayload[0];
    switch(regID)
    {
    case STM32MCP_BUS_VOLTAGE_REG_ID:
        {
            uint16_t voltage_mV = *((uint16_t*) rxPayload) * 1000; // rxPayload in V, voltage in mV

            // send battery voltage to dataAnalysis
            //dataAnalysis_mcData(voltageID, &voltage);

            // this is battery percentage.  batteryLevel = batteryPercentage.
            //uint8_t batteryLevel = (uint8_t) ((((uint32_t)voltage - STM32MCP_SYSTEM_MIMIMUM_VOLTAGE)*100/(STM32MCP_SYSTEM_MAXIMUM_VOLTAGE - STM32MCP_SYSTEM_MIMIMUM_VOLTAGE)) & 0xFF);
            break;
        }
        // !!!!!!!!!!!!!!!!!!!!!!!!!  current sensor to be added to MCU.  Reserved case for current measurement
    //case STM32MCP_BUS_CURRENT_REG_ID:
        //{
            // keep current in mV - do not convert it to A
//          uint16_t current_mA = *((uint8_t*) rxPayload) * 1000;
//
            //send current to dataAnalysis
            //dataAnalysis_mcData(currentID, &current);
            //break;
        //}
    case STM32MCP_HEATSINK_TEMPERATURE_REG_ID:
        {
            int8_t heatSinkTemperature_Celcius = (int8_t) (*((uint8_t*) rxPayload) & 0xFF);     // temperature can be a negative value, unless it is in Kelvin
            //send heatSinkTemperature to dataAnalysis
            //dataAnalysis_mcData(heatSinkTemperatureID, &heatSinkTemperature);
            break;
        }
    case STM32MCP_SPEED_MEASURED_REG_ID:
        {
            int32_t rawRPM = *((int32_t*) rxPayload);
            if(rawRPM >= 0)
            {
                uint16_t rpm = (uint16_t) (rawRPM & 0xFFFF);
                //send rpm to dataAnalysis
                //dataAnalysis_mcData(rpmID, &rpm);
            }
            else
            {
                // what if rawRPM is negative???  Would rawRPM be negative??? Rolling in reverse or measurement error???
            }
            break;
        }
// ********************    Need to create new REG_IDs
    //case STM32MCP_MOTOR_TEMPERATURE_REG_ID:
        //{

            //break;
        //}
    //case STM32MCP_CONTROLLER_ERRORCODE_REG_ID:
        //{

            //break;
        //}
    default:
            break;
    }

}

/*********************************************************************
 * @fn      motorcontrol_rxMsgCb
 *
 * @brief   When the motor controller sends the feedback message back, it reaches here
 *
 * @param   rxMsg - The memory message to the received message, the size of the message is the second index rxMsg[1]
 *          motorID - The motor ID described in STM32MCP
 *          frameID - The frame ID described in STM32MCP
 *
 * @return  None.
 */
static void motorcontrol_rxMsgCb(uint8_t *rxMsg, STM32MCP_txMsgNode_t *STM32MCP_txMsgNode)
{
    uint8_t frameID = STM32MCP_txMsgNode->txMsg[0] & 0x1F;

    uint8_t rxPayloadLength = rxMsg[1];
    uint8_t *rxPayload = (uint8_t*)malloc(sizeof(uint8_t)*rxPayloadLength);
    memcpy(rxPayload, rxMsg + 2, rxPayloadLength);

    uint8_t txPayloadLength = STM32MCP_txMsgNode->size;
    uint8_t *txPayload = (uint8_t*)malloc(sizeof(uint8_t)*txPayloadLength);
    memcpy(txPayload, STM32MCP_txMsgNode->txMsg + 2, txPayloadLength);

    switch(frameID)
    {
    case STM32MCP_SET_REGISTER_FRAME_ID:
        break;
    case STM32MCP_GET_REGISTER_FRAME_ID:
        motorcontrol_processGetRegisterFrameMsg(txPayload, txPayloadLength, rxPayload, rxPayloadLength);
        break;
    case STM32MCP_EXECUTE_COMMAND_FRAME_ID:
        break;
    case STM32MCP_GET_BOARD_INFO_FRAME_ID:
        break;
    case STM32MCP_EXEC_RAMP_FRAME_ID:
        break;
    case STM32MCP_GET_REVUP_DATA_FRAME_ID:
        break;
    case STM32MCP_SET_REVUP_DATA_FRAME_ID:
        break;
    case STM32MCP_SET_CURRENT_REFERENCES_FRAME_ID:
        break;
    default:
        break;
    }
    free(rxPayload);
    free(txPayload);
}

/*********************************************************************
 * @fn      motorcontrol_exMsgCb
 *
 * @brief   When the motor controller sends the exception back, it reaches here
 *
 * @param   exceptionCode - The exception code described in STM32MCP
 *
 * @return  None.
 */
static void motorcontrol_exMsgCb(uint8_t exceptionCode)
{
    switch(exceptionCode)
        {
        case STM32MCP_QUEUE_OVERLOAD:
            break;
        case STM32MCP_EXCEED_MAXIMUM_RETRANSMISSION_ALLOWANCE:
            break;
        default:
            break;
        }
}

/*********************************************************************
 * @fn      motorcontrol_erMsgCb
 *
 * @brief   When the motor controller sends the error back, it reaches here
 *
 * @param   exceptionCode - The error code described in STM32MCP
 *
 * @return  None.
 */
static void motorcontrol_erMsgCb(uint8_t errorCode)
{
    switch(errorCode)
    {
    case STM32MCP_BAD_FRAME_ID:
        break;
    case STM32MCP_WRITE_ON_READ_ONLY:
        break;
    case STM32MCP_READ_NOT_ALLOWED:
        break;
    case STM32MCP_BAD_TARGET_DRIVE:
        break;
    case STM32MCP_OUT_OF_RANGE:
        break;
    case STM32MCP_BAD_COMMAND_ID:
        break;
    case STM32MCP_OVERRUN_ERROR:
        break;
    case STM32MCP_TIMEOUT_ERROR:
        break;
    case STM32MCP_BAD_CRC:
        break;
    default:
        break;
    }
}

/*********************************************************************
 * @fn      motorcontrol_brakeAndThrottleCB
 *
 * @brief   When the brake and throttle completed the adc conversion, it sends message here to communicate with STM32
 *
 * @param   throttlePercent - how much speed or torque in percentage the escooter should reach
 *          errorMsg - The error Msg
 *
 * @return  None.
 */
uint16_t execute_rpm;
//static void motorcontrol_brakeAndThrottleCB(uint16_t allowableSpeed, uint16_t IQValue, uint8_t errorMsg)
static void motorcontrol_brakeAndThrottleCB(uint16_t allowableSpeed, uint16_t IQValue, uint8_t errorMsg)
{
    if((errorMsg == BRAKE_AND_THROTTLE_NORMAL))
    {
        //uint16_t
        //execute_rpm = (uint16_t) (allowableSpeed * throttlePercent / 100) & 0xFFFF;
        STM32MCP_setDynamicCurrent(allowableSpeed,IQValue); //Torque Mode + Dynamic Current
        //STM32MCP_executeRampFrame(STM32MCP_MOTOR_1_ID, execute_rpm, 200);
        //STM32MCP_executeCommandFrame(STM32MCP_MOTOR_1_ID, STM32MCP_START_MOTOR_COMMAND_ID);
    }
    else
    {
        /*Brake and Throttle Error -> Stop the motor !!!*/
        STM32MCP_executeCommandFrame(STM32MCP_MOTOR_1_ID, STM32MCP_STOP_MOTOR_COMMAND_ID);
    }

}

/*********************************************************************
 * @fn      motorcontrol_speedModeChgCB
 *
 * @brief   When there is a speed mode change, it sends message here to communicate with STM32
 *
 * @param   speed mode parameters
 *
 *
 * @return  None.
 */
uint8_t motorcontrol_speedModeChgCount = 0;     // for debugging only
void motorcontrol_speedModeChgCB(uint16_t torqueIQ, uint16_t allowableSpeed, uint16_t rampRate)
{
    motorcontrol_speedModeChgCount++;           // for debugging only
    // send speed mode change parameters to motor control

    //STM32MCP_executeRampFrame(STM32MCP_MOTOR_1_ID, rpm, 200);
    //STM32MCP_executeCommandFrame(STM32MCP_MOTOR_1_ID, STM32MCP_START_MOTOR_COMMAND_ID);
    //STM32MCP_setTorqueRampConfiguration(15750,663,2000);
    //STM32MCP_setTorqueRampConfiguration(11450,480,3000);
    //STM32MCP_setTorqueRampConfiguration(6500,270,4000);
    STM32MCP_setTorqueRampConfiguration(torqueIQ, allowableSpeed, rampRate);
}

/*********************************************************************
 * @fn      motorcontrol_controllerCB
 *
 * @brief   When the motor controller sends the error back, it reaches here
 *
 * @param   exceptionCode - The error code described in STM32MCP
 *
 * @return  None.
 */
static void motorcontrol_controllerCB(uint8_t paramID)
{
    switch(paramID)
    {
        case CONTROLLER_TOTAL_DISTANCE_TRAVELLED:
            {
                //Controller_GetParameter(CONTROLLER_TOTAL_DISTANCE_TRAVELLED, &Total_Distance_Travelled);
                break;
            }
        default:
            break;
    }
}
/*********************************************************************
 * @fn      motorcontrol_dashboardCB
 *
 * @brief   When the client (The mobile app) writes the Dashboard GATT server
 *          It will post the paramID of the changed characteristic, and Set the gatt due to client Callback
 *
 *          G-Link v1 allows the following characteristics to be changed from the client (mobile app)
 *          - light mode (paramID 3)
 *
 * @param   paramID: the paramID of the characteristics
 *
 * @return  TRUE or FALSE
 */
static void motorcontrol_dashboardCB(uint8_t paramID)
{
    switch(paramID)
    {
    case DASHBOARD_LIGHT_MODE: // whenever user tabs the light mode icon on the mobile app, motorcontrol_dashboardCB will toggle the light mode by calling lightControl_change()
        {
            lightControl_lightModeChange();
            break;
        }
    default:
        break;
    }
}

/*********************************************************************
 * @fn      motorcontrol_singleButtonCB
 * @brief   Set the gatt due to singleButton Callback
 *
 * @param   serviceUUID - the service UUID
 *          characteristics - the characteristics (Not UUID, just the index)
 *          payload - just the payload
 *
 * @return  TRUE or FALSE
 */
//uint8_t advertEnable = FALSE;
uint8_t messageid;              // for debugging only
uint8_t powerOn = 0;            // How does power on and power off work?

static void motorcontrol_singleButtonCB(uint8_t messageID)
{
    messageid = messageID;  // for debugging only

    switch(messageID)
    {
    case SINGLE_BUTTON_SINGLE_LONG_PRESS_MSG:       // case = 0x01 - toggle Device Power ON / OFF
        {// toggle Power ON/OFF or Enter/Exit Sleep Mode
            // if Powering On -> switch to Power Off
            if(powerOn == 1){
                powerOn = 0;
                //  turn off ADC
                //  turn off I2C
                //  turn off TSL2561
                //  turn off all tasks
                // At very last:
                //  call nvs write to write into NVS memory only when power off - will take a few milliseconds to complete
                //  nvsControl_nvsWrite();
            }
            // if Powering Off -> switch to Power On
            else if(powerOn == 0){
                powerOn = 1;
                //  call nvs read to read NVS memory - will take a few milliseconds to complete
                //  nvsControl_nvsRead();
                //  close NVS
                //  initialize ADC and turn On ADC
                //  initialize I2C
                //  initialize TSL2561
                //  turn ON tasks
            }
        // ICallPlatform_pwrNotify(unsigned int eventType, uintptr_t eventArg, uintptr_t clientArg)
            break;
        }
    case SINGLE_BUTTON_SINGLE_SHORT_PRESS_MSG:      // case = 0x02 - toggle light modes
        {
            lightControl_lightModeChange();
            break;
        }
    case SINGLE_BUTTON_SINGLE_SHORT_LONG_PRESS_MSG: // case = 0x03 - ADVERT_ENABLE if BLE in waiting state or waiting after timeout state
        {
            if (motorControl_getGAPRole_taskCreate_flag == 1)
            {
                gaprole_States_t get_gaproleState;
                GAPRole_GetParameter(GAPROLE_STATE, &get_gaproleState);
                if((get_gaproleState == GAPROLE_WAITING) || (get_gaproleState == GAPROLE_WAITING_AFTER_TIMEOUT) || (get_gaproleState == GAPROLE_STARTED))
                    {
                        uint8_t advertising = true;
                        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advertising);
                    }
            }
            break;
        }
    case SINGLE_BUTTON_DOUBLE_SHORT_PRESS_MSG:      // case = 0x04 - toggle speed modes
        {
            brakeAndThrottle_toggleSpeedMode();
            break;
        }
    case SINGLE_BUTTON_TREBLE_SHORT_PRESS_MSG:      // case = 0x05 - toggle units
        {
            uint8_t UnitSelectDash = dataAnalysis_getUnitSelectDash();
            if (UnitSelectDash == IMP_UNIT) {
                UnitSelectDash = SI_UNIT;
            }
            else if (UnitSelectDash == SI_UNIT) {
                UnitSelectDash = IMP_UNIT;
            }
            dataAnalysis_changeUnitSelectDash(UnitSelectDash);
            break;
        }
    default:                                        // case 0x00 and all other cases
        break;
    }
}
/*********************************************************************
 * @fn      motorcontrol_setGatt
 * @brief   Set the gatt
 *
 * @param   serviceUUID - the service UUID
 *          characteristics - the characteristics (Not UUID, just the index)
 *          payloadLength - the length of the payload
 *          payload - just the payload
 *
 * @return  TRUE or FALSE <--- Nil (void)???
 */
//static void motorcontrol_setGatt(uint16_t serviceUUID, uint8_t characteristics, uint8_t payloadLength, uint8_t* payload)
void motorcontrol_setGatt(uint16_t serviceUUID, uint8_t charteristics, uint8_t payloadLength, uint8_t* payload)
{
    uint8_t *theMessage = ICall_malloc(sizeof(uint8_t)*(payloadLength + 4));
    if (theMessage)
    {
        theMessage[0] = (serviceUUID >> 0) & 0xFF;
        theMessage[1] = (serviceUUID >> 8) & 0xFF;
        theMessage[2] = charteristics;
        theMessage[3] = payloadLength;
        memcpy(theMessage + 4, payload, payloadLength);
        motorcontrol_bleCBs->enqueueMsg(SBP_MC_GATT_EVT, 0, theMessage);
    }
}
