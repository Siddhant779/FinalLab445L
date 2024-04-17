/*
 *  ILI9341 Driver for TM4C123GXL
 *  Allen Jiang
 *  319H: Intro to Embedded Systems
 *  January 2020
 *
 *
 *              ILI9341
 *
 *                       T_IRQ: --
 *                        T_DO: -
 *                       T_DIN: -
 *  SD_SCK: PB4           T_CS: -
 * SD_MISO: PB6          T_CLK: -
 * SD_MOSI: PB7           MISO: -
 *   SD_CS: PB5            LED: 3.3V
 *                         SCK: PA2
 *                        MOSI: PA5
 *                          DC: PA6
 *                       RESET: PE5
 *                          CS: PA3
 *                         GND: GND
 *                         VCC: 3.3V

 */

// //use Port A for display 
// // use port B for touchscreen and sd card 
// // PB0 - SD card cs PB1 touchscreen cs PB2 touchIRQ PB4 - clk  - on the discord 
// // MISO ---->> unconnected
// // LED  ---->>  3V
// // SCK ---->>  PA2
// // MOSI ---->> PA5
// // DC ---->>  PA6
// // RESET ---->> PA7
// // CS ----> unconnected 
// // GND ---->> GND
// // VCC ---->> 3V


#include "ILI9341.h"
#include "../inc/ST7735.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../inc/tm4c123gh6pm.h"





#define TFT_CS                  (*((volatile uint32_t *)0x40004020))
#define TFT_CS_LOW              0           // CS normally controlled by hardware
#define TFT_CS_HIGH             0x08
#define DC                      (*((volatile uint32_t *)0x40004100))
#define DC_COMMAND              0
#define DC_DATA                 0x40
#define RESET                   (*((volatile uint32_t *)0x40024000))
#define RESET_LOW               0
#define RESET_HIGH              0x20


#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left


#define DELAY 0x80

void beginSPITransaction();
void endSPITransaction();

static uint8_t rotation;           // 0 to 3

static int16_t _width = ILI9341_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
static int16_t _height = ILI9341_TFTHEIGHT;

