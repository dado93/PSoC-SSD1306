/**
 *   \file SSD1306.h
 *
 *   \brief Header file for SSD1306 library.
 *   \section author Author
 *
 *   Arduino Version:
 *     Written by Limor Fried/Ladyada for Adafruit Industries, with
 *     contributions from the open source community.
 *
 *   PSoC Version:
 *     Written dy Davide Marzorati.
 *
 *  \section license License
 *
 *  BSD license, all text above, and the splash screen included below,
 *  must be included in any redistribution.
*/

#ifndef __SSD1306_H__
    #define __SSD1306_H__
    
    #include "cytypes.h"
    
    /**
    *   \brief Pixel is drawn as black (i.e., OFF).
    */
    #define SSD1306_BLACK 0  
    
    /**
    *   \brief Pixel is drawn as white (i.e., ON).
    */
    #define SSD1306_WHITE 1  
    
    /**
    *   \brief Pixel is inverted.
    */
    #define SSD1306_INVERSE 2 
    
    /**
    *   \brief Start the display.
    *
    *   This function initializes the display with the configured 
    *   settings and prepares it for later use.
    *   \retval #SSD1306_OK if device was found and initialized.
    *   \retval #SSD1306_ERROR if error occurred during I2C communication.
    */
    uint8_t SSD1306_Start(void);
    
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
