/*
 * TSL2561.h
 *
 *  Created on: 29 May 2023
 *      Author: User
 */

#ifndef TSL2561_TSL2561_H_
#define TSL2561_TSL2561_H_


#ifdef _cplusplus
extern "C"
{
#endif
//
/*********************************************************************
 * INCLUDES
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ti/drivers/I2C.h>

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
* CONSTANTS
*/
/*
 *  ======== TSL2561 Registers ========
 */
#define TSL2561_GND_ADDR        0x29;
#define TSL2561_FLOAT_ADDR      0x39;
#define TSL2561_VDD_ADDR        0x49;

#define LUXNUMSAMPLES           2
#define LUXTHRESHOLD            500

#define TSL2561_COMMAND_REG     0x80
#define TSL2561_TIMING_REG      0x81
#define TSL2561_POWER_ON        0x03
#define TSL2561_POWER_OFF       0x00
#define READCHANNEL0            0xAC
#define READCHANNEL1            0xAE

#define CS_TYPE                 1
#define T_FN_CL_TYPE            0

#define GAINSCALE01             1
#define GAINSCALE16             0

#define INTEGRATIONTIME013      0
#define INTEGRATIONTIME101      1
#define INTEGRATIONTIME402      2

#define LUX_SCALE               14      // scale by 2^14
#define RATIO_SCALE             9       // scale ratio by 2^9
/*********************************************************************
// Integration time scaling factors
*********************************************************************/
#define CH_SCALE                10      // scale channel values by 2^10
#define TINT0                   0       // 13.7ms
#define TINT1                   1       // 101 ms
#define CHSCALE_TINT0           0x7517  // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1           0x0fe7  // 322/81 * 2^CH_SCALE
/*********************************************************************
 * T, FN, and CL Package coefficients
 * For Ch1/Ch0=0.00 to 0.50
 * Lux/Ch0=0.0304-0.062*((Ch1/Ch0)^1.4)
 * piecewise approximation
 * For Ch1/Ch0=0.00 to 0.125:
 * Lux/Ch0=0.0304-0.0272*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.125 to 0.250:
 * Lux/Ch0=0.0325-0.0440*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.250 to 0.375:
 * Lux/Ch0=0.0351-0.0544*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.375 to 0.50:
 * Lux/Ch0=0.0381-0.0624*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.50 to 0.61:
 * Lux/Ch0=0.0224-0.031*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.61 to 0.80:
 * Lux/Ch0=0.0128-0.0153*(Ch1/Ch0)
 *
 * For Ch1/Ch0=0.80 to 1.30:
 * Lux/Ch0=0.00146-0.00112*(Ch1/Ch0)
 *
 * For Ch1/Ch0>1.3:
 * Lux/Ch0=0
*********************************************************************/
#define K1T         0x0040 // 0.125 * 2^RATIO_SCALE
#define B1T         0x01f2 // 0.0304 * 2^LUX_SCALE
#define M1T         0x01be // 0.0272 * 2^LUX_SCALE

#define K2T         0x0080 // 0.250 * 2^RATIO_SCALE
#define B2T         0x0214 // 0.0325 * 2^LUX_SCALE
#define M2T         0x02d1 // 0.0440 * 2^LUX_SCALE

#define K3T         0x00c0 // 0.375 * 2^RATIO_SCALE
#define B3T         0x023f // 0.0351 * 2^LUX_SCALE
#define M3T         0x037b // 0.0544 * 2^LUX_SCALE

#define K4T         0x0100 // 0.50 * 2^RATIO_SCALE
#define B4T         0x0270 // 0.0381 * 2^LUX_SCALE
#define M4T         0x03fe // 0.0624 * 2^LUX_SCALE

#define K5T         0x0138 // 0.61 * 2^RATIO_SCALE
#define B5T         0x016f // 0.0224 * 2^LUX_SCALE
#define M5T         0x01fc // 0.0310 * 2^LUX_SCALE

#define K6T         0x019a // 0.80 * 2^RATIO_SCALE
#define B6T         0x00d2 // 0.0128 * 2^LUX_SCALE
#define M6T         0x00fb // 0.0153 * 2^LUX_SCALE

#define K7T         0x029a // 1.3 * 2^RATIO_SCALE
#define B7T         0x0018 // 0.00146 * 2^LUX_SCALE
#define M7T         0x0012 // 0.00112 * 2^LUX_SCALE

#define K8T         0x029a // 1.3 * 2^RATIO_SCALE
#define B8T         0x0000 // 0.000 * 2^LUX_SCALE
#define M8T         0x0000 // 0.000 * 2^LUX_SCALE
/*********************************************************************
// CS package coefficients
 * For 0 <= Ch1/Ch0 <= 0.52
 *  Lux/Ch0 = 0.0315-0.0593*((Ch1/Ch0)^1.4)
 *              Or
 *  piecewise approximation
 *      For 0 <= Ch1/Ch0 <= 0.13
 *          Lux/Ch0 = 0.0315-0.0262*(Ch1/Ch0)
 *      For 0.13 <= Ch1/Ch0 <= 0.26
 *          Lux/Ch0 = 0.0337-0.0430*(Ch1/Ch0)
 *      For 0.26 <= Ch1/Ch0 <= 0.39
 *          Lux/Ch0 = 0.0363-0.0529*(Ch1/Ch0)
 *      For 0.39 <= Ch1/Ch0 <= 0.52
 *          Lux/Ch0 = 0.0392-0.0605*(Ch1/Ch0)
 * For 0.52 < Ch1/Ch0 <= 0.65
 * Lux/Ch0 = 0.0229-0.0291*(Ch1/Ch0)
 * For 0.65 < Ch1/Ch0 <= 0.80
 * Lux/Ch0 = 0.00157-0.00180*(Ch1/Ch0)
 * For 0.80 < Ch1/Ch0 <= 1.30
 * Lux/Ch0 = 0.00338-0.00260*(Ch1/Ch0)
 * For Ch1/Ch0 > 1.30
 * Lux = 0
*********************************************************************/
#define K1C         0x0043 // 0.130 * 2^RATIO_SCALE
#define B1C         0x0204 // 0.0315 * 2^LUX_SCALE
#define M1C         0x01ad // 0.0262 * 2^LUX_SCALE

#define K2C         0x0085 // 0.260 * 2^RATIO_SCALE
#define B2C         0x0228 // 0.0337 * 2^LUX_SCALE
#define M2C         0x02c1 // 0.0430 * 2^LUX_SCALE

#define K3C         0x00c8 // 0.390 * 2^RATIO_SCALE
#define B3C         0x0253 // 0.0363 * 2^LUX_SCALE
#define M3C         0x0363 // 0.0529 * 2^LUX_SCALE

#define K4C         0x010a // 0.520 * 2^RATIO_SCALE
#define B4C         0x0282 // 0.0392 * 2^LUX_SCALE
#define M4C         0x03df // 0.0605 * 2^LUX_SCALE

#define K5C         0x014d // 0.65 * 2^RATIO_SCALE
#define B5C         0x0177 // 0.0229 * 2^LUX_SCALE
#define M5C         0x01dd // 0.0291 * 2^LUX_SCALE

#define K6C         0x019a // 0.80 * 2^RATIO_SCALE
#define B6C         0x0101 // 0.0157 * 2^LUX_SCALE
#define M6C         0x0127 // 0.0180 * 2^LUX_SCALE

#define K7C         0x029a // 1.3 * 2^RATIO_SCALE
#define B7C         0x0037 // 0.00338 * 2^LUX_SCALE
#define M7C         0x002b // 0.00260 * 2^LUX_SCALE

#define K8C         0x029a // 1.3 * 2^RATIO_SCALE
#define B8C         0x0000 // 0.000 * 2^LUX_SCALE
#define M8C         0x0000 // 0.000 * 2^LUX_SCALE

/*********************************************************************
 * @Structure TSL2561_ALSManager_t
 *
 * @brief     It defines a set of function pointer that the the library can access and control the device peripheral to manipulate the ALS
 *
 * @data      ALS_open: Called when the application wants to open the ALS
 *            ALS_close: Called when the application wants to close the ALS
 *            ALS_transfer: Called when the application wants to transfer data to the ALS
 */

typedef void (*ALS_open)(void);
typedef uint8_t (*ALS_transfer)(uint_least8_t slave_address, void *writeBuffer, size_t writeSize, void *readBuffer, size_t readSize);
typedef void (*ALS_close)(void);

typedef struct
{
    ALS_open            ALS_open;
    ALS_transfer        ALS_transfer;
    ALS_close           ALS_close;
}TSL2561_ALSManager_t;

/*********************************************************************
 * API FUNCTIONS
 */
extern void TSL2561_registerALS( TSL2561_ALSManager_t *ALSI2C );

/*********************************************************************
 *  Global Function declaration
 *
**********************************************************************/
extern void TSL2561_init( void );
extern void TSL2561_powerOn( void );
extern void TSL2561_initTimingReg( void );
extern void TSL2561_setTimingRegister( void );
extern void TSL2561_readChannel(uint8_t readChannel);
extern uint32_t TSL2561_lux( void );

#ifdef _cplusplus
}
#endif


#endif /* TSL2561_TSL2561_H_ */