static const uint8_t Font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F,
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30,
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5F, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x56, 0x20, 0x50,
  0x00, 0x08, 0x07, 0x03, 0x00,
  0x00, 0x1C, 0x22, 0x41, 0x00,
  0x00, 0x41, 0x22, 0x1C, 0x00,
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
  0x08, 0x08, 0x3E, 0x08, 0x08,
  0x00, 0x80, 0x70, 0x30, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x00, 0x60, 0x60, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
  0x00, 0x42, 0x7F, 0x40, 0x00, // 1
  0x72, 0x49, 0x49, 0x49, 0x46, // 2
  0x21, 0x41, 0x49, 0x4D, 0x33, // 3
  0x18, 0x14, 0x12, 0x7F, 0x10, // 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
  0x41, 0x21, 0x11, 0x09, 0x07, // 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x46, 0x49, 0x49, 0x29, 0x1E, // 9
  0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x00, 0x41, 0x22, 0x14, 0x08,
  0x02, 0x01, 0x59, 0x09, 0x06,
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A
  0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // D
  0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x3E, 0x41, 0x41, 0x51, 0x73, // G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x26, 0x49, 0x49, 0x49, 0x32, // S
  0x03, 0x01, 0x7F, 0x01, 0x03, // T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x03, 0x04, 0x78, 0x04, 0x03, // Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // Z
  0x00, 0x7F, 0x41, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x00, 0x41, 0x41, 0x41, 0x7F,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00,
  0x20, 0x54, 0x54, 0x78, 0x40, // a
  0x7F, 0x28, 0x44, 0x44, 0x38, // b
  0x38, 0x44, 0x44, 0x44, 0x28, // c
  0x38, 0x44, 0x44, 0x28, 0x7F, // d
  0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x02, // f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
  0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x20, 0x40, 0x40, 0x3D, 0x00, // j
  0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x7C, 0x04, 0x78, 0x04, 0x78, // m
  0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x38, 0x44, 0x44, 0x44, 0x38, // o
  0xFC, 0x18, 0x24, 0x24, 0x18, // p
  0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x48, 0x54, 0x54, 0x54, 0x24, // s
  0x04, 0x04, 0x3F, 0x44, 0x24, // t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // y
  0x44, 0x64, 0x54, 0x4C, 0x44, // z
  0x00, 0x08, 0x36, 0x41, 0x00,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x00, 0x41, 0x36, 0x08, 0x00,
  0x02, 0x01, 0x02, 0x04, 0x02,
  0x3C, 0x26, 0x23, 0x26, 0x3C,
  0x1E, 0xA1, 0xA1, 0x61, 0x12,
  0x3A, 0x40, 0x40, 0x20, 0x7A,
  0x38, 0x54, 0x54, 0x55, 0x59,
  0x21, 0x55, 0x55, 0x79, 0x41,
  0x21, 0x54, 0x54, 0x78, 0x41,
  0x21, 0x55, 0x54, 0x78, 0x40,
  0x20, 0x54, 0x55, 0x79, 0x40,
  0x0C, 0x1E, 0x52, 0x72, 0x12,
  0x39, 0x55, 0x55, 0x55, 0x59,
  0x39, 0x54, 0x54, 0x54, 0x59,
  0x39, 0x55, 0x54, 0x54, 0x58,
  0x00, 0x00, 0x45, 0x7C, 0x41,
  0x00, 0x02, 0x45, 0x7D, 0x42,
  0x00, 0x01, 0x45, 0x7C, 0x40,
  0xF0, 0x29, 0x24, 0x29, 0xF0,
  0xF0, 0x28, 0x25, 0x28, 0xF0,
  0x7C, 0x54, 0x55, 0x45, 0x00,
  0x20, 0x54, 0x54, 0x7C, 0x54,
  0x7C, 0x0A, 0x09, 0x7F, 0x49,
  0x32, 0x49, 0x49, 0x49, 0x32,
  0x32, 0x48, 0x48, 0x48, 0x32,
  0x32, 0x4A, 0x48, 0x48, 0x30,
  0x3A, 0x41, 0x41, 0x21, 0x7A,
  0x3A, 0x42, 0x40, 0x20, 0x78,
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,
  0x39, 0x44, 0x44, 0x44, 0x39,
  0x3D, 0x40, 0x40, 0x40, 0x3D,
  0x3C, 0x24, 0xFF, 0x24, 0x24,
  0x48, 0x7E, 0x49, 0x43, 0x66,
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
  0xFF, 0x09, 0x29, 0xF6, 0x20,
  0xC0, 0x88, 0x7E, 0x09, 0x03,
  0x20, 0x54, 0x54, 0x79, 0x41,
  0x00, 0x00, 0x44, 0x7D, 0x41,
  0x30, 0x48, 0x48, 0x4A, 0x32,
  0x38, 0x40, 0x40, 0x22, 0x7A,
  0x00, 0x7A, 0x0A, 0x0A, 0x72,
  0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28,
  0x26, 0x29, 0x29, 0x29, 0x26,
  0x30, 0x48, 0x4D, 0x40, 0x20,
  0x38, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x38,
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,
  0x2F, 0x10, 0x28, 0x34, 0xFA,
  0x00, 0x00, 0x7B, 0x00, 0x00,
  0x08, 0x14, 0x2A, 0x14, 0x22,
  0x22, 0x14, 0x2A, 0x14, 0x08,
  0xAA, 0x00, 0x55, 0x00, 0xAA,
  0xAA, 0x55, 0xAA, 0x55, 0xAA,
  0x00, 0x00, 0x00, 0xFF, 0x00,
  0x10, 0x10, 0x10, 0xFF, 0x00,
  0x14, 0x14, 0x14, 0xFF, 0x00,
  0x10, 0x10, 0xFF, 0x00, 0xFF,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x14, 0x14, 0x14, 0xFC, 0x00,
  0x14, 0x14, 0xF7, 0x00, 0xFF,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x14, 0x14, 0xF4, 0x04, 0xFC,
  0x14, 0x14, 0x17, 0x10, 0x1F,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0xF0, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFF, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x14,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x00, 0x00, 0x1F, 0x10, 0x17,
  0x00, 0x00, 0xFC, 0x04, 0xF4,
  0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4,
  0x00, 0x00, 0xFF, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xF7, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x17, 0x14,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0xF4, 0x14,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x00, 0x00, 0x1F, 0x10, 0x1F,
  0x00, 0x00, 0x00, 0x1F, 0x14,
  0x00, 0x00, 0x00, 0xFC, 0x14,
  0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF,
  0x14, 0x14, 0x14, 0xFF, 0x14,
  0x10, 0x10, 0x10, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x10,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x38, 0x44, 0x44, 0x38, 0x44,
  0x7C, 0x2A, 0x2A, 0x3E, 0x14,
  0x7E, 0x02, 0x02, 0x06, 0x06,
  0x02, 0x7E, 0x02, 0x7E, 0x02,
  0x63, 0x55, 0x49, 0x41, 0x63,
  0x38, 0x44, 0x44, 0x3C, 0x04,
  0x40, 0x7E, 0x20, 0x1E, 0x20,
  0x06, 0x02, 0x7E, 0x02, 0x02,
  0x99, 0xA5, 0xE7, 0xA5, 0x99,
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,
  0x4C, 0x72, 0x01, 0x72, 0x4C,
  0x30, 0x4A, 0x4D, 0x4D, 0x30,
  0x30, 0x48, 0x78, 0x48, 0x30,
  0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00,
  0x7E, 0x01, 0x01, 0x01, 0x7E,
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x40, 0x51, 0x4A, 0x44, 0x40,
  0x40, 0x44, 0x4A, 0x51, 0x40,
  0x00, 0x00, 0xFF, 0x01, 0x03,
  0xE0, 0x80, 0xFF, 0x00, 0x00,
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36,
  0x06, 0x0F, 0x09, 0x0F, 0x06,
  0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00,
  0x30, 0x40, 0xFF, 0x01, 0x01,
  0x00, 0x1F, 0x01, 0x01, 0x1E,
  0x00, 0x19, 0x1D, 0x17, 0x12,
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x00,
};

