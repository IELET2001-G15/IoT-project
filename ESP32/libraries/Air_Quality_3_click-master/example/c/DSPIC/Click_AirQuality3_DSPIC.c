/*
Example for AirQuality3 Click

    Date          : Jan 2018.
    Author        : MikroE Team

Test configuration dsPIC :
    
    MCU                : P33FJ256GP710A
    Dev. Board         : EasyPIC Fusion v7
    dsPIC Compiler ver : v7.0.1.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes I2C module, set CS and RST pin as OUTPUT and INT pin as INPUT
- Application Initialization - Initializes Driver, initializes and settings mode AirQuality3 click
- Application Task - (code snippet) - This functions read CO2 and TVOC value in the air and LOG to USBUART.

*/

#include "Click_AirQuality3_types.h"
#include "Click_AirQuality3_config.h"

uint16_t AIRQ3_Data[5];
char text[256];

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_i2cInit( _MIKROBUS1, &_ARIQ3_I2C_CFG[0] );
    mikrobus_logInit( _LOG_USBUART_A, 9600 );
    Delay_ms( 100 );
}

void applicationInit()
{
    airq3_i2cDriverInit( (T_ARIQ3_P)&_MIKROBUS1_GPIO, (T_ARIQ3_P)&_MIKROBUS1_I2C, 0x5A);
    airq3_init();
    airq3_setMeasurementMode(_AIRQ3_DRIVE_MODE_4 | _AIRQ3_INT_DATARDY_1);
    mikrobus_logWrite("-------Init done------",_LOG_LINE);
    Delay_ms(3000);
}

void applicationTask()
{
    airq3_getCO2andTVOC(&AIRQ3_Data[0]);
    IntToStr(AIRQ3_Data[0],text);
    mikrobus_logWrite("CO2 value : ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_TEXT);
    mikrobus_logWrite("  ppm",_LOG_LINE);

    Delay_100ms();

    IntToStr(AIRQ3_Data[1],text);
    mikrobus_logWrite("TVOC value : ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_TEXT);
    mikrobus_logWrite("  ppb",_LOG_LINE);
    Delay_1sec();
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
            applicationTask();
    }
}