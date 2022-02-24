#include <stdint.h>         
#include <pic32mx.h>      
#include <stdbool.h>    

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

static uint8_t buffer[512] = {0};
void quicksleep(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 1));
    return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

void display_image(const int x, const uint8_t *data) {
    int page, j;

    // 4 stripes across the display called pages
    // each stripe is 8 pixels high and can hold 128 bytes
    for(page = 0; page < 4; page++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);    // Command to set the page
        spi_send_recv(page);    // Set the current page

        // Don't really know how these work
        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        // j is the x axis of the current page
        for(j = 0; j < 32; j++)
            // Each byte sent to this function is a 8 pixel high column on the display
            // the lsb is the top most pixel and the msb is the most bottom pixel
            spi_send_recv(~data[page*32 + j]);
    }
}
void setPixel(x,y) {
	int temp = x/8;
	int index = (temp * 128) + y;

	//int b = index;

	int value = (1 << (x % 8 ));

	buffer[index] |= (value);
}


void fillSquare(int x, int y) {
	int i = 0;
	int j = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			setPixel(x+i,y+j);
		}
	}
}

void draw_borders(void) {
    int i;
    // Draw the borders
    // Will draw them from the bottom and up
    for(i = 0; i < 96; i++) {
        // Draw the right border
        buffer[127 - i] |= 1;
        // Draw the left border
        buffer[511 - i] |= 128;
    }

    // Go through each page and put a line there
    for (i = 0; i < 4; i++) {
        // Draw top
        buffer[i * 128 + 4*8 - 1] = 255;
        // Draw bottom
        buffer[(i + 1) * 128 - 1] = 255;
    }
}

void render(void) {
    int page, j;
    // 4 stripes across the display called pages
    // each stripe is 8 pixels high and can hold 128 bytes
    for(page = 0; page < 4; page++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);    // Command to set the page
        spi_send_recv(page);    // Set the current page

        // Don't really know how these work
        // But they have something to do with the x value from
        // the left side of the screen
        spi_send_recv(0x0);
        spi_send_recv(0x10);

        DISPLAY_CHANGE_TO_DATA_MODE;

        // j is the x axis of the current page
        for(j = 0; j < 128; j++) {
            // Each byte sent to this function is a 8 pixel high column on the display
            // the lsb is the top most pixel and the msb is the most bottom pixel
            spi_send_recv(buffer[page*128 + j]);
            // Clear the buffer after drawing it
            buffer[page*128 + j] = 0x0;
        }
    }
}



static void num32asc( char * s, int n ) {
    int i;
    for( i = 28; i >= 0; i -= 4 )
        *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}