static const uint8_t cmd_ili9341[] = {
    25,

    ILI9341_SWRESET, DELAY, 150, // Software reset

    0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
    0xCF, 3, 0x00, 0xC1, 0x30,
    0xEF, 3, 0x03, 0x80, 0x02,
    0xE8, 3, 0x85, 0x00, 0x78,
    0xEA, 2, 0x00, 0x00,
    0xCF, 3, 0x00, 0XC1, 0X30,
    0xED, 4, 0x64, 0x03, 0x12, 0x81,
    0xF7, 1, 0x20,

    ILI9341_PWCTRL1, 1, 0x23, // Power control
    ILI9341_PWCTRL2, 1, 0x10, // Power control
    ILI9341_VMCTRL1, 2, 0x3E, 0x28, //0x3e, 0x28, // VCM control
    ILI9341_VMCTRL2, 1, 0x86, // VCM control2
    ILI9341_MADCTL, 1, 0x48, // Memory Access Control
    ILI9341_PIXFMT, 1, 0x55, //16-bit    0x66 24-bit
    ILI9341_INVOFF, 0,
    ILI9341_FRMCTR1, 2, 0x00, 0x10, // FrameRate Control 119Hz
    ILI9341_DISCTRL, 4, 0x08, 0x82, 0x27, 0x00, // Display Function Control
    0x30, 4, 0x00, 0x00, 0x01, 0x3F,
    0xF2, 1, 0x00,
    ILI9341_GAMMASET, 1, 0x01, // Gamma curve selected
    ILI9341_GMCTRP1, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
        0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
    ILI9341_GMCTRN1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
        0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
    ILI9341_SLPOUT, DELAY, 200,  // Normal display on, 10ms delay
    ILI9341_DISPON, DELAY, 100
};

