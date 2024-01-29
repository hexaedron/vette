#include "include/sh1106.h"

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

/*
 * send OLED command byte
 */
uint8_t sh1106::sh1106_cmd(uint8_t cmd)
{
	sh1106_pkt_send(&cmd, 1, 1);
	return 0;
}

/*
 * send OLED data packet (up to 32 bytes)
 */
uint8_t sh1106::sh1106_data(uint8_t *data, uint8_t sz)
{
	sh1106_pkt_send(data, sz, 0);
	return 0;
}

/*
 * set the buffer to a color
 */
void sh1106::sh1106_setbuf(uint8_t color)
{
	memset(sh1106_buffer, color ? 0xFF : 0x00, sizeof(sh1106_buffer));
}

/*
 * Send the frame buffer
 */
void sh1106::sh1106_refresh(void)
{
	for (uint16_t page = 0; page < SH1106_MAX_PAGE_COUNT; page++)
    {
		sh1106_cmd(SH1106_SET_PAGE_ADDRESS + page);
        sh1106_cmd(0x02); // low column start address
        sh1106_cmd(0x10); // high column start address
		for (uint16_t i=0; i<(SH1106_W); i+=SH1106_PSZ) 
		{
			sh1106_data(&sh1106_buffer[i + (page * SH1106_W)], SH1106_PSZ);
		}
	}
}

/*
 * plot a pixel in the buffer
 */
void sh1106::sh1106_drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	uint16_t addr;
	
	/* clip */
	if(x >= SH1106_W)
		return;
	if(y >= SH1106_H)
		return;
	
	/* compute buffer address */
	addr = x + SH1106_W*(y/8);
	
	/* set/clear bit in buffer */
	if(color)
		sh1106_buffer[addr] |= (1<<(y&7));
	else
		sh1106_buffer[addr] &= ~(1<<(y&7));
}

/*
 * plot a pixel in the buffer
 */
void sh1106::sh1106_xorPixel(uint8_t x, uint8_t y)
{
	uint16_t addr;
	
	/* clip */
	if(x >= SH1106_W)
		return;
	if(y >= SH1106_H)
		return;
	
	/* compute buffer address */
	addr = x + SH1106_W*(y/8);
	
	sh1106_buffer[addr] ^= (1<<(y&7));
}

/*
 * draw a an image from an array, directly into to the display buffer
 * the color modes allow for overwriting and even layering (sprites!)
 */
void sh1106::sh1106_drawImage(uint8_t x, uint8_t y, const unsigned char* input, uint8_t width, uint8_t height, uint8_t color_mode) {
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
						sh1106_buffer[buffer_addr] = (sh1106_buffer[buffer_addr] & ~v_mask) | (input_pixel ? v_mask : 0);
						break;
					case 1:
						// write pixels after inversion
						sh1106_buffer[buffer_addr] = (sh1106_buffer[buffer_addr] & ~v_mask) | (!input_pixel ? v_mask : 0);
						break;
					case 2:
						// 0 clears pixel
						sh1106_buffer[buffer_addr] &= input_pixel ? 0xFF : ~v_mask;
						break;
					case 3:
						// 1 sets pixel
						sh1106_buffer[buffer_addr] |= input_pixel ? v_mask : 0;
						break;
					case 4:
						// 0 sets pixel
						sh1106_buffer[buffer_addr] |= !input_pixel ? v_mask : 0;
						break;
					case 5:
						// 1 clears pixel
						sh1106_buffer[buffer_addr] &= input_pixel ? ~v_mask : 0xFF;
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
void sh1106::sh1106_drawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color)
{
	// clipping
	if((x >= SH1106_W) || (y >= SH1106_H)) return;
	if((y+h-1) >= SH1106_H) h = SH1106_H-y;
	while(h--)
	{
        sh1106_drawPixel(x, y++, color);
	}
}

/*
 *  fast horiz line
 */
void sh1106::sh1106_drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color)
{
	// clipping
	if((x >= SH1106_W) || (y >= SH1106_H)) return;
	if((x+w-1) >= SH1106_W)  w = SH1106_W-x;

	while (w--)
	{
        sh1106_drawPixel(x++, y, color);
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
void sh1106::sh1106_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
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
			sh1106_drawPixel(y, x, color);
		else
			/* just plot */
			sh1106_drawPixel(x, y, color);

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
void sh1106::sh1106_drawCircle(int16_t x, int16_t y, int16_t radius, int8_t color)
{
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        sh1106_drawPixel(x - x_pos, y + y_pos, color);
        sh1106_drawPixel(x + x_pos, y + y_pos, color);
        sh1106_drawPixel(x + x_pos, y - y_pos, color);
        sh1106_drawPixel(x - x_pos, y - y_pos, color);
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
void sh1106::sh1106_fillCircle(int16_t x, int16_t y, int16_t radius, int8_t color)
{
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        sh1106_drawPixel(x - x_pos, y + y_pos, color);
        sh1106_drawPixel(x + x_pos, y + y_pos, color);
        sh1106_drawPixel(x + x_pos, y - y_pos, color);
        sh1106_drawPixel(x - x_pos, y - y_pos, color);
        sh1106_drawFastHLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        sh1106_drawFastHLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
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
void sh1106::sh1106_drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	sh1106_drawFastVLine(x, y, h, color);
	sh1106_drawFastVLine(x+w-1, y, h, color);
	sh1106_drawFastHLine(x, y, w, color);
	sh1106_drawFastHLine(x, y+h-1, w, color);
}

/*
 * fill a rectangle
 */
void sh1106::sh1106_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
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
			sh1106_drawPixel(m++, n, color);
		}
		n++;
	}
}

