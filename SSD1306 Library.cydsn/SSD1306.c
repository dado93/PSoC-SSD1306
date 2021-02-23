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
#include "I2C_Interface.h"
#include "SSD1306.h"
#include "SSD1306_RegMap.h"
#include "SSD1306_Splash.h"
#include "string.h"

#define SSD1306_I2C_ADDR 0x3C

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32

#define SSD1306_Swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

static struct SSD1306_Settings {
    uint8_t contrast;
    uint8_t vcc_state;
    uint8_t width;
    uint8_t height;
    uint8_t rotation;
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

void SSD1306_Start(void)
{
    settings.vcc_state = SSD1306_SWITCHCAPVCC;
    settings.width = SSD1306_WIDTH;
    settings.height = SSD1306_HEIGHT;
    settings.contrast = 0x8F;
    settings.rotation = 0;
    
    SSD1306_ClearDisplay();
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
    SSD1306_SendCommandList(init1, 4);
    SSD1306_Command(SSD1306_HEIGHT - 1);
    
    static const uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET, // 0xD3
                                        0x00,                      // no offset
                                        SSD1306_SETSTARTLINE | 0x0, // line #0
                                        SSD1306_CHARGEPUMP};        // 0x8D
    SSD1306_SendCommandList(init2, 4);

    SSD1306_Command((settings.vcc_state == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

    static const uint8_t  init3[] = {SSD1306_MEMORYMODE, // 0x20
                                        0x00, // 0x0 act like ks0108
                                        SSD1306_SEGREMAP | 0x1,
                                        SSD1306_COMSCANDEC};
    SSD1306_SendCommandList(init3, 4);

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
    SSD1306_SendCommandList(init5, 5);
}

void SSD1306_ClearDisplay(void)
{
    memset(buffer, 0, SSD1306_WIDTH * ((SSD1306_HEIGHT + 7) / 8));
}


void SSD1306_Display(void) {
    
    
    static const uint8_t dlist1[] = { SSD1306_PAGEADDR,
                                          0,                      // Page start address
                                          0xFF,                   // Page end (not really, but works here)
                                          SSD1306_COLUMNADDR, 0}; // Column start address
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

/* [] END OF FILE */