static void writeCommand(uint8_t c) {
                                        // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  TFT_CS = TFT_CS_LOW;
  DC = DC_COMMAND;
  SSI0_DR_R = c;                        // data out
                                        // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
}

static void writeData(uint8_t d) {
  while((SSI0_SR_R&SSI_SR_TNF)==0){};   // wait until transmit FIFO not full
  DC = DC_DATA;
  SSI0_DR_R = d;                        // data out
}


void static ILI9341_setColor(uint32_t rgb) {
    /* // 18-bit, 6-6-6
    uint8_t r = (rgb & 0x3F0000) >> 16;
    uint8_t g = (rgb & 0x003F00) >> 8;
    uint8_t b = (rgb & 0x00003F);

    writeData(b << 2);
    writeData(g << 2);
    writeData(r << 2);

    */

      // 16-bit, 5-6-5
    //    uint8_t r = (rgb & 0xF80000) >> 19;
    //    uint8_t g = (rgb & 0x00FC00) >> 10;
    //    uint8_t b = (rgb & 0x0000F8) >> 3;
    uint8_t r = (rgb & 0xF80000) >> 19;
    uint8_t g = (rgb & 0x00FC00) >> 10;
    uint8_t b = (rgb & 0x0000F8) >> 3;

    writeData((r << 3) | (g >> 3));
    writeData((g << 5) | (b));
}

void static ILI9341_setCoords(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writeCommand(ILI9341_CASET); // Column addr set
    writeData(y0>>8);
    writeData(y0);     // YSTART
    writeData(y1>>8);
    writeData(y1);     // YEND

    writeCommand(ILI9341_PASET); // Row addr set
    writeData(x0>>8);
    writeData(x0);     // XSTART
    writeData(x1>>8);
    writeData(x1);     // XEND

    writeCommand(ILI9341_RAMWR);    // write to RAM
}

static void deselect(void) {
    // wait until SSI0 not busy/transmit FIFO empty
  while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
  TFT_CS = TFT_CS_HIGH;
}



//------------------------------------------------------------------
void ILI9341_init() {
    //GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);

    SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
    SYSCTL_RCGCGPIO_R |= 0x01; // activate port A
    while((SYSCTL_PRGPIO_R&0x01)==0){}; // allow time for clock to start

    // toggle RST low to reset; CS low so it'll listen to us
    // SSI0Fss is temporarily used as GPIO
    GPIO_PORTA_DIR_R |= 0xC8;             // make PA3,6,7 out
    GPIO_PORTA_AFSEL_R &= ~0xC8;          // disable alt funct on PA3,6,7
    GPIO_PORTA_DEN_R |= 0xC8;             // enable digital I/O on PA3,6,7
                                        // configure PA3,6,7 as GPIO
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0x00FF0FFF)+0x00000000;
    GPIO_PORTA_AMSEL_R &= ~0xC8;          // disable analog functionality on PA3,6,7
    //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);
    //sleep(10);
    //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
    //sleep(20);
    //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5);
    //sleep(150);
    TFT_CS = TFT_CS_LOW;
    //RESET = RESET_HIGH;
    GPIO_PORTA_DATA_R |= 0x80; 
    Delay1ms(500);
    //RESET = RESET_LOW;
    GPIO_PORTA_DATA_R &= ~0x80; 
    Delay1ms(500);
    RESET = RESET_HIGH;
    GPIO_PORTA_DATA_R |= 0x80; 
    Delay1ms(500);

    // initialize SSI0
    GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2,3,5
    GPIO_PORTA_DEN_R |= 0x2C;             // enable digital I/O on PA2,3,5
                                        // configure PA2,3,5 as SSI
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;
    GPIO_PORTA_AMSEL_R &= ~0x2C;          // disable analog functionality on PA2,3,5
    SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
    SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode
                                        // configure for system clock/PLL baud clock source
    SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;

                                        // SysClk/(CPSDVSR*(1+SCR))
                                        // 80/(10*(1+0)) = 8 MHz (slower than 4 MHz)
    //SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+16;
    //SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+10;

    SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+4; // must be even number
