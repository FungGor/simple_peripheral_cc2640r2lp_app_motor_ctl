/*
 * TSL2561.c
 *
 *  Created on: 29 May 2023
 *      Author: User
 */
#include "TSL2561/TSL2561.h"

static TSL2561_ALSManager_t *TSL2561_ALSManager;

static uint_least8_t    TSL2561SlaveAddress = TSL2561_VDD_ADDR;
static uint8_t          iType = T_FN_CL_TYPE;
static uint8_t          timingReg = 0x00;
static uint8_t          iGain;
static uint8_t          tInt;
static uint16_t         calibrationFactorx1000;

static uint8_t          TSL2561_i2cTransferStatus;      // initially set to 1

uint16_t                raw_channel0;
uint16_t                raw_channel1;

static uint32_t         TSL2561_calculateLux();

/*********************************************************************
 * Routine:     void TSL2561_init()
 *
 * Description: Initialization of TSL2561 Ambient Light Sensor
 *              set GAIN and time integration values
 *
 * Arguments:   None
 *
 * Return:      None
*********************************************************************/
void TSL2561_init()
{
    /***************** Step 1:  Power On TSL2561: Command Register = 0x80 **********
     *
     *   Control register
     *   Power On = 0x03
     *   Power Off = 0x00
     *
     *              I2C transfer Power on TLS2561 sensor
     *********************************************************************/
    TSL2561_powerOn();

    // make sure i2c transfer is successful
    /********** Step 2:  Set Timing Registration on TLS2561 sensor = 0x81 **********
     */
    if (TSL2561_i2cTransferStatus == 1)
    {
        TSL2561_setTimingRegister();
    }

}

/*******************   Power On TSL2561: Command Register = 0x80 **********************
 *
 *   Control register
 *   Power On = 0x03
 *   Power Off = 0x00
 *
 *********************************************************************/
void TSL2561_powerOn()
{
    /*******************   Power On TSL2561: Command Register = 0x80 **********************
     *   Control register
     *   Power On = 0x03
     *   Power Off = 0x00
     *********************************************************************/
    uint8_t          TSL2561_writeBuffer[2] = {TSL2561_COMMAND_REG, TSL2561_POWER_ON};
    uint8_t          TSL2561_readBuffer = NULL;
    size_t           writeBufferSize = 2;
    size_t           readBufferSize = 0;
    /*********************************************************************
     *              I2C transfer Power on TLS2561 sensor
     *********************************************************************/
    TSL2561_i2cTransferStatus = TSL2561_ALSManager -> ALS_transfer(TSL2561SlaveAddress, &TSL2561_writeBuffer, writeBufferSize, &TSL2561_readBuffer, readBufferSize);
}

/*******************   Set timing register: Command = 0x81 **********************
 *   0x00 = Low gain & TINT = 13.7
 *   0x01 = Low gain & TINT = 101
 *   0x02 = Low gain & TINT = 402
 *   0x03 = Low gain & TINT = manual
 *   0x11 = High gain & TINT = 101
 *********************************************************************/
void TSL2561_setTimingRegister()
{
    // Set Timing Registration on TLS2561 sensor
    uint8_t          TSL2561_writeBuffer[2] = {TSL2561_TIMING_REG, timingReg};
    uint8_t          TSL2561_readBuffer = NULL;
    size_t           writeBufferSize = 2;
    size_t           readBufferSize = 0;
    // I2C transfer Set Timing Registration on TLS2561 sensor
    TSL2561_i2cTransferStatus = TSL2561_ALSManager -> ALS_transfer(TSL2561SlaveAddress, &TSL2561_writeBuffer, writeBufferSize, &TSL2561_readBuffer, readBufferSize);
    TSL2561_initTimingReg();
}

/*******************      Basic Operation       **********************
 * Read words (recommended)
 *   0xAC = raw Channel 0
 *   0xAE = raw Channel 1
 * Read Byte
 *   0x0C = raw Channel 0 Low Data
 *   0x0D = raw Channel 0 High Data
 *   0x0E = raw Channel 1 Low Data
 *   0x0F = raw Channel 1 High Data
 *
 *********************************************************************/
 // Read Channels via Read Word Protocol

