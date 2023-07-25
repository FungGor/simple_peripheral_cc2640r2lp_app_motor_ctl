/*
 * IS31FL3236A.h
 *
 * IS31FL3236A 36-CHANNEL LED DRIVER; SELECTABLE PWM FREQUENCY
 *
 *  Created on: 12 Jul 2023
 *      Author: Jermyn & Chee
 */

#ifndef IS31FL3236A_IS31FL3236A_H_
#define IS31FL3236A_IS31FL3236A_H_


#ifdef _cplusplus
extern "C"
{
#endif
//
/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
//#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

/*********************************************************************
 * CONSTANTS
 */
/*********************************************************************
* CONSTANTS
*/
#define IS31FL3236A_ADDR            0x3C
#define IS31FL3236A_RESET_REG       0x4F
#define IS31FL3236A_ON_REG          0x00
#define IS31FL3236A_ENABLE_REG      0x4A
#define IS31FL3236A_UPDATE_REG      0x25
#define IS31FL3236A_PWM_FREQUENCY   0x4B


#define ZERO_ONE_CUSTOM             0x07
#define ZERO_ZERO_CUSTOM            0x00

//PWM Brightness control
#define PWM_CUSTOM                  0x7E //255 STEPS
#define PWM_BRIGHT                  0xFF
#define PWM_ZERO                    0x00


//Brightness 32 Gamma Steps
extern const uint8_t brightness_32[];

//Brightness 64 Gamma Steps
extern const uint8_t brightness_64[];

//Testing all leds
extern const uint8_t led_array[][2];

//LED Current control(affect brightness)
//status_buf: last bit must be 1 to let led on, 6th and 7th bits set output current
#define I_MAX_OUT                    0x01 //MAX current 23mA
#define I_MAX2_OUT                   0x03 //MAX/2
#define I_MAX3_OUT                   0x05 //MAX/3
#define I_MAX4_OUT                   0x07 //MAX/4
#define I_OUT                        0x07

//LED control register address

/* Address for LED Status */
#define IS31FL3236A_LED43_ADDR       0x26
#define IS31FL3236A_LED44_ADDR       0x27

#define IS31FL3236A_LED1_ADDR        0x28
#define IS31FL3236A_LED2_ADDR        0x29
#define IS31FL3236A_LED3_ADDR        0x2A
#define IS31FL3236A_LED4_ADDR        0x2B
#define IS31FL3236A_LED5_ADDR        0x2C
#define IS31FL3236A_LED6_ADDR        0x2D
#define IS31FL3236A_LED7_ADDR        0x2E
#define IS31FL3236A_LED8_ADDR        0x2F
#define IS31FL3236A_LED9_ADDR        0x30
#define IS31FL3236A_LED10_ADDR       0x31
#define IS31FL3236A_LED11_ADDR       0x32
#define IS31FL3236A_LED12_ADDR       0x33
#define IS31FL3236A_LED13_ADDR       0x34

#define IS31FL3236A_LED19_ADDR       0x39
#define IS31FL3236A_LED20_ADDR       0x3A
#define IS31FL3236A_LED21_ADDR       0x3B
#define IS31FL3236A_LED22_ADDR       0x3C

#define IS31FL3236A_LED27_ADDR       0x41
#define IS31FL3236A_LED28_ADDR       0x42
#define IS31FL3236A_LED29_ADDR       0x43
#define IS31FL3236A_LED30_ADDR       0x44
#define IS31FL3236A_LED31_ADDR       0x45
#define IS31FL3236A_LED32_ADDR       0x46
#define IS31FL3236A_LED33_ADDR       0x47
#define IS31FL3236A_LED34_ADDR       0x48
#define IS31FL3236A_LED35_ADDR       0x49

/* Address for PWM value of LED */
#define IS31FL3236A_LED43_PWM_ADDR   0x01
#define IS31FL3236A_LED44_PWM_ADDR   0x02

#define IS31FL3236A_LED1_PWM_ADDR    0x03
#define IS31FL3236A_LED2_PWM_ADDR    0x04
#define IS31FL3236A_LED3_PWM_ADDR    0x05
#define IS31FL3236A_LED4_PWM_ADDR    0x06
#define IS31FL3236A_LED5_PWM_ADDR    0x07
#define IS31FL3236A_LED6_PWM_ADDR    0x08
#define IS31FL3236A_LED7_PWM_ADDR    0x09
#define IS31FL3236A_LED8_PWM_ADDR    0x0A
#define IS31FL3236A_LED9_PWM_ADDR    0x0B
#define IS31FL3236A_LED10_PWM_ADDR   0x0C
#define IS31FL3236A_LED11_PWM_ADDR   0x0D
#define IS31FL3236A_LED12_PWM_ADDR   0x0E
#define IS31FL3236A_LED13_PWM_ADDR   0x0F

#define IS31FL3236A_LED19_PWM_ADDR   0x14
#define IS31FL3236A_LED20_PWM_ADDR   0x15
#define IS31FL3236A_LED21_PWM_ADDR   0x16
#define IS31FL3236A_LED22_PWM_ADDR   0x17

#define IS31FL3236A_LED27_PWM_ADDR   0x1C
#define IS31FL3236A_LED28_PWM_ADDR   0x1D
#define IS31FL3236A_LED29_PWM_ADDR   0x1E
#define IS31FL3236A_LED30_PWM_ADDR   0x1F
#define IS31FL3236A_LED31_PWM_ADDR   0x20
#define IS31FL3236A_LED32_PWM_ADDR   0x21
#define IS31FL3236A_LED33_PWM_ADDR   0x22
#define IS31FL3236A_LED34_PWM_ADDR   0x23
#define IS31FL3236A_LED35_PWM_ADDR   0x24

//#define IS31FL3236A_ADDR            0x3C
//#define IS31FL3236A_ON_REG          0x00
//#define IS31FL3236A_RESET_REG       0x17
//#define ZERO_ONE_CUSTOM             0x01


/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 *  Global Function declaration
 *
**********************************************************************/
//Basic set up
I2C_Handle setup_i2c(void);
void i2c_write(I2C_Handle handle, uint8_t slave_addr, uint8_t *write_buf, size_t write_count);
void i2c_init_buf(I2C_Handle handle, uint8_t slave_addr);

//LED basic functions
void reset_led_driver(I2C_Handle handle);
void turn_on_led_driver(I2C_Handle handle);
void turn_off_led_driver(I2C_Handle handle);//Software shutdown
void enable_channels(I2C_Handle handle);//all channels enable
void disable_channels(I2C_Handle handle);
void update_bit(I2C_Handle handle);//update register
void Brightness(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);//adjust brightness
void Flashing (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf, uint8_t index);//flash once
void PWM_Frequency_22k(I2C_Handle handle);
void PWM_Frequency_3k(I2C_Handle handle);
void LED_Turn_OFF_ALL(I2C_Handle handle);//turn off all led (set PWM to 0)
void LED_Turn_ON_ALL(I2C_Handle handle);//turn on all led (set PWM to 0)

//LED control
//void set_pwm_duty_cycle(I2C_Handle handle);
//void set_led_register(I2C_Handle handle);
//void IS31FL3236A_Set_Output_Frequency_22kHz();

//Modes / Status
void IS31FL3236A_Sports_Mode_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Leisure_Mode_pin( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Amble_Mode_pin( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Sports_Mode(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Leisure_Mode( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Amble_Mode( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Bluetooth_LED(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Light_On(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Auto_Mode_pin( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Auto_Mode( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Attention( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

//Battery bar pins
void IS31FL3236A_1_Bar( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_2_Bar( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_3_Bar( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_4_Bar( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_5_Bar( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

//Battery bar control
void IS31FL3236A_All_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_4_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_3_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_2_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_1_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_0_Bars( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

// Speed indicator
void IS31FL3236A_kmph_pin( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_mph_pin( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_kmph(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_mph(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

//Number LED sigle
void IS31FL3236A_Digit_1_PIN_44( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_3( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_5( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_30( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_31( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_32( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_PIN_33( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

void IS31FL3236A_Digit_2_PIN_8( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_9( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_12( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_27( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_28( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_29( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_PIN_13( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

//Digits Numbers
void IS31FL3236A_Digit_1_Number_0 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_1 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_2 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_3 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_4 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_5 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_6 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_7 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_8 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_9 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

void IS31FL3236A_Digit_2_Number_0 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_1 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_2 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_3 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_4 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_5 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_6 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_7 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_8 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_9 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);

//Digits Alphabets
void IS31FL3236A_Digit_1_Number_A (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_A (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_C (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_C (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_E (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_E (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_F (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_F (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_H (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_H (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_J (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_J (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_L (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_L (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_P (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_P (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_U (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_U (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_u (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_u (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_n (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_n (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_b (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_b (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_c (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_c (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_h (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_h (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_d (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_d (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_q (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_q (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_1_Number_ (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);
void IS31FL3236A_Digit_2_Number_ (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf);


//Function table
typedef void (*IS31FL3236A_Function)(I2C_Handle handle, uint8_t status_buf, uint8_t brightness_buf);
typedef enum {
    BRIGHTNESS,
    SPORTS_MODE, LEISURE_MODE, AMBLE_MODE, BLUETOOTH_LED, LIGHT_ON, AUTO_MODE, ATTENTION,
    BAR_1, BAR_2, BAR_3, BAR_4, BAR_5, KMPH, MPH,
    DIGIT_1_PIN_44, DIGIT_1_PIN_3, DIGIT_1_PIN_5, DIGIT_1_PIN_30, DIGIT_1_PIN_31, DIGIT_1_PIN_32, DIGIT_1_PIN_33,
    DIGIT_2_PIN_8, DIGIT_2_PIN_9,DIGIT_2_PIN_12, DIGIT_2_PIN_27, DIGIT_2_PIN_28,  DIGIT_2_PIN_29,DIGIT_2_PIN_13,
    ALL_BARS, BARS_4_ON, BARS_3_ON, BARS_2_ON, BARS_1_ON, BARS_0_ON,
    DIGIT_1_NO_0, DIGIT_1_NO_1, DIGIT_1_NO_2, DIGIT_1_NO_3, DIGIT_1_NO_4,
    DIGIT_1_NO_5, DIGIT_1_NO_6, DIGIT_1_NO_7, DIGIT_1_NO_8, DIGIT_1_NO_9,
    DIGIT_2_NO_0, DIGIT_2_NO_1, DIGIT_2_NO_2, DIGIT_2_NO_3, DIGIT_2_NO_4,
    DIGIT_2_NO_5, DIGIT_2_NO_6, DIGIT_2_NO_7, DIGIT_2_NO_8, DIGIT_2_NO_9,
    DIGIT_1_A, DIGIT_2_A, DIGIT_1_C, DIGIT_2_C, DIGIT_1_E, DIGIT_2_E,
    DIGIT_1_F, DIGIT_2_F, DIGIT_1_H, DIGIT_2_H, DIGIT_1_J, DIGIT_2_J,
    DIGIT_1_L, DIGIT_2_L, DIGIT_1_P, DIGIT_2_P, DIGIT_1_U, DIGIT_2_U,
    DIGIT_1_u, DIGIT_2_u, DIGIT_1_n, DIGIT_2_n, DIGIT_1_b, DIGIT_2_b,
    DIGIT_1_c, DIGIT_2_c, DIGIT_1_h, DIGIT_2_h, DIGIT_1_d, DIGIT_2_d,
    DIGIT_1_q, DIGIT_2_q, DIGIT_1_, DIGIT_2_,
    FUNCTION_COUNT
} IS31FL3236A_FunctionIndex;

extern IS31FL3236A_Function functionTable[FUNCTION_COUNT];


#ifdef _cplusplus
}
#endif

#endif /* IS31FL3236A_IS31FL3236A_H_ */