//    SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+6; // must be even number
    SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (8 Mbps data rate)
                  SSI_CR0_SPH |         // SPH = 0
                  SSI_CR0_SPO);         // SPO = 0
                                        // FRF = Freescale format
    SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
                                        // DSS = 8-bit data
    SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;
    SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI

    commandList(cmd_ili9341);

    ILI9341_fillScreen(0);                 // set screen to black
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in ROM byte array.
void commandList(const uint8_t *addr) {

  uint8_t numCommands, numArgs;
  uint16_t ms;

  numCommands = *(addr++);               // Number of commands to follow
  while(numCommands--) {                 // For each command...
    writeCommand(*(addr++));             //   Read, issue command
    numArgs  = *(addr++);                //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while(numArgs--) {                   //   For each argument...
      writeData(*(addr++));              //     Read, issue argument
    }

    if(ms) {
      ms = *(addr++);             // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      Delay1ms(ms);
    }
  }
}

//  Bottom-left corner is (0, 0). Up = +y, Right = +x
void ILI9341_drawPixel(uint32_t x, uint32_t y, uint32_t rgb) {
    if((x > _width) || (y > _height)) return;

    beginSPITransaction();

    ILI9341_setCoords(x,y,x,y);

    ILI9341_setColor(rgb);

    deselect();
    endSPITransaction();
}

//  Coordinate is left-most pixel of line
void ILI9341_drawHLine(uint32_t x, uint32_t y, uint32_t l, uint32_t rgb) {
    if((x > _width) || (y > _height)) return;
    if(x + l > _width) l = _width-x;

    beginSPITransaction();

    ILI9341_setCoords(x,y,x+l,y);

    for(uint32_t i = 0; i < l; i++) {
        ILI9341_setColor(rgb);
    }
    deselect();
    endSPITransaction();
}

//  Coordinate is bottom pixel of line
void ILI9341_drawVLine(uint32_t x, uint32_t y, uint32_t l, uint32_t rgb) {
    if((x > _width) || (y > _height)) return;
    if(y + l > _width) l = _height-y;

    beginSPITransaction();

    ILI9341_setCoords(x,y,x,y+l);

    for(uint16_t i = 0; i < l; i++) {
        ILI9341_setColor(rgb);
    }
    deselect();
    endSPITransaction();
}

void ILI9341_drawColors(uint32_t x, uint32_t y, int32_t *rgbArr, uint16_t totalPixels) {
    if((x > _width) || (y > _height)) return;

    if(x + totalPixels > _width) totalPixels = _width-x;

    beginSPITransaction();

    ILI9341_setCoords(x,y,x+totalPixels,y);

    for(uint32_t i = 0; i < totalPixels; i++) {
        ILI9341_setColor(rgbArr[i]);
    }

    deselect();
}

//  Coordinate is left-most pixel of line
void ILI9341_drawHLineMulticolored(uint32_t x, uint32_t y, uint32_t *rgb, uint32_t *num, uint32_t n) {
    if((x > _width) || (y > _height)) return;

    uint32_t l = 0;
    for(int i = 0; i < n; i++) {
        l += num[i];
    }

    if(x + l >= _width) l = _width-x-1;

    beginSPITransaction();

    ILI9341_setCoords(x,y,x+l,y);

    uint32_t j;
    uint32_t loops;
    for(int i = n-1; i >= 0; i--) {
        loops = num[i];
        for(j = 0; j < loops && l-- > 0; j++) {
            ILI9341_setColor(rgb[i]);
        }
    }
    deselect();
}

//  coordinate is bottom-left of rectangle
void ILI9341_fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t rgb) {
    if((x > _width) || (y > _height)) return;

    if(x + w >= _width) w = _width - x;
    if(y + h >= _height) h = _height - y;

    beginSPITransaction();

    ILI9341_setCoords(x, y, x+w, y+h);

    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            ILI9341_setColor(rgb);
        }
    }
    deselect();
}

