/******************************************************************************

 @file  UDHAL_UART.c

 @brief This file contain the functions about


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/UART.h>
#include "STM32MCP/STM32MCP.h"
#include "Board.h"
/*********************************************************************
 * LOCAL VARIABLES
 */
static UART_Handle UART_handle;
static UART_Params UART_params;
static uint8_t receivedByte;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_UART_open();
static void UDHAL_UART_read(uint8_t *message, uint8_t size);
static void UDHAL_UART_write(uint8_t *message, uint8_t size);
static void UDHAL_UART_close();
static void UDHAL_readCallback(UART_Handle UART_handle, void *rxBuf, size_t size);
static void UDHAL_writeCallback(UART_Handle UART_handle, void *rxBuf, size_t size);
/*********************************************************************
 * Marco
 */
static STM32MCP_uartManager_t STM32MCP_uartManager =
{
    UDHAL_UART_open,
    UDHAL_UART_read,
    UDHAL_UART_write,
    UDHAL_UART_close
};
/*********************************************************************
 * @fn      UDHAL_UART_init
 *
 * @brief   It is used to initialize the UART
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_UART_init()
{
    UART_init();
    STM32MCP_registerUart(&STM32MCP_uartManager);
}
/*********************************************************************
 * @fn      UDHAL_UART_params_init
 *
 * @brief   It is used to initialize the UART parameters
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_UART_params_init()
{
    UART_Params_init(&UART_params);
    UART_params.baudRate      = 115200;
    UART_params.writeMode     = UART_MODE_CALLBACK;
    UART_params.writeDataMode = UART_DATA_BINARY;
    UART_params.writeCallback = UDHAL_writeCallback;
    UART_params.readMode      = UART_MODE_CALLBACK;
    UART_params.readTimeout   = 1;
    UART_params.readDataMode  = UART_DATA_BINARY;
    UART_params.readCallback  = UDHAL_readCallback;
}
/*********************************************************************
 * @fn      UDHAL_UART_open
 *
 * @brief   It is used to open the UART communication port
 *
 * @param   None
 *
 * @return  None
 */
static void UDHAL_UART_open()
{
    // Open the UART and initiate the first read
    UART_handle = UART_open(Board_UART0, &UART_params);
    UDHAL_UART_read(&receivedByte, 1);
}
/*********************************************************************
 * @fn      UDHAL_UART_write
 *
 * @brief   It is used to write data on UART
 *
 * @param   message: The pointer to the message
 *          size: The size of the message
 *
 * @return  none
 */
static void UDHAL_UART_write(uint8_t *message, uint8_t size)
{
    UART_write(UART_handle, message, size);
}
/*********************************************************************
 * @fn      UDHAL_UART_read
 *
 * @brief   It is used to read data from the UART
 *
 * @param   message: the pointer to the message you want to put
 *          size: The size of read message
 *
 * @return  true if the data is successfully received
 *          false if there is problem
 */
static void UDHAL_UART_read(uint8_t *message, uint8_t size)
{
    UART_read(UART_handle, message, size);
}
/*********************************************************************
 * @fn      UDHAL_UART_close
 *
 * @brief   It is used to terminate UART communication
 *
 * @param   None
 *
 * @return  None
 */
static void UDHAL_UART_close()
{
    UART_writeCancel(UART_handle);
    UART_readCancel(UART_handle);
    UART_close(UART_handle);
}
/*********************************************************************
 * @fn      readCallback
 *
 * @brief   UART read call back, when uart has received a single byte, it will execute this code.
 *          I repeat, readCallback must be configured as byte by byte. Not a number of bytes.
 *          You must add STM32MCP_flowControlHandler to this function
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_readCallback(UART_Handle UART_handle, void *rxBuf, size_t size)
{
      //Pass the received byte to the flow control handler
      STM32MCP_flowControlHandler(((uint8_t *)rxBuf)[0]);
      //Wait the the next received byte
      UDHAL_UART_read(&receivedByte, 1);
}
/*********************************************************************
 * @fn      readCallback
 *
 * @brief   neglect this function, not used but required for TI driver initialization
 *
 * @param   None.
 *
 * @return  None.
 */
static void UDHAL_writeCallback(UART_Handle UART_handle, void *rxBuf, size_t size)
{

}
