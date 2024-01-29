#include "include/sh1106.h"

#ifndef NEED_PRINTF
    #define printf(x, y) // To avoid printf() in 1106 lib
#endif

/*
* error descriptions
*/
const char *errstr[] =
{
	"not busy",
	"master mode",
	"transmit mode",
	"tx empty",
	"transmit complete",
};

// event codes we use
#define  SH1106_I2C_EVENT_MASTER_MODE_SELECT ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
#define  SH1106_I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  SH1106_I2C_EVENT_MASTER_BYTE_TRANSMITTED ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

#define FONT_WIDTH 5

// comfortable packet size for this OLED
#define SH1106_PSZ 32

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
const uint8_t init_array[] =
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


/*
 * send OLED command byte
 */
uint8_t sh1106::cmd(uint8_t cmd)
{
	pkt_send(&cmd, 1, 1);
	return 0;
}

/*
 * send OLED data packet (up to 32 bytes)
 */
uint8_t sh1106::data(uint8_t *data, uint8_t sz)
{
	pkt_send(data, sz, 0);
	return 0;
}

/*
 * set the buffer to a color
 */
void sh1106::setbuf(uint8_t color)
{
	memset(buffer, color ? 0xFF : 0x00, sizeof(buffer));
    pageUpdateMask = 0xFF;
}

/*
 * Send the frame buffer
 */
void sh1106::refresh(void)
{
	for (uint16_t page = 0; page < SH1106_MAX_PAGE_COUNT; page++)
    {
		// Update only dirty pages.
        if(!(((pageUpdateMask) >> (page)) & 0x01))
            continue;
        
        cmd(SH1106_SET_PAGE_ADDRESS + page);
        cmd(0x02); // low column start address
        cmd(0x10); // high column start address
		for (uint16_t i=0; i<(SH1106_W); i+=SH1106_PSZ) 
		{
			data(&buffer[i + (page * SH1106_W)], SH1106_PSZ);
		}
	}

    pageUpdateMask = 0;
}

/*
 * plot a pixel in the buffer
 */
void sh1106::drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	uint16_t addr;
	
	/* clip */
	if(x >= SH1106_W)
		return;
	if(y >= SH1106_H)
		return;
	
    pageUpdateMask |= (1 << (x % SH1106_MAX_PAGE_COUNT) );

	/* compute buffer address */
	addr = x + SH1106_W*(y/8);
	
	/* set/clear bit in buffer */
	if(color)
		buffer[addr] |= (1<<(y&7));
	else
		buffer[addr] &= ~(1<<(y&7));
}

/*
 * plot a pixel in the buffer
 */
void sh1106::xorPixel(uint8_t x, uint8_t y)
{
	uint16_t addr;
	
	/* clip */
	if(x >= SH1106_W)
		return;
	if(y >= SH1106_H)
		return;
	
    pageUpdateMask |= (1 << (x / SH1106_MAX_PAGE_COUNT) );

	/* compute buffer address */
	addr = x + SH1106_W*(y/8);
	
	buffer[addr] ^= (1<<(y&7));
}

/*
 * draw a an image from an array, directly into to the display buffer
 * the color modes allow for overwriting and even layering (sprites!)
 */
