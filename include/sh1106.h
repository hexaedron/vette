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

class sh1106
{
	public:

		/*
		* set the buffer to a color
		*/
		void setbuf(uint8_t color);

		/*
		* Send the frame buffer
		*/
		void refresh(void);

		/*
		* plot a pixel in the buffer
		*/
		void drawPixel(uint8_t x, uint8_t y, uint8_t color);

		/*
		* plot a pixel in the buffer
		*/
		void xorPixel(uint8_t x, uint8_t y);

		/*
		* draw a an image from an array, directly into to the display buffer
		* the color modes allow for overwriting and even layering (sprites!)
		*/
		void drawImage(uint8_t x, uint8_t y, const unsigned char* input, uint8_t width, uint8_t height, uint8_t color_mode);

		/*
		*  fast vert line
		*/
		void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color);

		/*
		*  fast horiz line
		*/
		void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color);

		/*
		* Bresenham line draw routine swiped from Wikipedia
		*/
		void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);

		/*
		*  draws a circle
		*/
		void drawCircle(int16_t x, int16_t y, int16_t radius, int8_t color);

		/*
		*  draws a filled circle
		*/
		void fillCircle(int16_t x, int16_t y, int16_t radius, int8_t color);

		/*
		*  draw a rectangle
		*/
		void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

		/*
		* fill a rectangle
		*/
		void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

		/*
		* invert a rectangle in the buffer
		*/
		void xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

		/*
		* Draw character to the display buffer
		*/
		void drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color);

		/*
		* draw a string to the display
		*/
		void drawstr(uint8_t x, uint8_t y, char *str, uint8_t color);

		/*
		* Draw character to the display buffer, scaled to size
		*/
		void drawchar_sz(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, font_size_t font_size);

		/*
		* draw a string to the display buffer, scaled to size
		*/
		void drawstr_sz(uint8_t x, uint8_t y, char *str, uint8_t color, font_size_t font_size);

		/*
		* initialize I2C and OLED
		*/
		uint8_t init(void);
	
	private:

		/*
		* send OLED command byte
		*/
		uint8_t cmd(uint8_t cmd);

		/*
		* send OLED data packet (up to 32 bytes)
		*/
		uint8_t data(uint8_t *data, uint8_t sz);

		/*
		* init I2C and GPIO
		*/
		uint8_t i2c_init(void);

		/*
		* reset is not used for SH1106 I2C interface
		*/
		void rst(void)
		{
		}

		/*
		* abs() helper function for line drawing
		*/
		int16_t gfx_abs(int16_t x);

		/*
		* swap() helper function for line drawing
		*/
		void gfx_swap(uint16_t *z0, uint16_t *z1);

		void i2c_setup(void);

		/*
		* error handler
		*/
		uint8_t i2c_error(uint8_t err);

		/*
		* check for 32-bit event codes
		*/
		uint8_t i2c_chk_evt(uint32_t event_mask);
		
		/*
		* low-level packet send for blocking polled operation via i2c
		*/
		uint8_t i2c_send(uint8_t addr, uint8_t *data, uint8_t sz);

		/*
		* high-level packet send for I2C
		*/
		uint8_t pkt_send(uint8_t *data, uint8_t sz, uint8_t cmd);

		// the display buffer
		uint8_t buffer[SH1106_W * SH1106_H / 8];

		// Mask used to update only pages we need to update
		uint8_t pageUpdateMask = 0xFF;
};

#endif
