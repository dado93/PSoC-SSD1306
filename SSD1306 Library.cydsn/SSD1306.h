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
*   
*/

#ifndef __SSD1306_H__
    #define __SSD1306_H__
    
    #include "cytypes.h"
    
    /**********************************
    *          COLOR MACROS
    **********************************/
    
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
    
    #define SSD1306_BLACK_ON_WHITE 1
    
    #define SSD1306_WHITE_ON_BLACK 0
    
    /**********************************
    *          ERROR CODES
    **********************************/
    
    /**
    *   \brief No error code.
    */
    #define SSD1306_NO_ERROR    0
    
    /**
    *   \brief I2C communication error
    */
    #define SSD1306_COMM_ERROR  1
    
    /**
    *   \brief Start the display.
    *
    *   This function initializes the display with the configured 
    *   settings and prepares it for later use.
    *
    *   \retval #SSD1306_OK if device was found and initialized.
    *   \retval #SSD1306_COMM_ERROR if error occurred during I2C communication.
    */
    uint8_t SSD1306_Start(void);
    
    /**
    *   \brief Refresh content of display.
    *
    *   This function sends the updated buffer to the display,
    *   so that the new content becomes visible. The whole
    *   buffer, which has a dimension dipending on display
    *   width and height, is sent over I2C. So make sure 
    *   to call this function when appropriate.
    *   
    *   \retval #SSD1306_OK if device was found and initialized.
    *   \retval #SSD1306_COMM_ERROR if error occurred during I2C communication.
    */
    uint8_t SSD1306_Display(void);
    
    /**
    *   \brief Clear current content of the display.
    *
    *   This function clears the current buffer, without
    *   sending it to the display. So if the display needs
    *   to be cleared, call #SSD1306_Display to refresh
    *   the display.
    */
    void SSD1306_ClearDisplay(void);
    
    /**
    *   \brief Invert display mode (white-on-black or black-on-white).
    *
    *   Default display mode is white-on-black. The value of the
    *   invert parameter allows to choose between white-on-black
    *   and black-on-white. If true, black-on-white is enabled.
    *
    *   \param invert invert mode. Possible values are:
    *       - #SSD1306_BLACK_ON_WHITE
    *       - #SSD1306_WHITE_ON_BLACK
    *   \retval #SSD1306_OK if no error was generated
    *   \retval #SSD1306_COMM_ERR if error occurred during I2C communication
    */
    uint8_t SSD1306_InvertDisplay(uint8_t invert);
    
    /**
    *   \brief Dim the display.
    *
    *   \param dim > 0 to enable lower brightness mode, = 0 for full brightness.
    *   \retval #SSD1306_OK if no error occurred.
    *   \retval #SSD1306_COMM_ERR if error occurred during I2C communication
    *   \note This function has an immediate effect on the display, and so there
    *   is no need to call the #SSD1306_Display function.
    */
    uint8_t SSD1306_Dim(uint8_t dim);
    

    
    void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color);
    
    void SSD1306_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    
    void SSD1306_DrawFastVLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    
    uint8_t SSD1306_StartScrollRight(uint8_t start, uint8_t stop);
    
    /**
    *   \brief Activate a left-handed scroll for all or part of the display.
    *
    *   \param start first row for the scroll.
    *   \param stop last row for the scroll.
    *   \retval #SSD1306_OK if no error occurred during configuration.
    *   \retval #SSD1306_COMM_ERR if I2C error occurred.
    *
    *   \note To scroll the whole display, run: SSD1306_StartScrollLeft(0x00, 0x0F)
    */
    uint8_t SSD1306_StartScrollLeft(uint8_t start, uint8_t stop);
    
    uint8_t SSD1306_StopScroll(void);
    
    /**
    *   \brief Get current width of the display.
    *
    *   \return width of the display.
    */
    uint8_t SSD1306_GetWidth(void);
    
    /**
    *   \brief Get current height of the display.
    *
    *   \return height of the display.
    */
    uint8_t SSD1306_GetHeight(void);
    
    /**
    *   \brief Return color of a pixel value in the display buffer.
    *
    *   \param x column of the display, from 0 (left) to width -1 (right).
    *   \param y row of the display, from 0 (top) to height -1 (bottom).
    *   \return pixel value in the data buffer.
    *
    *   \note This function returns the value in the display buffer,
    *   so it may not be equal to what it is actually shown on the display,
    *   which depends on when the #SSD1306_Display function was called.
    */
    uint8 SSD1306_GetPixel(int16_t x, int16_t y);
    
    uint8_t *SSD1306_GetBuffer(void);
    
    
#endif


/* [] END OF FILE */
