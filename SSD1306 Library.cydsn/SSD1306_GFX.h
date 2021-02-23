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
#ifndef __SSD1306_GFX_H__
    #define __SSD1306_GFX_H__
    
    #include "cytypes.h"
    
    void SSD1306_GFX_WriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                             uint16_t color);
    
    void SSD1306_GFX_WritePixel(int16_t x, int16_t y, uint16_t color);
    
    void SSD1306_GFX_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color);
    
#endif
/* [] END OF FILE */