/*
 * invert a rectangle in the buffer
 */
void sh1106::sh1106_xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
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
			sh1106_xorPixel(m++, n);
		}
		n++;
	}
}

/*
 * Draw character to the display buffer
 */
void sh1106::sh1106_drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color)
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
			
			sh1106_drawPixel(x + i, y - j, col);
			
			// next bit
			d <<= 1;
		}
	}
}

/*
 * draw a string to the display
 */
void sh1106::sh1106_drawstr(uint8_t x, uint8_t y, char *str, uint8_t color)
{
	uint8_t c;
	
	while((c=*str++))
	{
		sh1106_drawchar(x, y, c, color);
		x += FONT_WIDTH + 1;
		if(x>120)
			break;
	}
}

/*
 * Draw character to the display buffer, scaled to size
 */
void sh1106::sh1106_drawchar_sz(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, font_size_t font_size)
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
                    sh1106_drawPixel(x + (i * font_scale) + k, y - (j * font_scale) - l, col);
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
void sh1106::sh1106_drawstr_sz(uint8_t x, uint8_t y, char *str, uint8_t color, font_size_t font_size)
{
	uint8_t c;
	
	while((c=*str++))
	{
		sh1106_drawchar_sz(x, y, c, color, font_size);
		x += (FONT_WIDTH + 1) * font_size;
		if(x>128 - FONT_WIDTH * font_size)
			break;
	}
}

/*
 * initialize I2C and OLED
 */
uint8_t sh1106::sh1106_init(void)
{
	// pulse reset
	sh1106_rst();
	
	// initialize OLED
	uint8_t *cmd_list = (uint8_t *)sh1106_init_array;
	while(*cmd_list != SH1106_TERMINATE_CMDS)
	{
		if(sh1106_cmd(*cmd_list++))
			return 1;
	}
	
	// clear display
	sh1106_setbuf(0);
	sh1106_refresh();
	
	return 0;
}

void sh1106::sh1106_i2c_setup(void)
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
uint8_t sh1106::sh1106_i2c_error(uint8_t err)
{
	// report error
	printf("sh1106_i2c_error - timeout waiting for %s\n\r", errstr[err]);
	
	// reset & initialize I2C
	sh1106_i2c_setup();

	return 1;
}

/*
 * check for 32-bit event codes
 */
uint8_t sh1106::sh1106_i2c_chk_evt(uint32_t event_mask)
{
	/* read order matters here! STAR1 before STAR2!! */
	uint32_t status = I2C1->STAR1 | (I2C1->STAR2<<16);
	return (status & event_mask) == event_mask;
}


/*
 * low-level packet send for blocking polled operation via i2c
 */
uint8_t sh1106::sh1106_i2c_send(uint8_t addr, uint8_t *data, uint8_t sz)
{
	int32_t timeout;
	
	// wait for not busy
	timeout = TIMEOUT_MAX;
	while((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--));
	if(timeout==-1)
		return sh1106_i2c_error(0);

	// Set START condition
	I2C1->CTLR1 |= I2C_CTLR1_START;
	
	// wait for master mode select
	timeout = TIMEOUT_MAX;
	while((!sh1106_i2c_chk_evt(SH1106_I2C_EVENT_MASTER_MODE_SELECT)) && (timeout--));
	if(timeout==-1)
		return sh1106_i2c_error(1);
	
	// send 7-bit address + write flag
	I2C1->DATAR = addr<<1;

	// wait for transmit condition
	timeout = TIMEOUT_MAX;
	while((!sh1106_i2c_chk_evt(SH1106_I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (timeout--));
	if(timeout==-1)
		return sh1106_i2c_error(2);

	// send data one byte at a time
	while(sz--)
	{
		// wait for TX Empty
		timeout = TIMEOUT_MAX;
		while(!(I2C1->STAR1 & I2C_STAR1_TXE) && (timeout--));
		if(timeout==-1)
			return sh1106_i2c_error(3);
		
		// send command
		I2C1->DATAR = *data++;
	}

	// wait for tx complete
	timeout = TIMEOUT_MAX;
	while((!sh1106_i2c_chk_evt(SH1106_I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (timeout--));
	if(timeout==-1)
		return sh1106_i2c_error(4);

	// set STOP condition
	I2C1->CTLR1 |= I2C_CTLR1_STOP;
	
	// we're happy
	return 0;
}

/*
 * high-level packet send for I2C
 */
uint8_t sh1106::sh1106_pkt_send(uint8_t *data, uint8_t sz, uint8_t cmd)
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
	return sh1106_i2c_send(SH1106_I2C_ADDR, pkt, sz+1);
}

/*
 * init I2C and GPIO
 */
uint8_t sh1106::sh1106_i2c_init(void)
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
	sh1106_i2c_setup();
	
	return 0;
}