void sh1106::drawImage(uint8_t x, uint8_t y, const unsigned char* input, uint8_t width, uint8_t height, uint8_t color_mode) {
	uint8_t x_absolute;
	uint8_t y_absolute;
	uint8_t pixel;
	uint8_t bytes_to_draw = width / 8;
	uint16_t buffer_addr;

	for (uint8_t line = 0; line < height; line++) {
		y_absolute = y + line;
		if (y_absolute >= SH1106_H) {
			break;
		}

		// SH1106 is in vertical mode, yet we want to draw horizontally, which necessitates assembling the output bytes from the input data
		// bitmask for current pixel in vertical (output) byte
		uint8_t v_mask = 1 << (y_absolute & 7);

		for (uint8_t byte = 0; byte < bytes_to_draw; byte++) {
			uint8_t input_byte = input[byte + line * bytes_to_draw];

			for (pixel = 0; pixel < 8; pixel++) {
				x_absolute = x + 8 * (bytes_to_draw - byte) + pixel;
				if (x_absolute >= SH1106_W) {
					break;
				}
				// looking at the horizontal display, we're drawing bytes bottom to top, not left to right, hence y / 8
				buffer_addr = x_absolute + SH1106_W * (y_absolute / 8);
				// state of current pixel
				uint8_t input_pixel = input_byte & (1 << pixel);

				switch (color_mode) {
					case 0:
						// write pixels as they are
						buffer[buffer_addr] = (buffer[buffer_addr] & ~v_mask) | (input_pixel ? v_mask : 0);
						break;
					case 1:
						// write pixels after inversion
						buffer[buffer_addr] = (buffer[buffer_addr] & ~v_mask) | (!input_pixel ? v_mask : 0);
						break;
					case 2:
						// 0 clears pixel
						buffer[buffer_addr] &= input_pixel ? 0xFF : ~v_mask;
						break;
					case 3:
						// 1 sets pixel
						buffer[buffer_addr] |= input_pixel ? v_mask : 0;
						break;
					case 4:
						// 0 sets pixel
						buffer[buffer_addr] |= !input_pixel ? v_mask : 0;
						break;
					case 5:
						// 1 clears pixel
						buffer[buffer_addr] &= input_pixel ? ~v_mask : 0xFF;
						break;
				}
			}
			#if SH1106_LOG_IMAGE == 1
			printf("%02x ", input_byte);
			#endif
		}
		#if SH1106_LOG_IMAGE == 1
		printf("\n\r");
		#endif
	}
}

/*
 *  fast vert line
 */
void sh1106::drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{
	// clipping
	if((x >= SH1106_W) || (y >= SH1106_H)) return;
	if((y+h-1) >= SH1106_H) h = SH1106_H-y;
	while(h--)
	{
        drawPixel(x, y++, color);
	}
}

/*
 *  fast horiz line
 */
void sh1106::drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
	// clipping
	if((x >= SH1106_W) || (y >= SH1106_H)) return;
	if((x+w-1) >= SH1106_W)  w = SH1106_W-x;

	while (w--)
	{
        drawPixel(x++, y, color);
	}
}

/*
 * abs() helper function for line drawing
 */
int16_t sh1106::gfx_abs(int16_t x)
{
	return (x<0) ? -x : x;
}

/*
 * swap() helper function for line drawing
 */
void sh1106::gfx_swap(uint16_t *z0, uint16_t *z1)
{
	uint16_t temp = *z0;
	*z0 = *z1;
	*z1 = temp;
}

/*
 * Bresenham line draw routine swiped from Wikipedia
 */
void sh1106::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
	int16_t steep;
	int16_t deltax, deltay, error, ystep, x, y;

	/* flip sense 45deg to keep error calc in range */
	steep = (gfx_abs(y1 - y0) > gfx_abs(x1 - x0));

	if(steep)
	{
		gfx_swap(&x0, &y0);
		gfx_swap(&x1, &y1);
	}

	/* run low->high */
	if(x0 > x1)
	{
		gfx_swap(&x0, &x1);
		gfx_swap(&y0, &y1);
	}

	/* set up loop initial conditions */
	deltax = x1 - x0;
	deltay = gfx_abs(y1 - y0);
	error = deltax/2;
	y = y0;
	if(y0 < y1)
		ystep = 1;
	else
		ystep = -1;

	/* loop x */
	for(x=x0;x<=x1;x++)
	{
		/* plot point */
		if(steep)
			/* flip point & plot */
			drawPixel(y, x, color);
		else
			/* just plot */
			drawPixel(x, y, color);

		/* update error */
		error = error - deltay;

		/* update y */
		if(error < 0)
		{
			y = y + ystep;
			error = error + deltax;
		}
	}
}

/*
 *  draws a circle
 */
