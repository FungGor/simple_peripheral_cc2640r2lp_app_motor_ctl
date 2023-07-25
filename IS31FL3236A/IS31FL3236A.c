/*
 * IS31FL3236A.c
 *
 * IS31FL3236A 36-CHANNEL LED DRIVER; SELECTABLE PWM FREQUENCY
 *
 *  Created on: 12 Jul 2023
 *      Author: Jermyn & Chee
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Driver Header files */
//#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

#include "Board.h"
#include "IS31FL3236A/IS31FL3236A.h"

//Basic set up

I2C_Handle setup_i2c(void)
{
     Board_init();
     I2C_init();
//     I2CCC26XX_I2CPinCfg i2cPinCfg = {
//                .pinSDA = Board_I2C0_SDA0,
//                .pinSCL = Board_I2C0_SCL0
//            };
    I2C_Params i2cParams;
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    I2C_Handle handle = I2C_open(Board_I2C0, &i2cParams);
//    i2cParams.custom = &i2cPinCfg; // Uncomment if you need to set custom pin configuration

    return handle;
}

void i2c_init_buf(I2C_Handle handle, uint8_t slave_addr)
{
    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = slave_addr;
    i2cTransaction.writeBuf = NULL;
    i2cTransaction.writeCount = 0;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    I2C_transfer(handle, &i2cTransaction);
}

void i2c_write(I2C_Handle handle, uint8_t slave_addr, uint8_t *write_buf, size_t write_count)
{
    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = slave_addr;
    i2cTransaction.writeBuf = write_buf;
    i2cTransaction.writeCount = write_count;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    I2C_transfer(handle, &i2cTransaction);
}

//LED basic functions
void reset_led_driver(I2C_Handle handle)
{
    /*
    Basic steps:

    I2C_Transaction i2cTransaction;
    i2cTransaction.slaveAddress = IS31FL3236A_ADDR;
    uint8_t writeBuf_reset[2] = {IS31FL3236A_RESET_REG, 0x00};
    i2cTransaction.writeBuf = writeBuf_reset;
    i2cTransaction.writeCount = sizeof(writeBuf_reset);
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    I2C_transfer(handle, &i2cTransaction);
    */

    uint8_t writeBuf_reset[2] = {IS31FL3236A_RESET_REG, 0x00};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_reset, sizeof(writeBuf_reset));
}


void turn_on_led_driver(I2C_Handle handle)
{
    uint8_t writeBuf_shutdown[2] = {IS31FL3236A_ON_REG, ZERO_ONE_CUSTOM};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_shutdown, sizeof(writeBuf_shutdown));

}

void turn_off_led_driver(I2C_Handle handle)
{
    uint8_t writeBuf_shutdown[2] = {IS31FL3236A_ON_REG, ZERO_ZERO_CUSTOM};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_shutdown, sizeof(writeBuf_shutdown));
}

void disable_channels(I2C_Handle handle)
{
    uint8_t writeBuf_disable[2] = {IS31FL3236A_ENABLE_REG, 0x01};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_disable, sizeof(writeBuf_disable));
}

void enable_channels(I2C_Handle handle)
{
    uint8_t writeBuf_enable[2] = {IS31FL3236A_ENABLE_REG, 0x00};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_enable, sizeof(writeBuf_enable));
}


void update_bit(I2C_Handle handle)
{
    uint8_t writeBuf_update[2] = {IS31FL3236A_UPDATE_REG, 0x00};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_update, sizeof(writeBuf_update));
}

void PWM_Frequency_22k(I2C_Handle handle)
{
    uint8_t writeBuf_22k[2] = {IS31FL3236A_PWM_FREQUENCY, 0x01};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_22k, sizeof(writeBuf_22k));
}
void PWM_Frequency_3k(I2C_Handle handle)
{
    uint8_t writeBuf_3k[2] = {IS31FL3236A_PWM_FREQUENCY, 0x00};
    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_3k, sizeof(writeBuf_3k));
}

