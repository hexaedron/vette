/*
 * Single-File-Header for using SH1106 OLED
 */

#ifndef _SH1106_H
#define _SH1106_H

#include "ch32v003fun.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>	   // printf
#include "font_5x8.h"


#define SH1106_128X64

/*
 * enum for font size
 */
typedef enum 
{
    fontsize_5x8   = 1,
    fontsize_10x16 = 2,
    fontsize_20x32 = 4,
	fontsize_40x64 = 8,
} font_size_t;

// event codes we use
#define  SH1106_I2C_EVENT_MASTER_MODE_SELECT ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
#define  SH1106_I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  SH1106_I2C_EVENT_MASTER_BYTE_TRANSMITTED ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

// comfortable packet size for this OLED
#define SH1106_PSZ 32

// characteristics of each type
#if !defined (SH1106_64X32) && !defined (SH1106_128X32) && !defined (SH1106_128X64)
	#error "Please define the SH1106_WXH resolution used in your application"
#endif

#ifdef SH1106_64X32
#define SH1106_W 64
#define SH1106_H 32
#define SH1106_OFFSET 32
#endif

#ifdef SH1106_128X32
#define SH1106_W 128
#define SH1106_H 32
#define SH1106_OFFSET 0
#endif

#ifdef SH1106_128X64
#define SH1106_W 128
#define SH1106_H 64
#define SH1106_OFFSET 0
#endif

#define FONT_WIDTH 5

// SH1106 I2C address
#define SH1106_I2C_ADDR 0x3c

// I2C Bus clock rate - must be lower the Logic clock rate
#define SH1106_I2C_CLKRATE 400000

// uncomment this for high-speed 36% duty cycle, otherwise 33%
#define SH1106_I2C_DUTY

// I2C Timeout count
#define TIMEOUT_MAX 400000

// uncomment this to enable IRQ-driven operation
//#define SH1106_I2C_IRQ


#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF
#define SH1106_OUTPUT_FOLLOWS_RAM 0xA4


#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10
#define SH1106_SET_SEGMENT_REMAP	0xA1 // 0 to 127

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22
#define SH1106_SET_PAGE_ADDRESS	0xB0 /* sets the page address from 0 to 7 */

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

// 0xFF --fake command to mark end
#define SH1106_TERMINATE_CMDS 0xFF

// Scrolling #defines
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define SH1106_MAX_PAGE_COUNT  8


/* choose VCC mode */
#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2
//#define vccstate SH1106_EXTERNALVCC
#define vccstate SH1106_SWITCHCAPVCC

typedef enum
{
    HORIZONTAL = 0,
    VERTICAL,
    PAGE,
    INVALID,
    END_MEMORY_ADDRESSING_MODES
} MEMORY_ADDRESSING_MODES;

// OLED initialization commands for 128x32
const uint8_t sh1106_init_array[] =
{
    SH1106_DISPLAYOFF,                    // 0xAE
    SH1106_SETDISPLAYCLOCKDIV,            // 0xD5


    // Init sequence for 128x64 OLED module
    0xF0,                                 // the suggested ratio 0xF0 XXXXXXXX
    SH1106_SETMULTIPLEX,                  // 0xA8 XXXXXXXXX
    0x3F,								   // XXXXXXXXXXX
    SH1106_OUTPUT_FOLLOWS_RAM,            // 0xA4 XXXXXXXXX
    SH1106_SETDISPLAYOFFSET,              // 0xD3 XXXXXXXXX
    0x0,                                  // no offset  XXXXXXXXX
    SH1106_SETSTARTLINE | 0x0,            // line #0
    SH1106_CHARGEPUMP,                    // 0x8D XXXXXXXXXX
    #if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXXXX
      0x10,						   // XXXXXXXXX
    #else												   // XXXXXXXXX
      0x14,						   // XXXXXXXXX
	#endif
    HORIZONTAL,                                  // 0x0 Horizontal XXXXXXXXXXX
    SH1106_SET_PAGE_ADDRESS, // start at page address 0 XXXXXXXXX
    SH1106_COMSCANDEC,					   // XXXXXXXXXX
    SH1106_SETLOWCOLUMN,				   // XXXXXXXXXX
    SH1106_SETHIGHCOLUMN,				   // XXXXXXXXXX
    SH1106_SETCOMPINS,                    // 0xDA XXXXXXXXX
    0x12,								   // XXXXXXXXXX
    SH1106_SETCONTRAST,                   // 0x81 XXXXXXXX
    #if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXX
      0x9F,						   // XXXXXXX
    #else					   							   // XXXXXXX
      0xCF,					       // XXXXXXX
    #endif
	SH1106_SET_SEGMENT_REMAP,             // 0xA1 XXXXXXXX
    SH1106_SETPRECHARGE,                  // 0xd9 XXXXXXXXX
    #if (vccstate == SH1106_EXTERNALVCC)					   // XXXXXXXXX
     0x22,						   // XXXXXXXXX
    #else												   // XXXXXXXXX
      0xF1,						   // XXXXXXXXX
    #endif
	SH1106_SETVCOMDETECT,                 // 0xDB XXXXXXXX
    0x20,								   // 0.77xVcc XXXXXXX
    SH1106_DISPLAYALLON_RESUME,           // 0xA4 XXXXXXXXXX
    SH1106_NORMALDISPLAY,                 // 0xA6  XXXXXXXXXX
	SH1106_DISPLAYON,
	
	SH1106_TERMINATE_CMDS
};