void sh1106::drawCircle(int16_t x, int16_t y, int16_t radius, int8_t color)
{
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        drawPixel(x - x_pos, y + y_pos, color);
        drawPixel(x + x_pos, y + y_pos, color);
        drawPixel(x + x_pos, y - y_pos, color);
        drawPixel(x - x_pos, y - y_pos, color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
              e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

/*
 *  draws a filled circle
 */
void sh1106::fillCircle(int16_t x, int16_t y, int16_t radius, int8_t color)
{
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        drawPixel(x - x_pos, y + y_pos, color);
        drawPixel(x + x_pos, y + y_pos, color);
        drawPixel(x + x_pos, y - y_pos, color);
        drawPixel(x - x_pos, y - y_pos, color);
        drawFastHLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        drawFastHLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if(e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while(x_pos <= 0);
}

/*
 *  draw a rectangle
 */
void sh1106::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
}

/*
 * fill a rectangle
 */
void sh1106::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	uint8_t m, n=y, iw = w;
	
	/* scan vertical */
	while(h--)
	{
		m=x;
		w=iw;
		/* scan horizontal */
		while(w--)
		{
			/* invert pixels */
			drawPixel(m++, n, color);
		}
		n++;
	}
}

/*
 * invert a rectangle in the buffer
 */
void sh1106::xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	uint8_t m, n=y, iw = w;
	
	/* scan vertical */
	while(h--)
	{
		m=x;
		w=iw;
		/* scan horizontal */
		while(w--)
		{
			/* invert pixels */
			xorPixel(m++, n);
		}
		n++;
	}
}

/*
 * Draw character to the display buffer
 */
void sh1106::drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color)
{
	uint16_t i, j, col;
	uint8_t d;
	
	for(i = 0; i < FONT_WIDTH; i++)
	{	
		if((chr-32) <= 95)
		{ 
			d = fontdata[(chr-32)*5+i];
		}
		else if(chr == 184) //ё
		{
			d = fontdata[(159)*5+i];
		}
		else if(chr == 168) //Ё
		{
			d = fontdata[(160)*5+i];
		}
		else 
		{
			d = fontdata[(chr-97)*5+i];
		}

		for(j = 0; j < 8; j++)
		{
			if(d&0x80)
				col = color;
			else
				col = (~color)&1;
			
			drawPixel(x + i, y - j, col);
			
			// next bit
			d <<= 1;
		}
	}
}

/*
 * draw a string to the display
 */
void sh1106::drawstr(uint8_t x, uint8_t y, char *str, uint8_t color)
{
	uint8_t c;
	
	while((c=*str++))
	{
		drawchar(x, y, c, color);
		x += FONT_WIDTH + 1;
		if(x>120)
			break;
	}
}

/*
 * Draw character to the display buffer, scaled to size
 */
void sh1106::drawchar_sz(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, font_size_t font_size)
{
    uint16_t i, j, col;
    uint8_t d;

    // Determine the font scale factor based on the font_size parameter
    uint8_t font_scale = (uint8_t)font_size;

    // Loop through each row of the font data
    for (i = 0; i < FONT_WIDTH; i++)
    {
        // Retrieve the font data for the current row
        if((chr-32) <= 95)
		{ 
			d = fontdata[(chr-32)*5+i];
		}
		else if(chr == 184) //ё
		{
			d = fontdata[(159)*5+i];
		}
		else if(chr == 168) //Ё
		{
			d = fontdata[(160)*5+i];
		}
		else 
		{
			d = fontdata[(chr-97)*5+i];
		}

        // Loop through each column of the font data
        for (j = 0; j < 8; j++)
        {
            // Determine the color to draw based on the current bit in the font data
            if (d & 0x80)
                col = color;
            else
                col = (~color) & 1;

            // Draw the pixel at the original size and scaled size using nested for-loops
            for (uint8_t k = 0; k < font_scale; k++) 
			{
                for (uint8_t l = 0; l < font_scale; l++) 
				{
                    drawPixel(x + (i * font_scale) + k, y - (j * font_scale) - l, col);
                }
            }

            // Move to the next bit in the font data
            d <<= 1;
        }
    }
}

/*
 * draw a string to the display buffer, scaled to size
 */
void sh1106::drawstr_sz(uint8_t x, uint8_t y, char *str, uint8_t color, font_size_t font_size)
{
	uint8_t c;
	
	while((c=*str++))
	{
		drawchar_sz(x, y, c, color, font_size);
		x += (FONT_WIDTH + 1) * font_size;
		if(x>128 - FONT_WIDTH * font_size)
			break;
	}
}

/*
 * initialize I2C and OLED
 */
