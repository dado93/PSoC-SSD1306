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
#include "SSD1306_GFX.h"

void testdrawline();

int main(void)
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    I2C_Master_Start();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    CyDelay(100);
    
    SSD1306_Start();
    SSD1306_Display();
    CyDelay(1000);
    
    SSD1306_DrawPixel(64, 16, 1);
    SSD1306_Display();
    CyDelay(1000);
    
    testdrawline();
    
    SSD1306_StartScrollLeft(0x00, 0x0F);
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

void testdrawline() {
    int16_t i;

    SSD1306_ClearDisplay(); // Clear display buffer

    for( i=0; i < SSD1306_GetWidth(); i+=4) 
    {
        SSD1306_GFX_DrawLine(0, 0, i, SSD1306_GetHeight()-1, SSD1306_WHITE);
        SSD1306_Display(); // Update screen with each newly-drawn line
        CyDelay(1);
    }
    
    for(i=0; i < SSD1306_GetHeight(); i+=4) 
    {
        SSD1306_GFX_DrawLine(0, 0, SSD1306_GetWidth()-1, i, SSD1306_WHITE);
        SSD1306_Display(); 
        CyDelay(1);
    }
    CyDelay(250);

    SSD1306_ClearDisplay();

    for(i=0; i < SSD1306_GetWidth(); i+=4) 
    {
        SSD1306_GFX_DrawLine(0, SSD1306_GetHeight()-1, i, 0, SSD1306_WHITE);
        SSD1306_Display(); 
        CyDelay(1);
    }
    for(i=SSD1306_GetHeight()-1; i>=0; i-=4) 
    {
        SSD1306_GFX_DrawLine(0, SSD1306_GetHeight()-1, SSD1306_GetWidth()-1, i, SSD1306_WHITE);
        SSD1306_Display(); 
        CyDelay(1);
    }
    CyDelay(250);

    SSD1306_ClearDisplay();

    for(i=SSD1306_GetWidth()-1; i>=0; i-=4) 
    {
        SSD1306_GFX_DrawLine(SSD1306_GetWidth()-1, SSD1306_GetHeight()-1, i, 0, SSD1306_WHITE);
        SSD1306_Display(); 
        CyDelay(1);
    }
    
    for(i=SSD1306_GetHeight()-1; i>=0; i-=4) 
    {
        SSD1306_GFX_DrawLine(SSD1306_GetWidth()-1, SSD1306_GetHeight()-1, 0, i, SSD1306_WHITE);
        SSD1306_Display();
        CyDelay(1);
    }
    CyDelay(250);

    SSD1306_ClearDisplay();

    for(i=0; i < SSD1306_GetHeight(); i+=4) 
    {
        SSD1306_GFX_DrawLine(SSD1306_GetWidth()-1, 0, 0, i, SSD1306_WHITE);
        SSD1306_Display();
        CyDelay(1);
    }
    for(i=0; i<SSD1306_GetWidth(); i+=4) 
    {
        SSD1306_GFX_DrawLine(SSD1306_GetWidth()-1, 0, i, SSD1306_GetHeight()-1, SSD1306_WHITE);
        SSD1306_Display();
        CyDelay(1);
    }

    CyDelay(2000); // Pause for 2 seconds
}
/* [] END OF FILE */
