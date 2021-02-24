/**
 *  \file SSD1306.c
 *
 *  \brief Main source file for SSD1306 Low Level library.
 * 
 *  \section author Author
 *
 *  Arduino Version:
 *   Written by Limor Fried/Ladyada for Adafruit Industries, with
 *   contributions from the open source community.
 *
 *  PSoC Version:
 *      Written dy Davide Marzorati.
 *
 *  \section license License
 *
 *  BSD license, all text above, and the splash screen included below,
 *  must be included in any redistribution.
 *
 */

#include "I2C_Interface.h"
#include "SSD1306.h"
#include "SSD1306_RegMap.h"
#include "SSD1306_Splash.h"
#include "string.h"

//***************************
//          MACROS
//***************************
#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32

#define SSD1306_Swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

/**
*   \brief Draw fast horizontal line.
*
*   \param x start x point.
*   \param y start y point.
*   \param w horizontal width of the line.
*   \param color color of the line.
*/
static void SSD1306_DrawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);

/**
*   \brief Draw fast vertical line.
*
*   \param x start x point.
*   \param y start y point.
*   \param h vertical height of the line.
*   \param color color of the line.
*/
static void SSD1306_DrawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);

/**
*   \brief Struct with display related settings.
*
*   This struct is used to save display settings
*   that are used throughout the module.
*/
static struct SSD1306_Settings {
    uint8_t contrast;   ///< Contrast of the display
    uint8_t vcc_state;  ///< VCC selection
    uint8_t width;      ///< Width of the display
    uint8_t height;     ///< Height of the display
    uint8_t rotation;   ///< Rotation of 
}  settings;


static uint8_t buffer[SSD1306_WIDTH*((SSD1306_HEIGHT+7) / 8)];

static void SSD1306_SendCommandList(const uint8_t *c, uint8_t n) 
{
    I2C_Peripheral_WriteRegisterMulti(SSD1306_I2C_ADDR, 0x00, n, c);
}

void SSD1306_Command(uint8_t c) 
{
    I2C_Peripheral_WriteRegister(SSD1306_I2C_ADDR, 0x00, c);
}