//void IS31FL3236A_Sports_PWM(I2C_Handle handle)
//{
//    uint8_t writeBuf_pwm[2] = {IS31FL3236A_PWM_REG, PWM_CUSTOM};
//    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));
//}
//
//void IS31FL3236A_Sports_LED(I2C_Handle handle)
//{
//    uint8_t writeBuf_led[2] = {IS31FL3236A_LED_REG, ZERO_ONE_CUSTOM};
//    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
//}
void LED_Turn_OFF_ALL(I2C_Handle handle){
       IS31FL3236A_Sports_Mode_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Leisure_Mode_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Amble_Mode_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Bluetooth_LED(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Light_On(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Auto_Mode_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Attention(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_1_Bar(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_2_Bar(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_3_Bar(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_4_Bar(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_5_Bar(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_kmph_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_mph_pin(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_44(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_3(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_5(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_30(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_31(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_32(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_33(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_8(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_9(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_12(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_27(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_28(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_29(handle,I_OUT,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_13(handle,I_OUT,PWM_ZERO);
}

void LED_Turn_ON_ALL(I2C_Handle handle){
       IS31FL3236A_Sports_Mode_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Leisure_Mode_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Amble_Mode_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Bluetooth_LED(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Light_On(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Auto_Mode_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Attention(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_1_Bar(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_2_Bar(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_3_Bar(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_4_Bar(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_5_Bar(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_kmph_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_mph_pin(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_44(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_3(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_5(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_30(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_31(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_32(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_1_PIN_33(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_8(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_9(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_12(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_27(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_28(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_29(handle,I_OUT,PWM_CUSTOM);
       IS31FL3236A_Digit_2_PIN_13(handle,I_OUT,PWM_CUSTOM);
}

void IS31FL3236A_Sports_Mode_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED43_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED43_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Leisure_Mode_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED35_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED35_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Amble_Mode_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED34_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED34_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Sports_Mode(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Sports_Mode_pin(handle,status_buf, brightness_buf);
    IS31FL3236A_Leisure_Mode_pin(handle,status_buf, PWM_ZERO);
    IS31FL3236A_Amble_Mode_pin(handle,status_buf, PWM_ZERO);
}
void IS31FL3236A_Leisure_Mode(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Sports_Mode_pin(handle,status_buf, PWM_ZERO);
    IS31FL3236A_Leisure_Mode_pin(handle,status_buf, brightness_buf);
    IS31FL3236A_Amble_Mode_pin(handle,status_buf, PWM_ZERO);
}
void IS31FL3236A_Amble_Mode(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Sports_Mode_pin(handle,status_buf, PWM_ZERO);
    IS31FL3236A_Leisure_Mode_pin(handle,status_buf, PWM_ZERO);
    IS31FL3236A_Amble_Mode_pin(handle,status_buf, brightness_buf);
}

void IS31FL3236A_Bluetooth_LED(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED19_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED19_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Light_On(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED20_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED20_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Auto_Mode_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED21_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED21_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Auto_Mode( I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Light_On(handle,status_buf, brightness_buf);
    IS31FL3236A_Auto_Mode_pin(handle,status_buf, brightness_buf);
}

void IS31FL3236A_Attention(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED22_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED22_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_1_Bar(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED1_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED1_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_2_Bar(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED2_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED2_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_3_Bar(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED6_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED6_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_4_Bar(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED10_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED10_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_5_Bar(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED11_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED11_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_kmph_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED4_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED4_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_mph_pin(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED7_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED7_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_kmph(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_kmph_pin(handle,status_buf, brightness_buf);        // mph Off
    IS31FL3236A_mph_pin(handle,status_buf, PWM_ZERO);               // kmph On

}
void IS31FL3236A_mph(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_kmph_pin(handle,status_buf, PWM_ZERO);              // kmph Off
    IS31FL3236A_mph_pin(handle,status_buf, brightness_buf);         // mph On
}

void IS31FL3236A_Digit_1_PIN_44(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED44_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED44_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_3(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED3_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED3_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_5(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED5_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED5_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_30(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED30_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED30_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_31(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED31_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED31_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_32(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED32_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED32_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_1_PIN_33(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED33_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED33_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_Digit_2_PIN_8(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED8_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED8_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_9(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED9_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED9_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_12(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED12_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED12_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_27(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED27_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED27_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_28(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED28_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED28_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_29(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED29_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED29_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}
void IS31FL3236A_Digit_2_PIN_13(I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{
    uint8_t writeBuf_pwm[2] = {IS31FL3236A_LED13_PWM_ADDR, brightness_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));

    uint8_t writeBuf_led[2] = {IS31FL3236A_LED13_ADDR, status_buf};

    i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
    update_bit(handle);
}

void IS31FL3236A_All_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_2_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_3_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_4_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_5_Bar(handle,status_buf,brightness_buf);
}

void IS31FL3236A_4_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_2_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_3_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_4_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_5_Bar(handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_3_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_2_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_3_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_4_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_5_Bar(handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_2_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_2_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_3_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_4_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_5_Bar(handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_1_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
    IS31FL3236A_2_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_3_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_4_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_5_Bar(handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_0_Bars (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_1_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_2_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_3_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_4_Bar(handle,status_buf,PWM_ZERO);
    IS31FL3236A_5_Bar(handle,status_buf,PWM_ZERO);
}


void IS31FL3236A_Digit_1_Number_0 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32(handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33(handle,status_buf,PWM_ZERO);

}

void IS31FL3236A_Digit_1_Number_1 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);        // PIN_44 off
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);         // PIN_3 off
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);   // PIN_5 on
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);  // PIN_30 on
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);        // PIN_31 off
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);        // PIN_32 off
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);        // PIN_33 off

}

void IS31FL3236A_Digit_1_Number_2 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_3 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_4 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_5 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_6 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_7 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);

}

void IS31FL3236A_Digit_1_Number_8 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_1_Number_9 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_0 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);

}

void IS31FL3236A_Digit_2_Number_1 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);

}

void IS31FL3236A_Digit_2_Number_2 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_3 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_4 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_5 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_6 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_7 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);

}

void IS31FL3236A_Digit_2_Number_8 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void IS31FL3236A_Digit_2_Number_9 (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){

        IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
        IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}

void Brightness (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf)
{

        IS31FL3236A_Sports_Mode_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_Leisure_Mode_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_Amble_Mode_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_Bluetooth_LED(handle,status_buf,brightness_buf);
        IS31FL3236A_Light_On(handle,status_buf,brightness_buf);
        IS31FL3236A_Auto_Mode_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_Attention(handle,status_buf,brightness_buf);
        IS31FL3236A_1_Bar(handle,status_buf,brightness_buf);
        IS31FL3236A_2_Bar(handle,status_buf,brightness_buf);
        IS31FL3236A_3_Bar(handle,status_buf,brightness_buf);
        IS31FL3236A_4_Bar(handle,status_buf,brightness_buf);
        IS31FL3236A_5_Bar(handle,status_buf,brightness_buf);
        IS31FL3236A_kmph_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_mph_pin(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_44(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_3(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_5(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_30(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_31(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_32(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_1_PIN_33(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_8(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_9(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_12(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_27(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_28(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_29(handle,status_buf,brightness_buf);
        IS31FL3236A_Digit_2_PIN_13(handle,status_buf,brightness_buf);
}



void IS31FL3236A_Digit_1_Number_A (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
       IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_A (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
       IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
       IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_C (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
       IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
       IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
       IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_2_Number_C (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_1_Number_E (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_E (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);

}
void IS31FL3236A_Digit_1_Number_F (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_F (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_H (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_H (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_J (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_2_Number_J (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_1_Number_L (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_2_Number_L (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_1_Number_P (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_P (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_U (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_2_Number_U (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_1_Number_u (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,PWM_ZERO);
}
void IS31FL3236A_Digit_2_Number_u (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,PWM_ZERO);
}

void IS31FL3236A_Digit_1_Number_n (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_n (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_b (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_b (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_c (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_c (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_h (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_h (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_d (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_d (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_q (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_q (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,brightness_buf);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_1_Number_ (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_1_PIN_44( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_3( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_5( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_30( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_31( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_32( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_1_PIN_33( handle,status_buf,brightness_buf);
}
void IS31FL3236A_Digit_2_Number_ (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf){
    IS31FL3236A_Digit_2_PIN_8( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_9( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_12( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_27( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_28( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_29( handle,status_buf,PWM_ZERO);
    IS31FL3236A_Digit_2_PIN_13( handle,status_buf,brightness_buf);
}



//void FlashingOri (I2C_Handle handle,uint8_t led_pin,uint8_t led_pwm,uint8_t status_buf, uint8_t brightness_buf){
//
//        uint8_t writeBuf_pwm[2] = {led_pwm, brightness_buf};
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));
//
//        uint8_t writeBuf_led[2] = {led_pin, status_buf};
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
//        usleep(300000);
//        writeBuf_pwm[1] =PWM_ZERO;
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));
//
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
//        usleep(300000);
//        writeBuf_pwm[1] =brightness_buf;
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_pwm, sizeof(writeBuf_pwm));
//
//        update_bit(handle);
//        i2c_write(handle, IS31FL3236A_ADDR, writeBuf_led, sizeof(writeBuf_led));
//
//}

void Flashing (I2C_Handle handle,uint8_t status_buf, uint8_t brightness_buf, uint8_t index){
    if(brightness_buf==PWM_ZERO){
    functionTable[index](handle, status_buf, brightness_buf);
    usleep(200000);
    functionTable[index](handle, status_buf, PWM_ZERO);
    usleep(200000);
    };
    if(brightness_buf==brightness_buf){
    functionTable[index](handle, status_buf, PWM_ZERO);
    usleep(200000);
    functionTable[index](handle, status_buf, brightness_buf);
    usleep(200000);
    };

}
// Function tables

IS31FL3236A_Function functionTable[FUNCTION_COUNT] = {
    Brightness,
    IS31FL3236A_Sports_Mode,
    IS31FL3236A_Leisure_Mode,
    IS31FL3236A_Amble_Mode,
    IS31FL3236A_Bluetooth_LED,
    IS31FL3236A_Light_On,
    IS31FL3236A_Auto_Mode,
    IS31FL3236A_Attention,
    IS31FL3236A_1_Bar,
    IS31FL3236A_2_Bar,
    IS31FL3236A_3_Bar,
    IS31FL3236A_4_Bar,
    IS31FL3236A_5_Bar,
    IS31FL3236A_kmph,
    IS31FL3236A_mph,
    IS31FL3236A_Digit_1_PIN_44,
    IS31FL3236A_Digit_1_PIN_3,
    IS31FL3236A_Digit_1_PIN_5,
    IS31FL3236A_Digit_1_PIN_30,
    IS31FL3236A_Digit_1_PIN_31,
    IS31FL3236A_Digit_1_PIN_32,
    IS31FL3236A_Digit_1_PIN_33,
    IS31FL3236A_Digit_2_PIN_8,
    IS31FL3236A_Digit_2_PIN_9,
    IS31FL3236A_Digit_2_PIN_12,
    IS31FL3236A_Digit_2_PIN_27,
    IS31FL3236A_Digit_2_PIN_28,
    IS31FL3236A_Digit_2_PIN_29,
    IS31FL3236A_Digit_2_PIN_13,
    IS31FL3236A_All_Bars,
    IS31FL3236A_4_Bars,
    IS31FL3236A_3_Bars,
    IS31FL3236A_2_Bars,
    IS31FL3236A_1_Bars,
    IS31FL3236A_0_Bars,
    IS31FL3236A_Digit_1_Number_0,
    IS31FL3236A_Digit_1_Number_1,
    IS31FL3236A_Digit_1_Number_2,
    IS31FL3236A_Digit_1_Number_3,
    IS31FL3236A_Digit_1_Number_4,
    IS31FL3236A_Digit_1_Number_5,
    IS31FL3236A_Digit_1_Number_6,
    IS31FL3236A_Digit_1_Number_7,
    IS31FL3236A_Digit_1_Number_8,
    IS31FL3236A_Digit_1_Number_9,
    IS31FL3236A_Digit_2_Number_0,
    IS31FL3236A_Digit_2_Number_1,
    IS31FL3236A_Digit_2_Number_2,
    IS31FL3236A_Digit_2_Number_3,
    IS31FL3236A_Digit_2_Number_4,
    IS31FL3236A_Digit_2_Number_5,
    IS31FL3236A_Digit_2_Number_6,
    IS31FL3236A_Digit_2_Number_7,
    IS31FL3236A_Digit_2_Number_8,
    IS31FL3236A_Digit_2_Number_9,
    IS31FL3236A_Digit_1_Number_A,
    IS31FL3236A_Digit_2_Number_A,
    IS31FL3236A_Digit_1_Number_C,
    IS31FL3236A_Digit_2_Number_C,
    IS31FL3236A_Digit_1_Number_E,
    IS31FL3236A_Digit_2_Number_E,
    IS31FL3236A_Digit_1_Number_F,
    IS31FL3236A_Digit_2_Number_F,
    IS31FL3236A_Digit_1_Number_H,
    IS31FL3236A_Digit_2_Number_H,
    IS31FL3236A_Digit_1_Number_J,
    IS31FL3236A_Digit_2_Number_J,
    IS31FL3236A_Digit_1_Number_L,
    IS31FL3236A_Digit_2_Number_L,
    IS31FL3236A_Digit_1_Number_P,
    IS31FL3236A_Digit_2_Number_P,
    IS31FL3236A_Digit_1_Number_U,
    IS31FL3236A_Digit_2_Number_U,
    IS31FL3236A_Digit_1_Number_u,
    IS31FL3236A_Digit_2_Number_u,
    IS31FL3236A_Digit_1_Number_n,
    IS31FL3236A_Digit_2_Number_n,
    IS31FL3236A_Digit_1_Number_b,
    IS31FL3236A_Digit_2_Number_b,
    IS31FL3236A_Digit_1_Number_c,
    IS31FL3236A_Digit_2_Number_c,
    IS31FL3236A_Digit_1_Number_h,
    IS31FL3236A_Digit_2_Number_h,
    IS31FL3236A_Digit_1_Number_d,
    IS31FL3236A_Digit_2_Number_d,
    IS31FL3236A_Digit_1_Number_q,
    IS31FL3236A_Digit_2_Number_q,
    IS31FL3236A_Digit_1_Number_,
    IS31FL3236A_Digit_2_Number_,

};

//Steps to call
//IS31FL3236A_FunctionIndex index = LIGHT_ON;
//functionTable[index](handle, status_buf, brightness_buf);


//Brightness 32 Gamma Steps
const uint8_t brightness_32_ori[]={
                               0x00, 0x01, 0x02, 0x04, 0x06, 0x0A, 0x0D, 0x12,
                               0x16, 0x1C, 0x21, 0x27, 0x2E, 0x35, 0x3D, 0x45,
                               0x4E, 0x56, 0x60, 0x6A, 0x74, 0x7E, 0x8A, 0x95,
                               0xA1, 0xAD, 0xBA, 0xC7, 0xD4, 0xE2, 0xF0, 0xFF
                                };
const uint8_t brightness_32[]={
                               0x00, 0x01, 0x02, 0x04, 0x06, 0x0A, 0x0D, 0x12,
                               0x16, 0x1C, 0x21, 0x27, 0x2E, 0x35, 0x3D, 0x45,
                               0x4E, 0x56, 0x60, 0x6A, 0x74, 0x7E, 0x8A, 0x95,
                               0xA1, 0xAD, 0xBA, 0xC7, 0xD4, 0xE2, 0xF0, 0xFF
                                };

//Brightness 64 Gamma Steps
const uint8_t brightness_64_ori[]={
                               0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16,
                               0x18, 0x1A, 0x1D, 0x20, 0x23, 0x26, 0x29, 0x2C,
                               0x2F, 0x32, 0x35, 0x39, 0x3D, 0x41, 0x45, 0x49,
                               0x4D, 0x51, 0x55, 0x59, 0x5E, 0x63, 0x68, 0x6D,
                               0x72, 0x77, 0x7C, 0x81, 0x86, 0x8C, 0x92, 0x98,
                               0x9E, 0xA4, 0xAA, 0xB0, 0xB6, 0xBC, 0xC3, 0xCA,
                               0xD1, 0xD8, 0xDF, 0xE6, 0xED, 0xF4, 0xFB, 0xFF
                                };
const uint8_t brightness_64[]={
                               0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16,
                               0x18, 0x1A, 0x1D, 0x20, 0x23, 0x26, 0x29, 0x2C,
                               0x2F, 0x32, 0x35, 0x39, 0x3D, 0x41, 0x45, 0x49,
                               0x4D, 0x51, 0x55, 0x59, 0x5E, 0x63, 0x68, 0x6D,
                               0x72, 0x77, 0x7C, 0x81, 0x86, 0x8C, 0x92, 0x98,
                               0x9E, 0xA4, 0xAA, 0xB0, 0xB6, 0xBC, 0xC3, 0xCA,
                               0xD1, 0xD8, 0xDF, 0xE6, 0xED, 0xF4, 0xFB, 0xFF
                                };
// LED pin array
const uint8_t led_array[][2] = {
    {IS31FL3236A_LED43_ADDR, IS31FL3236A_LED43_PWM_ADDR},
    {IS31FL3236A_LED44_ADDR, IS31FL3236A_LED44_PWM_ADDR},
    {IS31FL3236A_LED1_ADDR,  IS31FL3236A_LED1_PWM_ADDR},
    {IS31FL3236A_LED2_ADDR,  IS31FL3236A_LED2_PWM_ADDR},
    {IS31FL3236A_LED3_ADDR,  IS31FL3236A_LED3_PWM_ADDR},
    {IS31FL3236A_LED4_ADDR,  IS31FL3236A_LED4_PWM_ADDR},
    {IS31FL3236A_LED5_ADDR,  IS31FL3236A_LED5_PWM_ADDR},
    {IS31FL3236A_LED6_ADDR,  IS31FL3236A_LED6_PWM_ADDR},
    {IS31FL3236A_LED7_ADDR,  IS31FL3236A_LED7_PWM_ADDR},
    {IS31FL3236A_LED8_ADDR,  IS31FL3236A_LED8_PWM_ADDR},
    {IS31FL3236A_LED9_ADDR,  IS31FL3236A_LED9_PWM_ADDR},
    {IS31FL3236A_LED10_ADDR, IS31FL3236A_LED10_PWM_ADDR},
    {IS31FL3236A_LED11_ADDR, IS31FL3236A_LED11_PWM_ADDR},
    {IS31FL3236A_LED12_ADDR, IS31FL3236A_LED12_PWM_ADDR},
    {IS31FL3236A_LED13_ADDR, IS31FL3236A_LED13_PWM_ADDR},
    {IS31FL3236A_LED19_ADDR, IS31FL3236A_LED19_PWM_ADDR},
    {IS31FL3236A_LED20_ADDR, IS31FL3236A_LED20_PWM_ADDR},
    {IS31FL3236A_LED21_ADDR, IS31FL3236A_LED21_PWM_ADDR},
    {IS31FL3236A_LED22_ADDR, IS31FL3236A_LED22_PWM_ADDR},
    {IS31FL3236A_LED27_ADDR, IS31FL3236A_LED27_PWM_ADDR},
    {IS31FL3236A_LED28_ADDR, IS31FL3236A_LED28_PWM_ADDR},
    {IS31FL3236A_LED29_ADDR, IS31FL3236A_LED29_PWM_ADDR},
    {IS31FL3236A_LED30_ADDR, IS31FL3236A_LED30_PWM_ADDR},
    {IS31FL3236A_LED31_ADDR, IS31FL3236A_LED31_PWM_ADDR},
    {IS31FL3236A_LED32_ADDR, IS31FL3236A_LED32_PWM_ADDR},
    {IS31FL3236A_LED33_ADDR, IS31FL3236A_LED33_PWM_ADDR},
    {IS31FL3236A_LED34_ADDR, IS31FL3236A_LED34_PWM_ADDR},
    {IS31FL3236A_LED35_ADDR, IS31FL3236A_LED35_PWM_ADDR},
};

