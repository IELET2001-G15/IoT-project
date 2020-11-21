/*
    __airq3_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__airq3_driver.h"
#include "__airq3_hal.c"

/* ------------------------------------------------------------------- MACROS */

const uint8_t _AIRQ3_REG_ID               = 0x20;
const uint8_t _AIRQ3_REG_ERROR_ID         = 0xE0;
const uint8_t _AIRQ3_REG_STATUS           = 0x00;
const uint8_t _AIRQ3_REG_HW_VERSION       = 0x21;
const uint8_t _AIRQ3_REG_FW_BOOT_VERSION  = 0x23;
const uint8_t _AIRQ3_REG_FW_APP_VERSION   = 0x24;
const uint8_t _AIRQ3_REG_SW_RESET         = 0xFF;
const uint8_t _AIRQ3_REG_ALG_DATA         = 0x02;
const uint8_t _AIRQ3_REG_RAW_DATA         = 0x03;
const uint8_t _AIRQ3_REG_ENV_DATA         = 0x05;
const uint8_t _AIRQ3_REG_NTC              = 0x06;
const uint8_t _AIRQ3_REG_MEAS_MODE        = 0x01;
const uint8_t _AIRQ3_REG_BASELINE         = 0x11;

// MEAS MODE
const uint8_t _AIRQ3_DRIVE_MODE_0   = 0x00 << 4;
const uint8_t _AIRQ3_DRIVE_MODE_1   = 0x01 << 4;
const uint8_t _AIRQ3_DRIVE_MODE_2   = 0x02 << 4;
const uint8_t _AIRQ3_DRIVE_MODE_3   = 0x03 << 4;
const uint8_t _AIRQ3_DRIVE_MODE_4   = 0x04 << 4;
const uint8_t _AIRQ3_INT_DATARDY_0  = 0x00 << 3;
const uint8_t _AIRQ3_INT_DATARDY_1  = 0x01 << 3;
const uint8_t _AIRQ3_INT_THRESH_0   = 0x00 << 2;
const uint8_t _AIRQ3_INT_THRESH_1   = 0x01 << 2;

const uint8_t _AIRQ3_APP_START   = 0xF4;
const uint8_t _AIRQ3_APP_VERIFY  = 0xF3;
const uint8_t _AIRQ3_APP_DATA    = 0xF2;
const uint8_t _AIRQ3_APP_ERASE   = 0xF1;











/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __ARIQ3_DRV_I2C__
static uint8_t _slaveAddress;
#endif

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __ARIQ3_DRV_SPI__

void airq3_spiDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __ARIQ3_DRV_I2C__

void airq3_i2cDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    //hal_gpio_csSet(1); // sleep mode
}

#endif
#ifdef   __ARIQ3_DRV_UART__

void airq3_uartDriverInit(T_ARIQ3_P gpioObj, T_ARIQ3_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif

// GPIO Only Drivers - remove in other cases
void airq3_gpioDriverInit(T_ARIQ3_P gpioObj)
{
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
}

/* ----------------------------------------------------------- IMPLEMENTATION */

uint8_t airq3_getStarus()
{
    uint8_t writeByte[1];
    uint8_t readReg[1];
    writeByte[0] = _AIRQ3_REG_STATUS;
    
    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, readReg, 1, END_MODE_STOP);
    Delay_100ms();
    
    return readReg[0];
}

uint8_t airq3_getID()    // 0x81
{
    uint8_t writeByte[1];
    uint8_t idReg[1];
    writeByte[0] = _AIRQ3_REG_ID;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, idReg, 1, END_MODE_STOP);
    Delay_100ms();
    
    return idReg[0];
}

uint8_t airq3_getHWVersion()  // 0x1X
{
    uint8_t writeByte[1];
    uint8_t readReg[1];
    writeByte[0] = _AIRQ3_REG_HW_VERSION;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, readReg, 1, END_MODE_STOP);
    Delay_100ms();
    
    return  readReg[0];
}

void airq3_getFWVersion(uint8_t version, uint8_t *pBuf)
{
    uint8_t writeByte[1];
    writeByte[0] = version;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, pBuf, 2, END_MODE_STOP);
    Delay_100ms();
}