uint8_t sh1106::init(void)
{
	i2c_init();
    
    // pulse reset
	rst();
	
	// initialize OLED
	uint8_t *cmd_list = (uint8_t *)init_array;
	while(*cmd_list != SH1106_TERMINATE_CMDS)
	{
		if(cmd(*cmd_list++))
			return 1;
	}
	
	// clear display
	setbuf(0);
	refresh();
	
	return 0;
}

void sh1106::i2c_setup(void)
{
	
	// Reset I2C1 to init all regs
	RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

	// Set I2C module clock frequency (in MHz)
  	I2C1->CTLR2 = 8;

    // Set bus clock configuration
  	I2C1->CKCFGR = (FUNCONF_SYSTEM_CORE_CLOCK / (2 * SH1106_I2C_CLKRATE)) | I2C_CKCFGR_FS ; 
	
	// Enable I2C
	I2C1->CTLR1 |= I2C_CTLR1_PE;

	// set ACK mode
	I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

/*
 * error handler
 */
uint8_t sh1106::i2c_error(uint8_t err)
{
	// report error
	printf("i2c_error - timeout waiting for %s\n\r", errstr[err]);
	
	// reset & initialize I2C
	i2c_setup();

	return 1;
}

/*
 * check for 32-bit event codes
 */
uint8_t sh1106::i2c_chk_evt(uint32_t event_mask)
{
	/* read order matters here! STAR1 before STAR2!! */
	uint32_t status = I2C1->STAR1 | (I2C1->STAR2<<16);
	return (status & event_mask) == event_mask;
}


/*
 * low-level packet send for blocking polled operation via i2c
 */
uint8_t sh1106::i2c_send(uint8_t addr, uint8_t *data, uint8_t sz)
{
	int32_t timeout;
	
	// wait for not busy
	timeout = TIMEOUT_MAX;
	while((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--));
	if(timeout==-1)
		return i2c_error(0);

	// Set START condition
	I2C1->CTLR1 |= I2C_CTLR1_START;
	
	// wait for master mode select
	timeout = TIMEOUT_MAX;
	while((!i2c_chk_evt(SH1106_I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));
	if(timeout==-1)
		return i2c_error(1);
	
	// send 7-bit address + write flag
	I2C1->DATAR = addr<<1;

	// wait for transmit condition
	timeout = TIMEOUT_MAX;
	while((!i2c_chk_evt(SH1106_I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
	if(timeout==-1)
		return i2c_error(2);

	// send data one byte at a time
	while(sz--)
	{
		// wait for TX Empty
		timeout = TIMEOUT_MAX;
		while(!(I2C1->STAR1 & I2C_STAR1_TXE) && (timeout--));
		if(timeout==-1)
			return i2c_error(3);
		
		// send command
		I2C1->DATAR = *data++;
	}

	// wait for tx complete
	timeout = TIMEOUT_MAX;
	while((!i2c_chk_evt(SH1106_I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
	if(timeout==-1)
		return i2c_error(4);

	// set STOP condition
	I2C1->CTLR1 |= I2C_CTLR1_STOP;
	
	// we're happy
	return 0;
}

/*
 * high-level packet send for I2C
 */
uint8_t sh1106::pkt_send(uint8_t *data, uint8_t sz, uint8_t cmd)
{
	uint8_t pkt[33];
	
	/* build command or data packets */
	if(cmd)
	{
		pkt[0] = 0;
		pkt[1] = *data;
	}
	else
	{
		pkt[0] = 0x40;
		memcpy(&pkt[1], data, sz);
	}
	return i2c_send(SH1106_I2C_ADDR, pkt, sz+1);
}

/*
 * init I2C and GPIO
 */
uint8_t sh1106::i2c_init(void)
{
	// Enable GPIOC and I2C
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
	RCC->APB1PCENR |= RCC_APB1Periph_I2C1;
	
	// PC1 is SDA, 10MHz Output, alt func, open-drain
	GPIOC->CFGLR &= ~(0xf<<(4*1));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF)<<(4*1);
	
	// PC2 is SCL, 10MHz Output, alt func, open-drain
	GPIOC->CFGLR &= ~(0xf<<(4*2));
	GPIOC->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF)<<(4*2);


	// load I2C regs
	i2c_setup();
	
	return 0;
}

#ifndef NEED_PRINTF
    #undef printf // To avoid printf() in 1106 lib
#endif