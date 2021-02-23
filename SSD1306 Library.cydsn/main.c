/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "i2c_interface.h"
#include "project.h"
#include "SSD1306.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Master_Start();
    
    if (I2C_Peripheral_IsDeviceConnected(0x3C) == 0x00)
    {
        Led_Write(1);
    }
    
    CyDelay(100);
    
    SSD1306_Start();
    SSD1306_Display();
    
    //SSD1306_DrawPixel(10, 10, 0);
    for (uint8_t y = 0; y < 32; y++)
    {
        SSD1306_DrawPixel(10, y, 1);
        SSD1306_DrawPixel(30, y, 0);
        SSD1306_DrawPixel(50, y, 2);
    }
    SSD1306_Display();
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