void TSL2561_readChannel(uint8_t readChannel)
{
    // make sure i2c transfer is successful
    /************************************************************
     *      Read light sensor value
     ************************************************************/
    uint8_t TSL2561_readBuffer[2] = {0, 0};                       // local variables are not stored in memory. This is equivalent to reset each time this function is called.
    uint8_t TSL2561_writeBuffer[1] = {readChannel};
    size_t  readBufferSize  = 2;
    size_t  writeBufferSize = 1;
    TSL2561_i2cTransferStatus = TSL2561_ALSManager -> ALS_transfer(TSL2561SlaveAddress, &TSL2561_writeBuffer, writeBufferSize, &TSL2561_readBuffer, readBufferSize);
    /*************************************************************************
     *  i2c transfer must be successful to proceed into the following routine
     *************************************************************************/

    if (readChannel == READCHANNEL0){
        raw_channel0 = 256 * TSL2561_readBuffer[1] + TSL2561_readBuffer[0];
    }
    else if (readChannel == READCHANNEL1){
        raw_channel1 = 256 * TSL2561_readBuffer[1] + TSL2561_readBuffer[0];
    }
}
/*********************************************************************
 * Routine:     uint32_t TSL2561_lux()
 *
 * Description: Calculate the sample illuminance (lux)
 *
 * Arguments:   Nil
 *
 * Return:      unsigned integer - the average illuminance (lux)
*********************************************************************/
uint32_t         luxSample = 0x11111111;         // default value = 286331153 (hexidecimal 0x11111111)
uint32_t TSL2561_lux()
{
    luxSample = (uint64_t) calibrationFactorx1000 * TSL2561_calculateLux() / 1000;
    return (luxSample);
}

/*********************************************************************************************
 * Routine:     uint32_t TSL2561_calculateLux()
 *
 * Description: Calculate the approximate illuminance (lux) given the measured raw
 *              channel values of the TSL2560. The equation is implemented as a piece-wise
 *              linear approximation.
 *
 * Arguments:   Nil
 *
 * Return:      unsigned integer - the approximate illuminance (lux)
**********************************************************************************************/
uint64_t channel1;
uint64_t channel0;

uint32_t TSL2561_calculateLux()
{
    // first, scale the channel values depending on the gain and integration time
    // 16X, 402mS is nominal.
    // scale if integration time is NOT 402 msec
    uint64_t chScale = 0;

    // the channel scale is selected depending on the tInt selected
    switch(tInt)
    {
    case TINT0:
        chScale = CHSCALE_TINT0;
        break;
    case TINT1:
        chScale = CHSCALE_TINT1;
        break;
    default:
        chScale = (1 << CH_SCALE);  // 1 shift bit by 10
        break;
    }
    // scale if gain is NOT 16X
    if (!iGain){            //(if iGAIN == 0, scale = 16, else scale = 1)
            chScale = chScale << 4; // scale 1X to 16X
    }
    // shift bits are used throughout to take care/eliminate use of floating point variables
    channel0 = (raw_channel0 * chScale) >> CH_SCALE;
    channel1 = (raw_channel1 * chScale) >> CH_SCALE;

    uint64_t ratio1 = 0;
    // find the ratio of the channel values (Channel 1/Channel 0)
    // protect against divide by zero
    if (channel0 != 0) {ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;}

    uint64_t ratio = 0;
    ratio = (ratio1 + 1) >> 1;
    uint32_t bb = 0 ;
    uint32_t mm = 0;
    switch(iType)
    {
    case T_FN_CL_TYPE:
    {
        if (ratio <= K1C){
            bb = B1T;
            mm = M1T;
        }
        else if (ratio <= K2T){
            bb = B2T;
            mm = M2T;
            }
        else if (ratio <= K3T){
            bb = B3T;
            mm = M3T;
        }
        else if (ratio <= K4T){
            bb = B4T;
            mm = M4T;
        }
        else if (ratio <= K5T){
            bb = B5T;
            mm = M5T;
            }
        else if (ratio <= K6T){
            bb = B6T;
            mm = M6T;
        }
        else if (ratio <= K7T){
            bb = B7T;
            mm = M7T;
        }
        else if (ratio > K8T){
            bb = B8T;
            mm = M8T;
        }
        break;
    }
    case CS_TYPE:
    {
        if (ratio <= K1C){
            bb = B1C;
            mm = M1C;
        }
        else if (ratio <= K2C){
            bb = B2C;
            mm = M2C;
            }
        else if (ratio <= K3C){
            bb = B3C;
            mm = M3C;
            }
        else if (ratio <= K4C){
            bb = B4C;
            mm = M4C;
            }
        else if (ratio <= K5C){
            bb = B5C;
            mm = M5C;
        }
        else if (ratio <= K6C){
            bb = B6C;
            mm = M6C;
        }
        else if (ratio <= K7C){
            bb = B7C;
            mm = M7C;
            }
        else if (ratio > K8C){
            bb = B8C;
            mm = M8C;
            }
        break;
    }
    default:
        break;
    }
    uint64_t temp = 0 ;
    temp = ((channel0 * bb) - (channel1 * mm));

    temp += (1 << (LUX_SCALE - 1));
    uint32_t lux = 0;
    lux = (temp >> LUX_SCALE);

    return(lux);
}