uint8_t SSD1306_Start(void)
{
    settings.vcc_state = SSD1306_SWITCHCAPVCC;
    settings.width = SSD1306_WIDTH;
    settings.height = SSD1306_HEIGHT;
    settings.contrast = 0x8F;
    settings.rotation = 0;
    
    //SSD1306_ClearDisplay();
    /*
    if (SSD1306_HEIGHT > 32) 
    {
        SSD1306_DrawBitmap((SSD1306_HEIGHT - splash1_width) / 2, (SSD1306_HEIGHT - splash1_height) / 2,
               splash1_data, splash1_width, splash1_height, 1);
    }
    else {
        SSD1306_DrawBitmap((WISSD1306_WIDTHDTH - splash2_width) / 2, (HEIGSSD1306_HEIGHTHT - splash2_height) / 2,
               splash2_data, splash2_width, splash2_height, 1);
    } 
    */

    // Init sequence
    
    static const uint8_t init1[] = {SSD1306_DISPLAYOFF,         // 0xAE
                                        SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
                                        0x80, // the suggested ratio 0x80
                                        SSD1306_SETMULTIPLEX}; // 0xA8
    SSD1306_SendCommandList(init1, sizeof(init1));
    SSD1306_Command(SSD1306_HEIGHT - 1);
    
    static const uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
                                        0x00,                      // no offset
                                        SSD1306_SETSTARTLINE | 0x0, // line #0
                                        SSD1306_CHARGEPUMP};        // 0x8D
    SSD1306_SendCommandList(init2, sizeof(init2));

    SSD1306_Command((settings.vcc_state == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

    static const uint8_t  init3[] = {SSD1306_MEMORYMODE, // 0x20
                                        0x00, // 0x0 act like ks0108
                                        SSD1306_SEGREMAP | 0x1,
                                        SSD1306_COMSCANDEC};
    SSD1306_SendCommandList(init3, sizeof(init3));

    uint8_t comPins = 0x02;
    
    if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 32)) 
    {
        comPins = 0x02;
        settings.contrast = 0x8F;
    }
    else if ((SSD1306_WIDTH == 128) && (SSD1306_HEIGHT == 64)) 
    {
        comPins = 0x12;
        settings.contrast = (settings.vcc_state == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
    }
    else if ((SSD1306_WIDTH == 96) && (SSD1306_HEIGHT == 16)) 
    {
        comPins = 0x2; // ada x12
        settings.contrast = (settings.vcc_state == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
    }
    else 
    {
        // Other screen varieties -- TBD
    }

    SSD1306_Command(SSD1306_SETCOMPINS);
    SSD1306_Command(comPins);
    SSD1306_Command(SSD1306_SETCONTRAST);
    SSD1306_Command(settings.contrast);

    SSD1306_Command(SSD1306_SETPRECHARGE); // 0xd9
    SSD1306_Command((settings.vcc_state == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
    static const uint8_t init5[] = {SSD1306_SETVCOMDETECT, // 0xDB
                                        0x40,
                                        SSD1306_DISPLAYALLON_RESUME, // 0xA4
                                        SSD1306_NORMALDISPLAY,       // 0xA6
                                        SSD1306_DEACTIVATE_SCROLL,
                                        SSD1306_DISPLAYON}; // Main screen turn on
    SSD1306_SendCommandList(init5, sizeof(init5));
    
    
}

void SSD1306_ClearDisplay(void)
{
    memset(buffer, 0, SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8));
}

uint8_t SSD1306_GetWidth(void)
{
    return settings.width;
}

uint8_t SSD1306_GetHeight(void)
{
    return settings.height;
}

void SSD1306_Display(void) {
    
    static const uint8_t dlist1[] = { SSD1306_PAGEADDR,
                                        0,                      // Page start address
                                        0xFF,                   // Page end (not really, but works here)
                                        SSD1306_COLUMNADDR,
                                        0}; // Column start address
    
    SSD1306_SendCommandList(dlist1, 5);
    
    SSD1306_Command(SSD1306_WIDTH - 1); // Column end address

    uint16_t count = SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8);
    //uint8_t *ptr = buffer;
    I2C_Peripheral_WriteRegisterMulti(SSD1306_I2C_ADDR, 0x40, count, buffer);
    
}

void SSD1306_InvertDisplay(uint8_t invert) {
  SSD1306_Command(invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void SSD1306_Dim(uint8_t dim) {
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  SSD1306_Command(SSD1306_SETCONTRAST);
  SSD1306_Command(dim ? 0 : settings.contrast);
}

void SSD1306_DrawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x >= 0) && (x < settings.width) && (y >= 0) && (y < settings.height)) 
    {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch (settings.rotation) {
            case 1:
              SSD1306_Swap(x, y);
              x = SSD1306_WIDTH - x - 1;
              break;
            case 2:
              x = SSD1306_WIDTH - x - 1;
              y = SSD1306_HEIGHT - y - 1;
              break;
            case 3:
              SSD1306_Swap(x, y);
              y = SSD1306_HEIGHT - y - 1;
              break;
        }
        switch (color) {
            case SSD1306_WHITE:
              buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y & 7));
              break;
            case SSD1306_BLACK:
              buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y & 7));
              break;
            case SSD1306_INVERSE:
              buffer[x + (y / 8) * SSD1306_WIDTH] ^= (1 << (y & 7));
              break;
        }
    }
}

