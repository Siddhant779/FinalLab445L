#ifndef _ILI9341H_
#define _ILI9341H_
#include <stdint.h>


//#include <cstdint>



#define ILI9341_NOP           0x00
#define ILI9341_SWRESET       0x01
#define ILI9341_RDDID         0x04
#define ILI9341_RDDST         0x09

#define ILI9341_SLPIN         0x10
#define ILI9341_SLPOUT        0x11
#define ILI9341_PTLON         0x12
#define ILI9341_NORON         0x13

#define ILI9341_RDMODE        0x0A
#define ILI9341_RDMADCTL      0x0B
#define ILI9341_RDPIXFMT      0x0C
#define ILI9341_RDIMGFMT      0x0D
#define ILI9341_RDSELFDIAG    0x0F

#define ILI9341_INVOFF        0x20
#define ILI9341_INVON         0x21
#define ILI9341_GAMMASET      0x26
#define ILI9341_DISPOFF       0x28
#define ILI9341_DISPON        0x29

#define ILI9341_CASET         0x2A
#define ILI9341_PASET         0x2B
#define ILI9341_RAMWR         0x2C
#define ILI9341_RAMRD         0x2E

#define ILI9341_PTLAR         0x30
#define ILI9341_MADCTL        0x36
#define ILI9341_VSCRSADD      0x37
#define ILI9341_PIXFMT        0x3A

#define ILI9341_FRMCTR1       0xB1
#define ILI9341_FRMCTR2       0xB2
#define ILI9341_FRMCTR3       0xB3
#define ILI9341_INVCTR        0xB4
#define ILI9341_DISCTRL       0xB6
#define ILI9341_DFUNCTR 0xB6 ///< Display Function Control


#define ILI9341_PWCTR1 0xC0 ///< Power Control 1
#define ILI9341_PWCTR2 0xC1 ///< Power Control 2
#define ILI9341_PWCTR3 0xC2 ///< Power Control 3
#define ILI9341_PWCTR4 0xC3 ///< Power Control 4
#define ILI9341_PWCTR5 0xC4 ///< Power Control 5
#define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
#define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2

#define ILI9341_PWCTRL1       0xC0
#define ILI9341_PWCTRL2       0xC1
#define ILI9341_PWCTRL3       0xC2
#define ILI9341_PWCTRL4       0xC3
#define ILI9341_PWCTRL5       0xC4
#define ILI9341_VMCTRL1       0xC5
#define ILI9341_VMCTRL2       0xC7

#define ILI9341_RDID1         0xDA
#define ILI9341_RDID2         0xDB
#define ILI9341_RDID3         0xDC
#define ILI9341_RDID4         0xDD

#define ILI9341_GMCTRP1       0xE0
#define ILI9341_GMCTRN1       0xE1
#define ILI9341_PWCTR6     0xFC


#define ILI9341_TFTWIDTH      320
#define ILI9341_TFTHEIGHT     240


// Color definitions
#define ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define ILI9341_NAVY 0x000F        ///<   0,   0, 123
#define ILI9341_DARKBLUE 0x03E0   ///<   0, 125,   0
#define ILI9341_MIDBLUE 0x03EF    ///<   0, 125, 123
#define ILI9341_GREEN 0x7800      ///< 123,   0,   0
#define ILI9341_LIGHTBLUE 0x7BE0       ///< 123, 125,   0
#define ILI9341_NAVYBLUE 0x001F        ///<   0,   0, 255
// #define ILI9341_UNKNOWN 0x07E0       ///<   0, 255,   0
// #define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define ILI9341_LIGHTGREEN 0xF800         ///< 255,   0,   0
#define ILI9341_SKYBLUE 0xFFE0      ///< 255, 255,   0 - SKYBLUE
#define ILI9341_TEST 0xFFFF      ///< 255, 255,   0 - SKYBLUE




/*
 *  @brief      Initializes the LCD screen
 */
//working 
extern void ILI9341_init(void);