/*******************   Initialize light sensor internal timing register integration parameters **********************
     *
     *   0x00 = Low gain & TINT = 13.7
     *   0x01 = Low gain & TINT = 101
     *   0x02 = Low gain & TINT = 402
     *   0x03 = Low gain & TINT = manual
     *   0x11 = High gain & TINT = 101
     *
*********************************************************************************************************************/
void TSL2561_initTimingReg()
{
    switch(timingReg)
    {
    case 0x00:
        {
            iGain = GAINSCALE01;            // we are only interested in detecting day-light -> scale fixed to 1
            tInt = INTEGRATIONTIME013;      // integration time = 13.7 ms
            calibrationFactorx1000 = 12000;  // calibration factor is based on in-house statistical measurements of TSL2561 vs LX-107 light meter
            break;
        }
    case 0x01:
        {
            iGain = GAINSCALE01;            // we are only interested in detecting day-light -> scale fixed to 1
            tInt = INTEGRATIONTIME101;      // integration time = 101ms
            calibrationFactorx1000 = 12000;  // calibration factor is based on in-house statistical measurements of TSL2561 vs LX-107 light meter
            break;
        }
    case 0x02:
        {
            iGain = GAINSCALE01;            // we are only interested in detecting day-light -> scale fixed to 1
            tInt = INTEGRATIONTIME402;      // integration time = 402ms
            calibrationFactorx1000 = 12000; // calibration factor is based on in-house statistical measurements of TSL2561 vs LX-107 light meter
            break;
        }
    case 0x03:
        {
            break;
        }
    case 0x11:
        {
            iGain = GAINSCALE16;            // This is high gain (16x) we are only interested in detecting day-light -> we should not select this option
            tInt = INTEGRATIONTIME101;      // integration time = 101ms
            calibrationFactorx1000 = 47;    // calibration factor is based on in-house statistical measurements of TSL2561 vs LX-107 light meter
            break;
        }
    default:
        {
            iGain = GAINSCALE01;            // we are only interested in detecting day-light -> scale fixed to 1
            tInt = INTEGRATIONTIME101;      // integration time = 101ms
            calibrationFactorx1000 = 12000; // calibration factor is based on in-house statistical measurements of TSL2561 vs LX-107 light meter
            break;
        }
    }

}

/*********************************************************************
 * @fn      TSL2561_registerALS
 *
 * @brief   It is used to register the I2C function to communicate with the ALS
 *
 * @param   None
 *
 * @return  None
 */
void TSL2561_registerALS( TSL2561_ALSManager_t *ALSI2C )
{
    TSL2561_ALSManager = ALSI2C;
}
