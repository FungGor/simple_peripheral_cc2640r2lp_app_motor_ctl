/*
 * @file   UDHAL_I2C.c
 * @brief This file contain the functions about
 *  Created on: 11 Mar 2023
 *      Author: User
 */
/*********************************************************************
 * INCLUDES
 */
#include "UDHAL/UDHAL_I2C.h"
#include "Board.h"
#include "Application/lightControl.h"
#include "TSL2561/TSL2561.h"
#include "Application/ledControl.h"

/*********************************************************************
 * LOCAL VARIABLES
 */
/*********************************************************************
 * LOCAL FUNCTIONS
 */
I2C_Handle       i2cHandle;
I2C_Params       i2cParams;
I2C_Transaction  i2cTransaction = {0};

uint8_t          i2cOpenStatus = 0;                   // i2cOpenStatus must be set to 0 at declaration.  i2cOpenStatus = 1 indicates i2c is opened. i2cOpenStatus = 0 indicates i2c is not opened

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_I2C_open();
static void UDHAL_I2C_close();
static uint8_t UDHAL_I2C_transfer(uint_least8_t slave_address, void *writeBuffer, size_t writeSize, void *readBuffer, size_t readSize);

/*********************************************************************
 * Marco
 */
static TSL2561_ALSManager_t ALS_I2C =
{
    UDHAL_I2C_open,
    UDHAL_I2C_transfer,
    UDHAL_I2C_close
};

/*********************************************************************
 * Marco
 */
static ledControl_ledDisplayManager_t ledDisplay_I2C =
{
    UDHAL_I2C_open,
    UDHAL_I2C_transfer,
    UDHAL_I2C_close
};
/* ********************************************************************
 * @fn:     UDHAL_I2C_init
 *
 * @brief: It is used to initialize I2C
 *
 * Arguments:   None
 *
 * Return:      None
 ******************************************************************** */
extern void UDHAL_I2C_init()
{
    I2C_init();
    TSL2561_registerALS(&ALS_I2C);                      // ALS Light Sensor
    ledControl_registerLedDisplay(&ledDisplay_I2C);     // LED Display
}

/* ********************************************************************
 * @fn:     (*I2C_CallbackFxn)
 *
 * @brief:  I2C_MODE_CALLBACK
 *
 * Arguments:   I2C_Handle handle, I2C_Transaction *transaction, bool transferStatus
 *
 * Return:      None
 ******************************************************************** */
extern void UDHAL_I2C_CallbackFxn(I2C_Handle i2c_Handle, I2C_Transaction *i2c_Transaction, bool i2c_TransferStatus)
{

    if (i2c_TransferStatus == false) {
        //transaction failed
    }
    else
    {
        // Check for a semaphore handle
//        if (msg->arg != NULL) {

            // Perform a semaphore post
//            sem_post((sem_t *) (msg->arg));
//        }

    }

}
/*********************************************************************
 * @fn      UDHAL_I2C_params_init
 *
 * @brief   It is used to initialize the I2C parameters
 *
 * @param   None
 *
 * @return  None
 */
extern void UDHAL_I2C_params_init()
{
    I2C_Params_init(&i2cParams);
//    i2cParams.transferMode = I2C_MODE_CALLBACK;
//    i2cParams.transferCallbackFxn = &UDHAL_I2C_CallbackFxn;
    i2cParams.bitRate = I2C_400kHz;
    if (i2cOpenStatus == 0) // if i2c is not opened, open i2c. i2c can only be opened once.
    {
        UDHAL_I2C_open();
    }
}

/*********************************************************************
 * @fn:     UDHAL_I2C_open
 *
 * @brief: open I2C communication
 *
 * Arguments:   None
 *
 * Return:      None
*********************************************************************/
static void UDHAL_I2C_open()
{
    i2cHandle = I2C_open(Board_I2C0, &i2cParams);

    if (!i2cHandle) //if (i2cHandle == NULL)
    {
        i2cOpenStatus = 0;      // i2c not opened
        //  add error protocol here
        //  Action:  when i2c open fails, need to disable LED display and Light auto mode function
    }
    else {
        i2cOpenStatus = 1;      //i2c opened
    }
}
/*********************************************************************
 * @fn:     UDHAL_I2C_getI2CStatus
 *
 * @brief:  return I2C open status
 *
 * Arguments:   None
 *
 * Return:      None
*********************************************************************/
extern uint8_t UDHAL_I2C_getOpenStatus(void)
{
    return (i2cOpenStatus);
}
/*********************************************************************
 * @fn:     UDHAL_I2C_close
 *
 * @brief: close I2C communication
 *
 * Arguments:   None
 *
 * Return:      None
*********************************************************************/
static void UDHAL_I2C_close()
{
    I2C_close(i2cHandle);
    i2cOpenStatus = 0;      // i2c closed = not opened
}
/*********************************************************************
 * @fn:     UDHAL_I2C_transfer()
 *
 * @brief: I2C transfer
 *
 * Arguments:   Peripheral slave address,
 *              pointer address of write buffer
 *              write size
 *              pointer address of read buffer
 *              read size
 *
 * Return:      None
*********************************************************************/
uint8_t          i2cTransferStatus;         // for debugging
static uint8_t UDHAL_I2C_transfer(uint_least8_t slave_address, void *writeBuffer, size_t writeSize, void *readBuffer, size_t readSize)
{
    //uint8_t          i2cTransferStatus;
    if (i2cOpenStatus == 1)
    {
        i2cTransaction.slaveAddress = slave_address;
        i2cTransaction.writeBuf   = writeBuffer;
        i2cTransaction.writeCount = writeSize;
        i2cTransaction.readBuf    = readBuffer;
        i2cTransaction.readCount  = readSize;

        i2cTransferStatus = I2C_transfer(i2cHandle, &i2cTransaction);
        // I2C_transfer returns true (1) or false (0)
        // when i2cTransferStatus = 1, transfer was successful
        // when i2cTransferStatus = 0, transfer was not successful
        if (!i2cTransferStatus)
        {
            // error handling if necessary
        }
    }
    return i2cTransferStatus;
}