/*
 *  @brief      Sets individual pixel on LCD
 *
 *  @param      x       x-coordinate of pixel
 *  @param      y       y-coordinate of pixel
 *  @param      rgb     6-6-6 RGB value of pixel
 */
//working 
extern void ILI9341_drawPixel(uint32_t x, uint32_t y, uint32_t rgb);

/*
 *  @brief      Sets individual pixel on LCD
 *
 *  @param      x       x-coordinate of left-most pixel
 *  @param      y       y-coordinate of line
 *  @param      l       length of line in pixels
 *  @param      rgb     6-6-6 RGB value of pixel
 */
//working
extern void ILI9341_drawHLine(uint32_t x, uint32_t y, uint32_t l, uint32_t rgb);

/*
 *  @brief      Sets individual pixel on LCD
 *
 *  @param      x       x-coordinate of line
 *  @param      y       y-coordinate of bottom-most pixel
 *  @param      l       length of line in pixels
 *  @param      rgb     6-6-6 RGB value of pixel
 */
//working
extern void ILI9341_drawVLine(uint32_t x, uint32_t y, uint32_t l, uint32_t rgb);

/*
 *  @brief       Draws a multi-colored line, optimized for efficiency
 *
 *  @param      x       x-coordinate of left-most pixel
 *  @param      y       y-coordinate of line
 *  @param      *rgb    pointer to array of RGB values (parallel to num, must be same array length)
 *  @param      *num    pointer to array of number of pixels corresponding to each RGB color
 *  @param      n       size of array
 */
extern void ILI9341_drawHLineMulticolored(uint32_t x, uint32_t y, uint32_t *rgb, uint32_t *num, uint32_t n);

extern void ILI9341_drawHLineMulticolored_indexed(uint32_t x, uint32_t y, uint16_t *rgb, uint16_t *num, uint32_t n);
extern void ILI9341_drawColors_indexed(uint32_t x, uint32_t y, int16_t *rgbIndex, uint16_t totalPixels, uint32_t n);
extern void ILI9341_drawColors(uint32_t x, uint32_t y, int32_t *rgbArr, uint16_t totalPixels);

/*
 *  @brief      Draws a single-color rectangle
 *
 *  @param      x       x-coordinate of bottom-left pixel
 *  @param      y       y-coordinate of bottom-left pixel
 *  @param      w       width of rectangle
 *  @param      h       height of rectangle
 *  @param      rgb     RGB color
 */
extern void ILI9341_fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t rgb);

/*
 * @brief       Fills entire screen with one color
 *
 * @param       rgb     RGB value
 */
extern void ILI9341_fillScreen(uint32_t rgb);

/*
 *  @brief      Sends coordinate information to LCD
 *
 *  @param      x0      x-coordinate of top left pixel
 *  @param      y0      y-coordinate of top left pixel
 *  @param      x1      x-coordinate of bottom right pixel
 *  @param      y1      y-coordinate of bottom right pixel
 */
//extern void ILI9341_setCoords(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/*
 *  @brief      Sends color information to LCD
 *
 *  @param      rgb     6-6-6 rgb color
 */
//extern void ILI9341_setColor(uint32_t rgb);

/*
 *  @brief      Command, resets the software on the LCD
 */
extern void ILI9341_softwareReset(void);

/*
 *  @brief      Command, turns display on and off
 *
 *  @param      enable    Enables or disables display
 */
//extern void ILI9341_enableDisplay(bool enable);

extern void commandList(const uint8_t *addr);

extern void ILI9341_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);

extern void setRotation(uint8_t m);

































// hardware connections
// **********ST7735 TFT and SDC*******************
// ILI9341
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground
// #define ILI9341_TFTWIDTH 240  ///< ILI9341 max TFT width
// #define ILI9341_TFTHEIGHT 320 ///< ILI9341 max TFT height

// #define ILI9341_NOP 0x00     ///< No-op register
// #define ILI9341_SWRESET 0x01 ///< Software reset register
// #define ILI9341_RDDID 0x04   ///< Read display identification information
// #define ILI9341_RDDST 0x09   ///< Read Display Status

