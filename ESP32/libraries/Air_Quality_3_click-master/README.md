![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# AirQuality3 Click

- **CIC Prefix**  : ARIQ3
- **mikroSDK**    : YES
- **Author**      : Katarina Perendic

---

### Software Support

We provide a library for the AirQuality3 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2302/air-quality-3-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

Library carries all necessary functions to have complete control over AirQuallity 3 Click board.

Key functions :

- ``` void airq3_getCO2andTVOC( uint16_t *algData ) ``` - Read Co2 and TVOC values
- ``` void airq3_setMeasurementMode( uint8_t mode ) ``` - Set measurement mode
- ``` void airq3_init() ``` - Default module initialization, must be called before module usage

**Examples Description**

The demo application is composed of three sections :

- System Initialization - Initializes I2C module, set CS and RST pin as OUTPUT and INT pin as INPUT
- Application Initialization - Initializes Driver, and module with default settings and also setup measurement mode
- Application Task - (code snippet) - This functions read CO2 and TVOC value in the air and LOG informations to UART. 
Operation is repeated each one second.


```.c
void applicationTask()
{
    airq3_getCO2andTVOC(&AIRQ3_Data[0]);

    ByteToStr(AIRQ3_Data[0],text);
    mikrobus_logWrite("CO2 value : ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_TEXT);
    mikrobus_logWrite("  ppm",_LOG_LINE);
     
    ByteToStr(AIRQ3_Data[1],text);
    mikrobus_logWrite("TVOC value : ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_TEXT);
    mikrobus_logWrite("  ppb",_LOG_LINE);

    Delay_1sec();
}
```

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2302/air-quality-3-click) page.

mikroE Libraries used in the example:

- UART
- I2C

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
