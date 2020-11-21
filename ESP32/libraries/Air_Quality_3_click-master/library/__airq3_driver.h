/*
    __airq3_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __airq3_driver.h
@brief    AirQuality3 Driver
@mainpage AirQuality3 Click
@{

@image html sch.jpg

@}

@defgroup   ARIQ3
@brief      AirQuality3 Click Driver
@{

| Global Library Prefix | **ARIQ3** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **Jan 2018.**      |
| Developer             | **MikroE Team**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _ARIQ3_H_
#define _ARIQ3_H_

/** 
 * @macro T_ARIQ3_P
 * @brief Driver Abstract type 
 */
#define T_ARIQ3_P    const uint8_t*

/** @defgroup ARIQ3_COMPILE Compilation Config */              /** @{ */

//  #define   __ARIQ3_DRV_SPI__                            /**<     @macro __ARIQ3_DRV_SPI__  @brief SPI driver selector */
   #define   __ARIQ3_DRV_I2C__                            /**<     @macro __ARIQ3_DRV_I2C__  @brief I2C driver selector */                                          
// #define   __ARIQ3_DRV_UART__                           /**<     @macro __ARIQ3_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup ARIQ3_VAR Variables */                           /** @{ */


extern const uint8_t _AIRQ3_REG_ID;
extern const uint8_t _AIRQ3_REG_ERROR_ID;
extern const uint8_t _AIRQ3_REG_STATUS;
extern const uint8_t _AIRQ3_REG_HW_VERSION;
extern const uint8_t _AIRQ3_REG_FW_BOOT_VERSION;
extern const uint8_t _AIRQ3_REG_FW_APP_VERSION;
extern const uint8_t _AIRQ3_REG_SW_RESET;
extern const uint8_t _AIRQ3_REG_ALG_DATA;
extern const uint8_t _AIRQ3_REG_RAW_DATA;
extern const uint8_t _AIRQ3_REG_ENV_DATA;
extern const uint8_t _AIRQ3_REG_NTC;
extern const uint8_t _AIRQ3_REG_MEAS_MODE;
extern const uint8_t _AIRQ3_REG_BASELINE;

// MEAS MODE
extern const uint8_t _AIRQ3_DRIVE_MODE_0;
extern const uint8_t _AIRQ3_DRIVE_MODE_1;
extern const uint8_t _AIRQ3_DRIVE_MODE_2;
extern const uint8_t _AIRQ3_DRIVE_MODE_3;
extern const uint8_t _AIRQ3_DRIVE_MODE_4;
extern const uint8_t _AIRQ3_INT_DATARDY_0;
extern const uint8_t _AIRQ3_INT_DATARDY_1;
extern const uint8_t _AIRQ3_INT_THRESH_0;
extern const uint8_t _AIRQ3_INT_THRESH_1;

// APP FUNCTION
extern const uint8_t _AIRQ3_APP_START;
extern const uint8_t _AIRQ3_APP_VERIFY;
extern const uint8_t _AIRQ3_APP_DATA;
extern const uint8_t _AIRQ3_APP_ERASE;

                                                                       /** @} */
/** @defgroup ARIQ3_TYPES Types */                             /** @{ */



                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup ARIQ3_INIT Driver Initialization */              /** @{ */

#ifdef   __ARIQ3_DRV_SPI__
void airq3_spiDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P spiObj);
#endif
#ifdef   __ARIQ3_DRV_I2C__
void airq3_i2cDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P i2cObj, uint8_t slave);
#endif
#ifdef   __ARIQ3_DRV_UART__
void airq3_uartDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P uartObj);
#endif

// GPIO Only Drivers - remove in other cases
void airq3_gpioDriverInit(T_ARIQ3_P gpioObj);
                                                                       /** @} */
/** @defgroup ARIQ3_FUNC Driver Functions */                   /** @{ */

/**
 * @brief Function reads status register
 *
 * @return  value readed from the status register
 *
 */
uint8_t airq3_getStarus();

/**
 * @brief Function reads id value
 *
 * @return iD value from register
 *
 */
uint8_t airq3_getID();

/**
 * @brief Function reads Hardware version
 *
 * @return hardware version value from register
 *
 */
uint8_t airq3_getHWVersion();

/**
 * @brief Function reads firmware version
 *
 * @param[in] version    version witch will be readed
 * @param[out] pBuf

 * The readings are:
     Firmware Boot Version,
     Firmware Application Version.
 *
 */
void airq3_getFWVersion(uint8_t version, uint8_t *pBuf);

/**
 * @brief Function for software reset
 *
 * @param[in] reset    4 bytes for reset chip
 *
 * If the correct is 4 bytes (0x11 0xE5 0x72 0x8A) are written
   to this register in a single sequence the device will reset
   and return to BOOT mode.
 *
 */
void airq3_SoftReset(uint32_t reset);

