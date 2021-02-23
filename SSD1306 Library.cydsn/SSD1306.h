/**
*   \file SSD1306.h
*
*   \brief Header file for SSD1306 library.
*/

#ifndef __SSD1306_H__
    #define __SSD1306_H__
    
    #include "cytypes.h"
    
    #define SSD1306_BLACK 0   ///< Draw 'off' pixels
    #define SSD1306_WHITE 1   ///< Draw 'on' pixels
    #define SSD1306_INVERSE 2 ///< Invert pixels
    
    void SSD1306_Start(void);
    
    void SSD1306_Display(void);
    
    void SSD1306_ClearDisplay(void);
    
    void SSD1306_InvertDisplay(uint8_t invert);
    
    void SSD1306_Dim(uint8_t dim);
    
    uint8_t SSD1306_GetWidth(void);
    
    uint8_t SSD1306_GetHeight(void);
    
    void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color);
    
    void SSD1306_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    
    void SSD1306_DrawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    
    void SSD1306_StartScrollRight(uint8_t start, uint8_t stop);
    
    void SSD1306_StartScrollLeft(uint8_t start, uint8_t stop);
    
    void SSD1306_StopScroll(void);
    
    void SSD1306_Command(uint8_t c);
    
    uint8 SSD1306_GetPixel(int16_t x, int16_t y);
    
    uint8_t *SSD1306_GetBuffer(void);
    
    
#endif


/* [] END OF FILE */