// #define ILI9341_SLPIN 0x10  ///< Enter Sleep Mode
// #define ILI9341_SLPOUT 0x11 ///< Sleep Out
// #define ILI9341_PTLON 0x12  ///< Partial Mode ON
// #define ILI9341_NORON 0x13  ///< Normal Display Mode ON

// #define ILI9341_RDMODE 0x0A     ///< Read Display Power Mode
// #define ILI9341_RDMADCTL 0x0B   ///< Read Display MADCTL
// #define ILI9341_RDPIXFMT 0x0C   ///< Read Display Pixel Format
// #define ILI9341_RDIMGFMT 0x0D   ///< Read Display Image Format
// #define ILI9341_RDSELFDIAG 0x0F ///< Read Display Self-Diagnostic Result

// #define ILI9341_INVOFF 0x20   ///< Display Inversion OFF
// #define ILI9341_INVON 0x21    ///< Display Inversion ON
// #define ILI9341_GAMMASET 0x26 ///< Gamma Set
// #define ILI9341_DISPOFF 0x28  ///< Display OFF
// #define ILI9341_DISPON 0x29   ///< Display ON

// #define ILI9341_CASET 0x2A ///< Column Address Set
// #define ILI9341_PASET 0x2B ///< Page Address Set
// #define ILI9341_RAMWR 0x2C ///< Memory Write
// #define ILI9341_RAMRD 0x2E ///< Memory Read

// #define ILI9341_PTLAR 0x30    ///< Partial Area
// #define ILI9341_VSCRDEF 0x33  ///< Vertical Scrolling Definition
// #define ILI9341_MADCTL 0x36   ///< Memory Access Control
// #define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
// #define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set

// #define ILI9341_FRMCTR1                                                        \
//   0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
// #define ILI9341_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
// #define ILI9341_FRMCTR3                                                        \
//   0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
// #define ILI9341_INVCTR 0xB4  ///< Display Inversion Control
// #define ILI9341_DFUNCTR 0xB6 ///< Display Function Control

// #define ILI9341_PWCTR1 0xC0 ///< Power Control 1
// #define ILI9341_PWCTR2 0xC1 ///< Power Control 2
// #define ILI9341_PWCTR3 0xC2 ///< Power Control 3
// #define ILI9341_PWCTR4 0xC3 ///< Power Control 4
// #define ILI9341_PWCTR5 0xC4 ///< Power Control 5
// #define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
// #define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2

// #define ILI9341_RDID1 0xDA ///< Read ID 1
// #define ILI9341_RDID2 0xDB ///< Read ID 2
// #define ILI9341_RDID3 0xDC ///< Read ID 3
// #define ILI9341_RDID4 0xDD ///< Read ID 4

// #define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
// #define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
// //#define ILI9341_PWCTR6     0xFC

// // Color definitions
// #define ILI9341_BLACK 0x0000       ///<   0,   0,   0
// #define ILI9341_NAVY 0x000F        ///<   0,   0, 123
// #define ILI9341_DARKGREEN 0x03E0   ///<   0, 125,   0
// #define ILI9341_DARKCYAN 0x03EF    ///<   0, 125, 123
// #define ILI9341_MAROON 0x7800      ///< 123,   0,   0
// #define ILI9341_PURPLE 0x780F      ///< 123,   0, 123
// #define ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
// #define ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
// #define ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
// #define ILI9341_BLUE 0x001F        ///<   0,   0, 255
// #define ILI9341_GREEN 0x07E0       ///<   0, 255,   0
// #define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
// #define ILI9341_RED 0xF800         ///< 255,   0,   0
// #define ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
// #define ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
// #define ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
// #define ILI9341_ORANGE 0xFD20      ///< 255, 165,   0
// #define ILI9341_GREENYELLOW 0xAFE5 ///< 173, 255,  41
// #define ILI9341_PINK 0xFC18        ///< 255, 130, 198


// void ILI9341_init();

// void static setAddrWindowILI9341(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
 #endif
