/*******************************************************************
 * UDHAL_NVSINT.c
 *
 *  Created on: 21 Mar 2023
 *      Author: User
 *******************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ti/drivers/NVS.h>
#include <ti/drivers/nvs/NVSCC26XX.h>
#include "Board.h"
#include "UDHAL_NVSINT.h"
#include "Application/dataAnalysis.h"

NVS_Handle nvsHandle;
NVS_Attrs regionAttrs;
NVS_Params nvsParams;

uint8_t nvsOpenStatus = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void UDHAL_NVSINT_open(void);
static void UDHAL_NVSINT_erase(void);
static void UDHAL_NVSINT_write(size_t nvsOffset, void *ptrwriteBuffer, size_t writeBufferSize);
static void UDHAL_NVSINT_read(size_t nvsOffset, void *ptrreadBuffer, size_t readBufferSize);
static void UDHAL_NVSINT_close(void);

/*********************************************************************
 * Marco
 */
///*
static dataAnalysis_NVS_Manager_t nvsManager =
{
     UDHAL_NVSINT_open,
     UDHAL_NVSINT_erase,
     UDHAL_NVSINT_write,
     UDHAL_NVSINT_read,
     UDHAL_NVSINT_close
};

//*/
/*********************************************************************
 * @fn      UDHAL_NVSINT_init
 *
 * @brief   It is used to initialize nvsinternal
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_NVSINT_init()
{
    nvsOpenStatus = 1;
    NVS_init();
    //dataAnalysis_registerNVSINT(&nvsManager);
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_params_init
 *
 * @brief   It is used to initialize nvsinternal params
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_NVSINT_params_init()
{
   // NVS_Params_init(&nvsParams);
   // UDHAL_NVSINT_open();
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_open
 *
 * @brief   It is used to open nvsinternal
 *
 * @param   ptrnvsParams
 *
 * @return  None
 */

void UDHAL_NVSINT_open()
{
    nvsHandle = NVS_open(Board_NVSINTERNAL, &nvsParams);
    if (nvsHandle == NULL) {
        nvsOpenStatus = 0;
    }
    else {
        nvsOpenStatus = 1;
    }
    // Populate a NVS_Attrs structure with properties specific
    // to a NVS_Handle such as region base address, region size,
    // and sector size.
    NVS_getAttrs(nvsHandle, &regionAttrs);
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_erase
 *
 * @brief   It is used to erase nvsinternal sector
 *
 * @param   None
 *
 * @return  None
 */

void UDHAL_NVSINT_erase()
{
    if (nvsOpenStatus == 1)
    {
        // Erase the entire flash sector - Erase sets all bits to 1.
        NVS_erase(nvsHandle, 0, regionAttrs.sectorSize);
    }
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_write
 *
 * @brief   It is used to write to nvsinternal
 *
 * @param   None
 *
 * @return  None
 */

void UDHAL_NVSINT_write(size_t nvsOffset, void *ptrwriteBuffer, size_t writeBufferSize)
{
    if (nvsOpenStatus == 1)
    {
        NVS_write(nvsHandle, nvsOffset, ptrwriteBuffer, writeBufferSize,
                        NVS_WRITE_ERASE | NVS_WRITE_POST_VERIFY);
    }
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_read
 *
 * @brief   It is used to read from nvsinternal
 *
 * @param   None
 *
 * @return  None
 */

void UDHAL_NVSINT_read(size_t nvsOffset, void *ptrreadBuffer, size_t readBufferSize)
{
    if (nvsOpenStatus == 1)
    {
        NVS_read(nvsHandle, nvsOffset, ptrreadBuffer, readBufferSize);
    }
}

/*********************************************************************
 * @fn      UDHAL_NVSINT_close
 *
 * @brief   It is used to close nvsinternal nvsHandle
 *
 * @param   None
 *
 * @return  None
 */
void UDHAL_NVSINT_close()
{
    NVS_close(nvsHandle);
}