void ILI9341_fillScreen(uint32_t rgb) {
    ILI9341_fillRect(0, 0, _width, _height, rgb);
}

void beginSPITransaction(void) {
    SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
}

void endSPITransaction(void) {
    while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
    SYSCTL_RCGCSSI_R &= ~0x01;  // deactivate SSI0
}



void ILI9341_DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // vertical column of pixels of character in font
  int32_t i, j;
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = Font[(c*5)+i];
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          //ILI9341_drawPixel(x+i, y+j, textColor);
          ILI9341_drawPixel(y-j, x-i, textColor);
          //ST7735_DrawPixel(x+i, y+j, textColor);
        else {  // big size
          ILI9341_fillRect(y-(j*size), x-(i*size), size, size, textColor);
          //ST7735_FillRect(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1) // default size
          ILI9341_drawPixel(y-j, x-i, bgColor);
          //ST7735_DrawPixel(x+i, y+j, bgColor);
        else {  // big size
          ILI9341_fillRect(y-j*size, x-i*size, size, size, bgColor);
          //ST7735_FillRect(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line >>= 1;
    }
  }
}

void setRotation(uint8_t m) {
  writeCommand(ILI9341_MADCTL);
  rotation = m % 4; // can't be higher than 3
  //
  switch (rotation) {
  case 0:
    m = (MADCTL_MX | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    Delay1ms(100);
    writeData(MADCTL_MX | MADCTL_BGR);
    break;
  case 1:
    m = (MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    writeData(MADCTL_MV | MADCTL_BGR);
    Delay1ms(100);
    break;
  case 2:
    m = (MADCTL_MY | MADCTL_BGR);
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    writeData(MADCTL_MY | MADCTL_BGR);
    Delay1ms(100);
    break;
  case 3:
    m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    writeData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
    Delay1ms(100);
    break;
  }
}















// #include "ILI9341.h"

// #include <stdio.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include "../inc/tm4c123gh6pm.h"
// //use Port A for display 
// // use port B for touchscreen and sd card 
// // PB0 - SD card cs PB1 touchscreen cs PB2 touchIRQ PB4 - clk  - on the discord 
// // MISO ---->> unconnected
// // LED  ---->>  3V
// // SCK ---->>  PA2
// // MOSI ---->> PA5
// // DC ---->>  PA6
// // RESET ---->> PA7
// // CS ----> unconnected 
// // GND ---->> GND
// // VCC ---->> 3V

// //USE THE ADAFRUIT SSD1306 library 

// #define SSI_CR0_SCR_M           0x0000FF00  // SSI Serial Clock Rate
// #define SSI_CR0_SPH             0x00000080  // SSI Serial Clock Phase
// #define SSI_CR0_SPO             0x00000040  // SSI Serial Clock Polarity
// #define SSI_CR0_FRF_M           0x00000030  // SSI Frame Format Select
// #define SSI_CR0_FRF_MOTO        0x00000000  // Freescale SPI Frame Format
// #define SSI_CR0_DSS_M           0x0000000F  // SSI Data Size Select
// #define SSI_CR0_DSS_8           0x00000007  // 8-bit data
// #define SSI_CR1_MS              0x00000004  // SSI Master/Slave Select
// #define SSI_CR1_SSE             0x00000002  // SSI Synchronous Serial Port
//                                             // Enable
// #define SSI_SR_BSY              0x00000010  // SSI Busy Bit
// #define SSI_SR_TNF              0x00000002  // SSI Transmit FIFO Not Full
// #define SSI_CPSR_CPSDVSR_M      0x000000FF  // SSI Clock Prescale Divisor
// #define SSI_CC_CS_M             0x0000000F  // SSI Baud Clock Source
// #define SSI_CC_CS_SYSPLL        0x00000000  // Either the system clock (if the
//                                             // PLL bypass is in effect) or the
//                                             // PLL output (default)
// #define SYSCTL_RCGC1_SSI0       0x00000010  // SSI0 Clock Gating Control
// #define SYSCTL_RCGC2_GPIOA      0x00000001  // port A Clock Gating Control

// #define TFT_CS                  (*((volatile uint32_t *)0x40004020))
// #define TFT_CS_LOW              0           // CS normally controlled by hardware
// #define TFT_CS_HIGH             0x08
// #define DC                      (*((volatile uint32_t *)0x40004100))
// #define DC_COMMAND              0
// #define DC_DATA                 0x40
// #define RESET                   (*((volatile uint32_t *)0x40004200))
// #define RESET_LOW               0
// #define RESET_HIGH              0x80

// #define DELAY 0x80

// static uint8_t ColStart, RowStart; // some displays need this changed

// static int16_t _width = ILI9341_TFTWIDTH;   // this could probably be a constant, except it is used in Adafruit_GFX and depends on image rotation
// static int16_t _height = ILI9341_TFTHEIGHT;

// static const uint8_t initcmd[] = {
//   0xEF, 3, 0x03, 0x80, 0x02,
//   0xCF, 3, 0x00, 0xC1, 0x30,
//   0xED, 4, 0x64, 0x03, 0x12, 0x81,
//   0xE8, 3, 0x85, 0x00, 0x78,
//   0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
//   0xF7, 1, 0x20,
//   0xEA, 2, 0x00, 0x00,
//   ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
//   ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
//   ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
//   ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
//   ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
//   ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
//   ILI9341_PIXFMT  , 1, 0x55,
//   ILI9341_FRMCTR1 , 2, 0x00, 0x18,
//   ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
//   0xF2, 1, 0x00,                         // 3Gamma Function Disable
//   ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
//   ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
//     0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
//   ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
//     0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
//   ILI9341_SLPOUT  , 0x80,                // Exit Sleep
//   ILI9341_DISPON  , 0x80,                // Display on
//   0x00                                   // End of list
// };

// void static writecommand(uint8_t c) {
//                                         // wait until SSI0 not busy/transmit FIFO empty
//   while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
//   TFT_CS = TFT_CS_LOW;
//   DC = DC_COMMAND;
//   SSI0_DR_R = c;                        // data out
//                                         // wait until SSI0 not busy/transmit FIFO empty
//   while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
// }

// // Subroutine to wait 1 msec
// // Inputs: None
// // Outputs: None
// // Notes: ...
// void static Delay1ms(uint32_t n){uint32_t volatile time;
//   while(n){
//     time = 72724*2/91;  // 1msec, tuned at 80 MHz
//     while(time){
//       time--;
//     }
//     n--;
//   }
// }

// // Companion code to the above tables.  Reads and issues
// // a series of LCD commands stored in ROM byte array.

// void static writedata(uint8_t c) {
//   while((SSI0_SR_R&SSI_SR_TNF)==0){};   // wait until transmit FIFO not full
//   DC = DC_DATA;
//   SSI0_DR_R = c;                        // data out
// }

// void static commandList(const uint8_t *addr) {

//   uint8_t numCommands, numArgs;
//   uint16_t ms;

//   numCommands = *(addr++);               // Number of commands to follow
//   while(numCommands--) {                 // For each command...
//     writecommand(*(addr++));             //   Read, issue command
//     numArgs  = *(addr++);                //   Number of args to follow
//     ms       = numArgs & DELAY;          //   If hibit set, delay follows args
//     numArgs &= ~DELAY;                   //   Mask out delay bit
//     while(numArgs--) {                   //   For each argument...
//       writedata(*(addr++));              //     Read, issue argument
//     }

//     if(ms) {
//       ms = *(addr++);             // Read post-command delay time (ms)
//       if(ms == 255) ms = 500;     // If 255, delay for 500 ms
//       Delay1ms(ms);
//     }
//   }
// }


// void static deselect(void) {
//                                         // wait until SSI0 not busy/transmit FIFO empty
//   while((SSI0_SR_R&SSI_SR_BSY)==SSI_SR_BSY){};
//   TFT_CS = TFT_CS_HIGH;    
// }




// void ILI9341_init() {
//     volatile uint32_t delay;
//   ColStart  = RowStart = 0; // May be overridden in init func

//   SYSCTL_RCGCSSI_R |= 0x01;  // activate SSI0
//   SYSCTL_RCGCGPIO_R |= 0x01; // activate port A
//   while((SYSCTL_PRGPIO_R&0x01)==0){}; // allow time for clock to start

//   // toggle RST low to reset; CS low so it'll listen to us
//   // SSI0Fss is temporarily used as GPIO
//   GPIO_PORTA_DIR_R |= 0xC8;             // make PA3,6,7 out
//   GPIO_PORTA_AFSEL_R &= ~0xC8;          // disable alt funct on PA3,6,7
//   GPIO_PORTA_DEN_R |= 0xC8;             // enable digital I/O on PA3,6,7
//                                         // configure PA3,6,7 as GPIO
//   GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0x00FF0FFF)+0x00000000;
//   GPIO_PORTA_AMSEL_R &= ~0xC8;          // disable analog functionality on PA3,6,7
//   TFT_CS = TFT_CS_LOW;
//   RESET = RESET_HIGH;
//   Delay1ms(500);
//   RESET = RESET_LOW;
//   Delay1ms(500);
//   RESET = RESET_HIGH;
//   Delay1ms(500);

//   // initialize SSI0
//   GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2,3,5
//   GPIO_PORTA_DEN_R |= 0x2C;             // enable digital I/O on PA2,3,5
//                                         // configure PA2,3,5 as SSI
//   GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;
//   GPIO_PORTA_AMSEL_R &= ~0x2C;          // disable analog functionality on PA2,3,5
//   SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
//   SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode
//                                         // configure for system clock/PLL baud clock source
//   SSI0_CC_R = (SSI0_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_SYSPLL;
// //                                        // clock divider for 3.125 MHz SSIClk (50 MHz PIOSC/16)
// //  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+16;
//                                         // clock divider for 8 MHz SSIClk (80 MHz PLL/24)
//                                         // SysClk/(CPSDVSR*(1+SCR))
//                                         // 80/(10*(1+0)) = 8 MHz (slower than 4 MHz)
//   SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+10; // must be even number
//   SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (8 Mbps data rate)
//                   SSI_CR0_SPH |         // SPH = 0
//                   SSI_CR0_SPO);         // SPO = 0
//                                         // FRF = Freescale format
//   SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
//                                         // DSS = 8-bit data
//   SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_;
//   SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI

//   commandList(initcmd);
// }

// void static setAddrWindowILI9341(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
//      if ((x0 > x1) || (x1 > ILI9341_TFTWIDTH) ||
//       (y0 > y1) || (y1 > ILI9341_TFTHEIGHT)) 
//   { 
//     // out of range
//     return ILI9341_ERROR;
//   }  

//   // set column
//   ILI9341_TransmitCmmd(ILI9341_CASET);
//   // set column -> set column
//   ILI9341_Transmit32bitData(((uint32_t) xs << 16) | xe);
//   // set page
//   ILI9341_TransmitCmmd(ILI9341_PASET);
//   // set page -> high byte first
//   ILI9341_Transmit32bitData(((uint32_t) ys << 16) | ye);
//   // success
// }
// void ILI9341_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
//   uint8_t hi = color >> 8, lo = color;

//   // rudimentary clipping (drawChar w/big text requires this)
//   if((x >= _width) || (y >= _height)) return;
//   if((x + w - 1) >= _width)  w = _width  - x;
//   if((y + h - 1) >= _height) h = _height - y;

//   setAddrWindowILI9341(x, y, x+w-1, y+h-1);

//   for(y=h; y>0; y--) {
//     for(x=w; x>0; x--) {
//       writedata(hi);
//       writedata(lo);
//     }
//   }

//   deselect();
// }