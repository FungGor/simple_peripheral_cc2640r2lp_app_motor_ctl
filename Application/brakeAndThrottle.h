/*
 * brake.h
 *
 *  Created on: 14 Feb 2022
 *      Author: harrysiu
 */

#ifndef APPLICATION_BRAKEANDTHROTTLE_H_
#define APPLICATION_BRAKEANDTHROTTLE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <math.h>
/*********************************************************************
*  EXTERNAL VARIABLES
*/
/*********************************************************************
 * CONSTANTS
 */
#define BRAKE_AND_THROTTLE_ADC_SAMPLING_PERIOD                    100
#define BRAKE_AND_THROTTLE_SAMPLES                                8

//Speed modes
#define BRAKE_AND_THROTTLE_SPEED_MODE_AMBLE                       0x00
#define BRAKE_AND_THROTTLE_SPEED_MODE_LEISURE                     0x01
#define BRAKE_AND_THROTTLE_SPEED_MODE_SPORTS                      0x02

#define BRAKE_AND_THROTTLE_MAXIMUMN_SPEED                         663       // 663 RPM = 25.4 Km/hr

//Speed mode reduction ratio    <--- what is the purpose and logic of reduction ratio???? Is it output power reduction???
#define BRAKE_AND_THROTTLE_SPEED_MODE_REDUCTION_RATIO_AMBLE       41        //64%   Pout = 216.75 W
#define BRAKE_AND_THROTTLE_SPEED_MODE_REDUCTION_RATIO_LEISURE     73        //80%   Pout = 240 W
#define BRAKE_AND_THROTTLE_SPEED_MODE_REDUCTION_RATIO_SPORTS      100       //100%  Pout = 300 W

//Speed mode maximum "powered" speed in RPM
#define BRAKE_AND_THROTTLE_MAXSPEED_AMBLE                         270       // 270 RPM = 10.4 Km/hr
#define BRAKE_AND_THROTTLE_MAXSPEED_LEISURE                       480       // 480 RPM = 18.4 Km/hr
#define BRAKE_AND_THROTTLE_MAXSPEED_SPORTS                        663       // 663 RPM = 25.4 Km/hr

//Speed mode ramp rate (acceleration) in milliseconds
#define BRAKE_AND_THROTTLE_RAMPRATE_AMBLE                         4000
#define BRAKE_AND_THROTTLE_RAMPRATE_LEISURE                       3000
#define BRAKE_AND_THROTTLE_RAMPRATE_SPORTS                        2000

//Speed mode Torque IQ value
#define BRAKE_AND_THROTTLE_TORQUEIQ_AMBLE                         6500     // IQ 10125  = 9.0 Amp
#define BRAKE_AND_THROTTLE_TORQUEIQ_LEISURE                       11450     // IQ 12600 = 11.2 Amp
#define BRAKE_AND_THROTTLE_TORQUEIQ_SPORTS                        15750     // IQ 15750 = 14.0 Amp
#define BRAKE_AND_THROTTLE_TORQUEIQ_MAX                           15750     // IQ 15750 = 14.0 Amp

//Hard braking definition   (What is Hard Braking? why is this necessary?)
#define HARD_BRAKING_THROTTLE_PERCENTAGE                          5
#define HARD_BRAKING_BRAKE_PERCENTAGE                             5
#define BRAKEPERCENTTHRESHOLD                                     5
#define THROTTLEPERCENTREDUCTION                                  0.7
//Throttle calibration values = value range the throttle ADC is conditioned to be within
#define THROTTLE_ADC_CALIBRATE_H                                  2350
#define THROTTLE_ADC_CALIBRATE_L                                  850

//Throttle error thresholds = values that should not be possible under nominal operation
#define THROTTLE_ADC_THRESHOLD_H                                  2500
#define THROTTLE_ADC_THRESHOLD_L                                  750

//Brake calibration values = value range the Brake ADC is conditioned to be within
#define BRAKE_ADC_CALIBRATE_H                                     2350
#define BRAKE_ADC_CALIBRATE_L                                     850

//Brake error thresholds = values that should not be possible under nominal operation
#define BRAKE_ADC_THRESHOLD_H                                     2500
#define BRAKE_ADC_THRESHOLD_L                                     750

//Error message
#define BRAKE_AND_THROTTLE_NORMAL                                 0x00
#define BRAKE_ERROR                                               0x01
#define THROTTLE_ERROR                                            0x02
#define HARD_BRAKING_ERROR                                        0x04

/*********************************************************************
 * MACROS
 */
typedef void (*brakeAndThrottle_timerStart)(void);
typedef void (*brakeAndThrottle_timerStop)(void);
typedef struct
{
    brakeAndThrottle_timerStart timerStart;
    brakeAndThrottle_timerStop  timerStop;
}brakeAndThrottle_timerManager_t;

typedef void (*brakeAndThrottle_ADC_Open)(void);
typedef void (*brakeAndThrottle_ADC_Convert)(uint16_t*);
typedef void (*brakeAndThrottle_ADC_Close)(void);
typedef struct
{
    brakeAndThrottle_ADC_Open       brakeAndThrottle_ADC_Open;
    brakeAndThrottle_ADC_Convert    brakeAndThrottle_ADC_Convert;
    brakeAndThrottle_ADC_Close      brakeAndThrottle_ADC_Close;
}brakeAndThrottle_adcManager_t;

typedef void (*brakeAndThrottle_CB_t)(uint16_t, uint16_t, uint8_t);
typedef struct
{
    brakeAndThrottle_CB_t       brakeAndThrottle_CB;
}brakeAndThrottle_CBs_t;

/*********************************************************************
 * FUNCTIONS
 */
/*
 * Task creation function for the Simple Peripheral.
 */
extern void brakeAndThrottle_init();
extern void brakeAndThrottle_start();
extern void brakeAndThrottle_stop();
extern void brakeAndThrottle_toggle();
extern void brakeAndThrottle_setSpeedMode(uint8_t speedMode);
extern uint8_t brakeAndThrottle_getSpeedMode();
extern void brakeAndThrottle_toggleSpeedMode();
extern void brakeAndThrottle_registerCBs(brakeAndThrottle_CBs_t *obj);

extern void brakeAndThrottle_registerTimer(brakeAndThrottle_timerManager_t *obj);
extern void brakeAndThrottle_registerADC1(brakeAndThrottle_adcManager_t *obj);
extern void brakeAndThrottle_registerADC2(brakeAndThrottle_adcManager_t *obj);
extern void brakeAndThrottle_ADC_conversion();
extern uint16_t brakeAndThrottle_getThrottlePercent();
extern uint16_t brakeAndThrottle_getBrakePercent();
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_BRAKEANDTHROTTLE_H_ */