/**
 * @brief Multidate reading function
 *
 * @param[out] AlgData    pointer to the buffer when data will be storaged
 *
 * This function returns a lot of data. 
   The first data in the buffer is the CO2 value,
   following the TVOC value, and the status register for CO2 and TVOC,
   error id value and RAW_DATA value.
 *
 */
void airq3_getCO2andTVOC(uint16_t *AlgData);

/**
 * @brief Function reads raw ADC data values for resistance and current source used.
 *
 * @param[out] RawData    pointer to the buffer when data will be storaged
 *
 */
void airq3_getRawData(uint16_t *RawData);

/**
 * @brief Function for settings temperature and humidity data
 *
 * @param[in] _data    4 bytes data for settings temperature and huminidy
 *
 * The internal algorithm uses these values (or default values if
   not set by the application) to compensate for changes in
   relative humidity and ambient temperature.
 *
 */
void airq3_setEnvironmentData(uint32_t _data);

/**
 * @brief Function reads NTC value
 *
 * @param[out] pBuf    pointer to the buffer when data will be storaged
 *
 * Four bytes read only a register that contains voltage across
   resistor (RREF) and voltage across the NTC resistor
   which determines the ambient temperature.
 *
 */
void airq3_getNTC(uint8_t *pBuf);

/**
 * @brief Function for settings sensor drive mode and interrupts.
 *
 * @param[in] mode    data to be written into the meas register
 *
 * set mode options:
    Mode 0 – Idle (Measurements are disabled in this mode)
    Mode 1 – Constant power mode, IAQ measurement every second
    Mode 2 – Pulse heating mode IAQ measurement every 10 seconds
    Mode 3 – Low power pulse heating mode IAQ measurement every 60 seconds
    Mode 4 – Constant power mode, sensor measurement every 250ms
 * set interrupts:
    0: Interrupt generation is disabled
    1: The nINT signal is asserted (driven low) when a new sample is ready in multiData register.
 * set thresholds:
    0: Interrupt mode (if enabled) operates normally
    1: Interrupt mode (if enabled) only asserts the nINT signal (driven low) if the new
       ALG_RESULT_DATA crosses one of the thresholds set in the THRESHOLDS register
       by more than the hysteresis value (also in the THRESHOLDS register).
 *
 */
void airq3_setMeasurementMode(uint8_t mode);

/**
 * @brief Function for settings baseline value
 *
 * @param[in] baseline   2 bytes data to be written into the baseline register
 *
 *  This function is used to correct the measurement,
    and set the baseline value that the algorithm uses to measure the CO2 and TVOC values.
 *
 */
void airq3_setBaseline(uint16_t baseline);

/**
 * @brief function to start APP task
 *
 * @param[in] app_task   1 bytes data to be written in register
 *
 *  Task options:
    App start - Application start. This command must be executed at the start.
    App verify - Starts the process of the bootloader checking though
    the application to make sure a full image is valid.
 *
 */
void airq3_AppFunction(uint8_t app_task);

/**
 * @brief Function for power ON chip
 */
void airq3_powerON();

/**
 * @brief Function for reset chip
 */
void airq3_reset();

/**
 * @brief Function for init click
 *
 * The function is used to initializing a click.
   The function calls powerON function, resets the function and starts the application.
 */
void airq3_init();





                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_AirQuality3_STM.c
    @example Click_AirQuality3_TIVA.c
    @example Click_AirQuality3_CEC.c
    @example Click_AirQuality3_KINETIS.c
    @example Click_AirQuality3_MSP.c
    @example Click_AirQuality3_PIC.c
    @example Click_AirQuality3_PIC32.c
    @example Click_AirQuality3_DSPIC.c
    @example Click_AirQuality3_AVR.c
    @example Click_AirQuality3_FT90x.c
    @example Click_AirQuality3_STM.mbas
    @example Click_AirQuality3_TIVA.mbas
    @example Click_AirQuality3_CEC.mbas
    @example Click_AirQuality3_KINETIS.mbas
    @example Click_AirQuality3_MSP.mbas
    @example Click_AirQuality3_PIC.mbas
    @example Click_AirQuality3_PIC32.mbas
    @example Click_AirQuality3_DSPIC.mbas
    @example Click_AirQuality3_AVR.mbas
    @example Click_AirQuality3_FT90x.mbas
    @example Click_AirQuality3_STM.mpas
    @example Click_AirQuality3_TIVA.mpas
    @example Click_AirQuality3_CEC.mpas
    @example Click_AirQuality3_KINETIS.mpas
    @example Click_AirQuality3_MSP.mpas
    @example Click_AirQuality3_PIC.mpas
    @example Click_AirQuality3_PIC32.mpas
    @example Click_AirQuality3_DSPIC.mpas
    @example Click_AirQuality3_AVR.mpas
    @example Click_AirQuality3_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __airq3_driver.h

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */