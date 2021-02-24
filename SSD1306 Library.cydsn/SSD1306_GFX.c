/**
 * This is the core graphics library for all our displays, providing a common
 * set of graphics primitives (points, lines, circles, etc.).  It needs to be
 * paired with a hardware-specific library for each display device we carry
 * (to handle the lower-level functions).

 * Adafruit invests time and resources providing this open source code, please
 * support Adafruit & open-source hardware by purchasing products from Adafruit!

 * Copyright (c) 2013 Adafruit Industries.  All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