void airq3_SoftReset(uint32_t reset)    // 0x11,0xE5,0x72,0x8A
{
    uint8_t writeByte[5];
    writeByte[0] = _AIRQ3_REG_SW_RESET;
    writeByte[1] = reset;
    writeByte[2] = reset >> 8;
    writeByte[3] = reset >> 16;
    writeByte[4] = reset >> 24;
     
    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 5, END_MODE_STOP);
    Delay_100ms();
}

void airq3_getCO2andTVOC(uint16_t *AlgData)
{
    uint8_t writeReg[1];
    uint8_t readReg[8];
    uint8_t RDY;
    writeReg[0] = _AIRQ3_REG_ALG_DATA;
    
    RDY = airq3_getStarus();
    if( RDY & 0x08 )
    {
        hal_i2cStart();
        hal_i2cWrite(_slaveAddress, writeReg, 1, END_MODE_RESTART);
        hal_i2cRead(_slaveAddress, readReg, 8, END_MODE_STOP);
        Delay_100ms();
    
        AlgData[0] = readReg[0];   // CO2
        AlgData[0] <<= 8;
        AlgData[0] |= readReg[1];

        AlgData[1] = readReg[2];  // TVOC
        AlgData[1] <<= 8;
        AlgData[1] |= readReg[3];

        AlgData[2] = readReg[4];  // status reg
        AlgData[3] = readReg[5];  // error ID
    
       AlgData[4] = readReg[6];  // RAW DATA
       AlgData[4] <<= 8;
       AlgData[4] |= readReg[7];
    }
}

void airq3_getRawData(uint16_t *RawData)
{
    uint8_t writeReg[1];
    uint8_t readReg[2];
    writeReg[0] = _AIRQ3_REG_RAW_DATA;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, readReg, 2, END_MODE_STOP);
    Delay_100ms();

    RawData[0] = readReg[0] & 0xFC;
    RawData[1] = readReg[0] & 0x03;
    RawData[1] <<= 8;
    RawData[1] |= readReg[1];
}

void airq3_setEnvironmentData(uint32_t _data)    // 0x11,0xE5,0x72,0x8A
{
    uint8_t writeByte[5];
    writeByte[0] = _AIRQ3_REG_ENV_DATA;
    writeByte[1] = _data;
    writeByte[2] = _data >> 8;
    writeByte[3] = _data >> 16;
    writeByte[4] = _data >> 24;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 5, END_MODE_STOP);
    Delay_100ms();
}

void airq3_getNTC(uint8_t *pBuf)
{
    uint8_t writeReg[1];
    writeReg[0] = _AIRQ3_REG_NTC;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress, pBuf, 4, END_MODE_STOP);
    Delay_100ms();
}

void airq3_setMeasurementMode(uint8_t mode)
{
    uint8_t writeByte[2];
    writeByte[0] = _AIRQ3_REG_MEAS_MODE;
    writeByte[1] = mode;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeByte, 2, END_MODE_STOP);
    Delay_100ms();
}

void airq3_setBaseline(uint16_t baseline)
{
    uint8_t writeReg[1];
    writeReg[0] = _AIRQ3_REG_BASELINE;
    writeReg[1] = baseline >> 8;
    writeReg[2] = baseline;
    
    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 3, END_MODE_RESTART);
    Delay_100ms();
}

void airq3_AppFunction(uint8_t app_task)
{
    uint8_t writeReg[1];

    writeReg[0] = app_task;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress, writeReg, 1, END_MODE_STOP);

    Delay_100ms();
}

void airq3_powerON()
{
   hal_gpio_csSet(0);
}

void airq3_reset()
{
   hal_gpio_rstSet(0);
   Delay_1sec();
   Delay_1sec();
   hal_gpio_rstSet(1);
   Delay_100ms();
   Delay_100ms();
}

void airq3_init()
{
   airq3_powerON();
   airq3_reset();
   airq3_AppFunction(_AIRQ3_APP_START);
}


/* -------------------------------------------------------------------------- */
/*
  __airq3_driver.c

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