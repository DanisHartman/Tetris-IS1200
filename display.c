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

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9
#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

char textbuffer[4][16];

static const uint8_t const font[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 94, 0, 0, 0, 0,
    0, 0, 4, 3, 4, 3, 0, 0,
    0, 36, 126, 36, 36, 126, 36, 0,
    0, 36, 74, 255, 82, 36, 0, 0,
    0, 70, 38, 16, 8, 100, 98, 0,
    0, 52, 74, 74, 52, 32, 80, 0,
    0, 0, 0, 4, 3, 0, 0, 0,
    0, 0, 0, 126, 129, 0, 0, 0,
    0, 0, 0, 129, 126, 0, 0, 0,
    0, 42, 28, 62, 28, 42, 0, 0,
    0, 8, 8, 62, 8, 8, 0, 0,
    0, 0, 0, 128, 96, 0, 0, 0,
    0, 8, 8, 8, 8, 8, 0, 0,
    0, 0, 0, 0, 96, 0, 0, 0,
    0, 64, 32, 16, 8, 4, 2, 0,
    0, 62, 65, 73, 65, 62, 0, 0,
    0, 0, 66, 127, 64, 0, 0, 0,
    0, 0, 98, 81, 73, 70, 0, 0,
    0, 0, 34, 73, 73, 54, 0, 0,
    0, 0, 14, 8, 127, 8, 0, 0,
    0, 0, 35, 69, 69, 57, 0, 0,
    0, 0, 62, 73, 73, 50, 0, 0,
    0, 0, 1, 97, 25, 7, 0, 0,
    0, 0, 54, 73, 73, 54, 0, 0,
    0, 0, 6, 9, 9, 126, 0, 0,
    0, 0, 0, 102, 0, 0, 0, 0,
    0, 0, 128, 102, 0, 0, 0, 0,
    0, 0, 8, 20, 34, 65, 0, 0,
    0, 0, 20, 20, 20, 20, 0, 0,
    0, 0, 65, 34, 20, 8, 0, 0,
    0, 2, 1, 81, 9, 6, 0, 0,
    0, 28, 34, 89, 89, 82, 12, 0,
    0, 0, 126, 9, 9, 126, 0, 0,
    0, 0, 127, 73, 73, 54, 0, 0,
    0, 0, 62, 65, 65, 34, 0, 0,
    0, 0, 127, 65, 65, 62, 0, 0,
    0, 0, 127, 73, 73, 65, 0, 0,
    0, 0, 127, 9, 9, 1, 0, 0,
    0, 0, 62, 65, 81, 50, 0, 0,
    0, 0, 127, 8, 8, 127, 0, 0,
    0, 0, 65, 127, 65, 0, 0, 0,
    0, 0, 32, 64, 64, 63, 0, 0,
    0, 0, 127, 8, 20, 99, 0, 0,
    0, 0, 127, 64, 64, 64, 0, 0,
    0, 127, 2, 4, 2, 127, 0, 0,
    0, 127, 6, 8, 48, 127, 0, 0,
    0, 0, 62, 65, 65, 62, 0, 0,
    0, 0, 127, 9, 9, 6, 0, 0,
    0, 0, 62, 65, 97, 126, 64, 0,
    0, 0, 127, 9, 9, 118, 0, 0,
    0, 0, 38, 73, 73, 50, 0, 0,
    0, 1, 1, 127, 1, 1, 0, 0,
    0, 0, 63, 64, 64, 63, 0, 0,
    0, 31, 32, 64, 32, 31, 0, 0,
    0, 63, 64, 48, 64, 63, 0, 0,
    0, 0, 119, 8, 8, 119, 0, 0,
    0, 3, 4, 120, 4, 3, 0, 0,
    0, 0, 113, 73, 73, 71, 0, 0,
    0, 0, 127, 65, 65, 0, 0, 0,
    0, 2, 4, 8, 16, 32, 64, 0,
    0, 0, 0, 65, 65, 127, 0, 0,
    0, 4, 2, 1, 2, 4, 0, 0,
    0, 64, 64, 64, 64, 64, 64, 0,
    0, 0, 1, 2, 4, 0, 0, 0,
    0, 0, 48, 72, 40, 120, 0, 0,
    0, 0, 127, 72, 72, 48, 0, 0,
    0, 0, 48, 72, 72, 0, 0, 0,
    0, 0, 48, 72, 72, 127, 0, 0,
    0, 0, 48, 88, 88, 16, 0, 0,
    0, 0, 126, 9, 1, 2, 0, 0,
    0, 0, 80, 152, 152, 112, 0, 0,
    0, 0, 127, 8, 8, 112, 0, 0,
    0, 0, 0, 122, 0, 0, 0, 0,
    0, 0, 64, 128, 128, 122, 0, 0,
    0, 0, 127, 16, 40, 72, 0, 0,
    0, 0, 0, 127, 0, 0, 0, 0,
    0, 120, 8, 16, 8, 112, 0, 0,
    0, 0, 120, 8, 8, 112, 0, 0,
    0, 0, 48, 72, 72, 48, 0, 0,
    0, 0, 248, 40, 40, 16, 0, 0,
    0, 0, 16, 40, 40, 248, 0, 0,
    0, 0, 112, 8, 8, 16, 0, 0,
    0, 0, 72, 84, 84, 36, 0, 0,
    0, 0, 8, 60, 72, 32, 0, 0,
    0, 0, 56, 64, 32, 120, 0, 0,
    0, 0, 56, 64, 56, 0, 0, 0,
    0, 56, 64, 32, 64, 56, 0, 0,
    0, 0, 72, 48, 48, 72, 0, 0,
    0, 0, 24, 160, 160, 120, 0, 0,
    0, 0, 100, 84, 84, 76, 0, 0,
    0, 0, 8, 28, 34, 65, 0, 0,
    0, 0, 0, 126, 0, 0, 0, 0,
    0, 0, 65, 34, 28, 8, 0, 0,
    0, 0, 4, 2, 4, 2, 0, 0,
    0, 120, 68, 66, 68, 120, 0, 0,
};

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

        spi_send_recv(0x22);    
        spi_send_recv(page);    

       
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
    for(i = 0; i < 97; i++) {
        // Draw the right border
        buffer[127 - i] |= 1;
        // Draw the left border
        buffer[511 - i] |= 128;
    }

    // Go through each page and put a line there
    for (i = 0; i < 4; i++) {
        // Draw top
        buffer[i * 128 + 4*8 - 2] = 255;
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
char textbuffer[4][16];

void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4)
        return;
    if(!s)
        return;
    
    for(i = 0; i < 16; i++)
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}

void display_update() {
    int i, j, k;
    int c;
    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);
        
        spi_send_recv(0x0);
        spi_send_recv(0x10);
        
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
        for(j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if(c & 0x80)
                continue;
            
            for(k = 0; k < 8; k++)
                spi_send_recv(font[c*8 + k]);
        }
    }
}



static void num32asc( char * s, int n ) {
    int i;
    for( i = 28; i >= 0; i -= 4 )
        *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}