void SSD1306_DrawFastHLine(int16_t x, int16_t y, int16_t w,
                                     uint16_t color) 
{
    uint8_t bSwap = 0;
    switch (settings.rotation) {
        case 1:
            // 90 degree rotation, swap x & y for rotation, then invert x
            bSwap = 1;
            SSD1306_Swap(x, y);
            x = SSD1306_WIDTH - x - 1;
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = SSD1306_WIDTH - x - 1;
            y = SSD1306_HEIGHT - y - 1;
            x -= (w - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation,
            // then invert y and adjust y for w (not to become h)
            bSwap = 1;
            SSD1306_Swap(x, y);
            y = SSD1306_HEIGHT - y - 1;
            y -= (w - 1);
            break;
    }

    if (bSwap)
    {
        SSD1306_DrawFastVLineInternal(x, y, w, color);
    }
    else
    {
        SSD1306_DrawFastHLineInternal(x, y, w, color);
    }
}

void SSD1306_DrawFastVLine(int16_t x, int16_t y, int16_t h,
                                     uint16_t color) {
    uint8_t bSwap = 0;
    switch (settings.rotation)
    {
        case 1:
            // 90 degree rotation, swap x & y for rotation,
            // then invert x and adjust x for h (now to become w)
            bSwap = 1;
            SSD1306_Swap(x, y);
            x = SSD1306_WIDTH - x - 1;
            x -= (h - 1);
            break;
        case 2:
            // 180 degree rotation, invert x and y, then shift y around for height.
            x = SSD1306_WIDTH - x - 1;
            y = SSD1306_HEIGHT - y - 1;
            y -= (h - 1);
            break;
        case 3:
            // 270 degree rotation, swap x & y for rotation, then invert y
            bSwap = 1;
            SSD1306_Swap(x, y);
            y = SSD1306_HEIGHT - y - 1;
            break;
    }

    if (bSwap)
    {
        SSD1306_DrawFastHLineInternal(x, y, h, color);
    }
    else
        SSD1306_DrawFastVLineInternal(x, y, h, color);
}

void SSD1306_DrawFastHLineInternal(int16_t x, int16_t y, int16_t w,
                                             uint16_t color) {

    if ((y >= 0) && (y < SSD1306_HEIGHT)) 
    {
        // Y coord in bounds?
        if (x < 0) 
        {                  // Clip left
            w += x;
            x = 0;
        }
        if ((x + w) > SSD1306_WIDTH)
        {
            // Clip right
            w = (SSD1306_WIDTH - x);
        }
        if (w > 0) 
        {
            // Proceed only if width is positive
            uint8_t *pBuf = &buffer[(y / 8) * SSD1306_WIDTH + x], mask = 1 << (y & 7);
            switch (color) {
                case SSD1306_WHITE:
                    while (w--) 
                    {
                        *pBuf++ |= mask;
                    }
                    break;
                case SSD1306_BLACK:
                    mask = ~mask;
                    while (w--) 
                    {
                      *pBuf++ &= mask;
                    }
                    break;
                case SSD1306_INVERSE:
                    while (w--) 
                    {
                        *pBuf++ ^= mask;
                    };
                break;
            }
        }
    } 
}

void SSD1306_DrawFastVLineInternal(int16_t x, int16_t __y,
                                             int16_t __h, uint16_t color) {

    if ((x >= 0) && (x < SSD1306_WIDTH)) 
    {
        // X coord in bounds?
        if (__y < 0) 
        {               // Clip top
            __h += __y;
            __y = 0;
        }
        if ((__y + __h) > SSD1306_HEIGHT) 
        {
            // Clip bottom
            __h = (SSD1306_HEIGHT - __y);
        }
        if (__h > 0) 
        {
            // Proceed only if height is now positive
            // this display doesn't need ints for coordinates,
            // use local byte registers for faster juggling
            uint8_t y = __y, h = __h;
            uint8_t *pBuf = &buffer[(y / 8) * SSD1306_WIDTH + x];

            // do the first partial byte, if necessary - this requires some masking
            uint8_t mod = (y & 7);
            if (mod) 
            {
                // mask off the high n bits we want to set
                mod = 8 - mod;
                // note - lookup table results in a nearly 10% performance
                // improvement in fill* functions
                uint8_t mask = ~(0xFF >> mod);
        
                // adjust the mask if we're not going to reach the end of this byte
                if (h < mod)
                {
                    mask &= (0XFF >> (mod - h));
                }
                switch (color) 
                {
                    case SSD1306_WHITE:
                        *pBuf |= mask;
                        break;
                    case SSD1306_BLACK:
                        *pBuf &= ~mask;
                        break;
                    case SSD1306_INVERSE:
                        *pBuf ^= mask;
                        break;
                }
                pBuf += SSD1306_WIDTH;
            }

            if (h >= mod) 
            {
                // More to go?
                h -= mod;
                // Write solid bytes while we can - effectively 8 rows at a time
                if (h >= 8) 
                {
                    if (color == SSD1306_INVERSE)
                    {
                        // separate copy of the code so we don't impact performance of
                        // black/white write version with an extra comparison per loop
                        do {
                            *pBuf ^= 0xFF; // Invert byte
                            pBuf += SSD1306_WIDTH; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    }
                    else
                    {
                        // store a local value to work with
                        uint8_t val = (color != SSD1306_BLACK) ? 255 : 0;
                        do 
                        {
                            *pBuf = val;   // Set byte
                            pBuf += SSD1306_WIDTH; // Advance pointer 8 rows
                            h -= 8;        // Subtract 8 rows from height
                        } while (h >= 8);
                    }
                }

                if (h) 
                {
                    // Do the final partial byte, if necessary
                    mod = h & 7;
                    // this time we want to mask the low bits of the byte,
                    // vs the high bits we did above
                    uint8_t mask = (1 << mod) - 1;
          
                    switch (color) 
                    {
                        case SSD1306_WHITE:
                            *pBuf |= mask;
                            break;
                        case SSD1306_BLACK:
                            *pBuf &= ~mask;
                            break;
                        case SSD1306_INVERSE:
                            *pBuf ^= mask;
                            break;
                    }
                }
            }
        } // endif positive height
    }   // endif x in bounds
}                                            
                                            
/* [] END OF FILE */
