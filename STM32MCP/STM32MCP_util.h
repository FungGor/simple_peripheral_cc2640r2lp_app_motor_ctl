/*
 * STM32MCP_util.h
 *
 *  Created on: 17 Sep 2021
 *      Author: HarryYeungYikSiu
 */

#ifndef STM32MCP_STM32MCP_UTIL_H_
#define STM32MCP_STM32MCP_UTIL_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

uint8_t STM32MCP_calChecksum(uint8_t *txMessage, uint8_t size);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* STM32MCP_STM32MCP_UTIL_H_ */
