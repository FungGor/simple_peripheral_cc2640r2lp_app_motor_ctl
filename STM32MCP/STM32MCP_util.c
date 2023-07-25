/******************************************************************************

 @file  STM32MCP.c

 @brief This file contains the Code of motor control interfacing


 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include <ti/display/Display.h>
#include "STM32MCP\STM32MCP.h"
#include <stdlib.h>
#include "OSAL.h"

/**********************************************************************
 *  Local functions
 */
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
uint8_t STM32MCP_calChecksum(uint8_t *msg, uint8_t size)
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

