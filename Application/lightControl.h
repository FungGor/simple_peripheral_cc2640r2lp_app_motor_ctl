/**********************************************************************************************
 * Filename:       lightControl.h
 *
 * Description:    This file contains the Dashboard service definitions and
 *                 prototypes.
 *
 *************************************************************************************************/
//
#ifndef LIGHTCONTROL_H_
#define LIGHTCONTROL_H_
//
#ifdef __cplusplus
extern "C"
{
#endif
//
/*********************************************************************
 * INCLUDES
 */
#include<stdio.h>
#include<stdint.h>
#include <ti/drivers/GPIO.h>
#include <string.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
* CONSTANTS
*/
#define LUX_THRESHOLD                                   500  // light shall be ON when light intensity is consistently below this lux value
#define ALS_NUMSAMPLES                                  2    // The number of samples used for light intensity evaluation --> must be 8
#define NUM_PERIODIC_COMMUNICATION_HF_2_ALS             2    // in milliseconds, the time between ALS sampling = PERIODIC_COMMUNICATION_HF_SAMPLING_TIME x N_ALS, i.e. ALS_SAMPLING_TIME

// there is actually no need to sample light level too frequently.
// Perhaps frequency of 1 second is sufficient.
// lightControl timer is timer 8
// For example, total time interval between AUTO light ON/OFF is (ALS_NUMSPLES - 1) * ALS_SAMPLING_TIME => (8-1) * 250 = 1750 milliseconds
// For example, total time interval between AUTO light ON/OFF is (ALS_NUMSPLES - 1) * ALS_SAMPLING_TIME => (8-1) * 300 = 2100 milliseconds
// For example, total time interval between AUTO light ON/OFF is (ALS_NUMSPLES - 1) * ALS_SAMPLING_TIME => (3-1) * 1000 = 2000 milliseconds
#define LIGHT_MODE_OFF                                  0x00
#define LIGHT_MODE_ON                                   0x01
#define LIGHT_MODE_AUTO                                 0x02
#define LIGHT_MODE_INITIAL                              LIGHT_MODE_AUTO

#define LIGHT_STATUS_OFF                                0x00
#define LIGHT_STATUS_ON                                 0x01
#define LIGHT_STATUS_INITIAL                            LIGHT_STATUS_OFF

#define LIGHTCONTROL_TASK_PRIORITY                      4

#ifndef LIGHTCONTROL_TASK_STACK_SIZE
#define LIGHTCONTROL_TASK_STACK_SIZE                    360
#endif

//ALS is an abbreviation for Ambient Light Sensor
/* ********************************************************************
 * TYPEDEFS
*/


/* ********************************************************************
* MACROS
*/

/* ********************************************************************
 * API FUNCTIONS
 */
extern void lightControl_init( uint8_t i2cOpenStatus );
extern void lightControl_taskFxn();
//static void lightControl_taskFxn(UArg a0, UArg a1);
/* Task creation function for the Simple Peripheral.
*/
//extern void GPtimer_createTask(void);

extern void lightControl_measureLux( void );
extern void light_MODE_AUTO( void );
static void light_MODE_OFF( void );
static void light_MODE_ON( void );
extern void lightControl_ALS_Controller( void );
extern void lightControl_lightModeChange( void );
extern void lightControl_motorControl_lightStatusChg( void );
extern uint8_t lightControl_getLightMode( void );
extern uint8_t lightControl_getLightStatus( void );

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LIGHTCONTROL_H_ */
