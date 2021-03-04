# PSoC - SSD1306 Library

This repository contains PSoC Creator projects to interface a PSoC 5LP uC to SSD1306 based displays. My current setup is based on a CY8CKIT-059 by Cypress, one 128x64 OLED display and one 128x32 OLED display by Adafruit.

## Supported Displays
- [128x32 OLED Display](https://www.adafruit.com/product/931)
- [128x64 OLED Display](https://www.adafruit.com/product/938)

Currently no PSoC Creator component is available in the library, therefore the following changes in the file `SSD1306.c` need to be performed in order to correctly set up the library according to the display dimensions and I2C address:
- `#define SSD1306_I2C_ADDR [I2C address of the display: either 0x3C or 0x3D]`
- `#define SSD1306_WIDTH 128`
- `#define SSD1306_HEIGHT [Height of the display in pixels: either 32 or 64]`


## Code Documentation
Updated documentation for the project can be found at [dado93.github.io/PSoC-SSD1306](https://dado93.github.io/PSoC-SSD1306). 

