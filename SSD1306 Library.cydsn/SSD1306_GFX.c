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

#include "stdlib.h"

#include "SSD1306_GFX.h"
#include "SSD1306.h"

#define _swap_int16_t(a, b)                                                    \
    {                                                                          \
        int16_t t = a;                                                         \
        a = b;                                                                 \
        b = t;                                                                 \
    }

    
void SSD1306_GFX_WriteLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                             uint16_t color) 
{

    float a = abs(10);
    
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) 
    {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) 
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) 
    {
        if (steep) 
        {
            SSD1306_GFX_WritePixel(y0, x0, color);
        } 
        else
        {
            SSD1306_GFX_WritePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) 
        {
            y0 += ystep;
            err += dx;
        }
    }
}

void SSD1306_GFX_WritePixel(int16_t x, int16_t y, uint16_t color) 
{
    SSD1306_DrawPixel(x, y, color);
}

void SSD1306_GFX_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color) 
{
    if (x0 == x1) 
    {
        if (y0 > y1)
        {
            _swap_int16_t(y0, y1);
        }
        SSD1306_DrawFastVLine(x0, y0, y1 - y0 + 1, color);
    } 
    else if (y0 == y1) 
    {
        if (x0 > x1)
        {
            _swap_int16_t(x0, x1);
        }
        SSD1306_DrawFastHLine(x0, y0, x1 - x0 + 1, color);
    } 
    else {
        SSD1306_GFX_WriteLine(x0, y0, x1, y1, color);
    }
}
/* [] END OF FILE */