class sh1106
{
	public:

		/*
		* init I2C and GPIO
		*/
		uint8_t sh1106_i2c_init(void);

		/*
		* send OLED command byte
		*/
		uint8_t sh1106_cmd(uint8_t cmd);

		/*
		* send OLED data packet (up to 32 bytes)
		*/
		uint8_t sh1106_data(uint8_t *data, uint8_t sz);

		/*
		* set the buffer to a color
		*/
		void sh1106_setbuf(uint8_t color);

		/*
		* Send the frame buffer
		*/
		void sh1106_refresh(void);

		/*
		* plot a pixel in the buffer
		*/
		void sh1106_drawPixel(uint8_t x, uint8_t y, uint8_t color);
		/*
		* plot a pixel in the buffer
		*/
		void sh1106_xorPixel(uint8_t x, uint8_t y);

		/*
		* draw a an image from an array, directly into to the display buffer
		* the color modes allow for overwriting and even layering (sprites!)
		*/
		void sh1106_drawImage(uint8_t x, uint8_t y, const unsigned char* input, uint8_t width, uint8_t height, uint8_t color_mode);

		/*
		*  fast vert line
		*/
		void sh1106_drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);

		/*
		*  fast horiz line
		*/
		void sh1106_drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color);

		/*
		* Bresenham line draw routine swiped from Wikipedia
		*/
		void sh1106_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);

		/*
		*  draws a circle
		*/
		void sh1106_drawCircle(int16_t x, int16_t y, int16_t radius, int8_t color);

		/*
		*  draws a filled circle
		*/
		void sh1106_fillCircle(int16_t x, int16_t y, int16_t radius, int8_t color);

		/*
		*  draw a rectangle
		*/
		void sh1106_drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

		/*
		* fill a rectangle
		*/
		void sh1106_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

		/*
		* invert a rectangle in the buffer
		*/
		void sh1106_xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

		/*
		* Draw character to the display buffer
		*/
		void sh1106_drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color);

		/*
		* draw a string to the display
		*/
		void sh1106_drawstr(uint8_t x, uint8_t y, char *str, uint8_t color);

		/*
		* Draw character to the display buffer, scaled to size
		*/
		void sh1106_drawchar_sz(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, font_size_t font_size);

		/*
		* draw a string to the display buffer, scaled to size
		*/
		void sh1106_drawstr_sz(uint8_t x, uint8_t y, char *str, uint8_t color, font_size_t font_size);

		/*
		* initialize I2C and OLED
		*/
		uint8_t sh1106_init(void);

		/*
		* reset is not used for SH1106 I2C interface
		*/
		void sh1106_rst(void)
		{
		}
	
	
	private:

		/*
		* abs() helper function for line drawing
		*/
		int16_t gfx_abs(int16_t x);

		/*
		* swap() helper function for line drawing
		*/
		void gfx_swap(uint16_t *z0, uint16_t *z1);

		// the display buffer
		uint8_t sh1106_buffer[SH1106_W * SH1106_H / 8];

		void sh1106_i2c_setup(void);

		/*
		* error handler
		*/
		uint8_t sh1106_i2c_error(uint8_t err);

		/*
		* check for 32-bit event codes
		*/
		uint8_t sh1106_i2c_chk_evt(uint32_t event_mask);

		
		/*
		* low-level packet send for blocking polled operation via i2c
		*/
		uint8_t sh1106_i2c_send(uint8_t addr, uint8_t *data, uint8_t sz);

		/*
		* high-level packet send for I2C
		*/
		uint8_t sh1106_pkt_send(uint8_t *data, uint8_t sz, uint8_t cmd);
};

#endif
