

#include <fonts/first.h>
#include <limine.h>

#define FONT_SIZE 16

static volatile struct limine_framebuffer* fb;

static volatile unsigned int cursor_x=0;
static volatile unsigned int cursor_y=0;

void init_print(struct limine_framebuffer* framebuffer) {
	fb = framebuffer;
}

void draw_char(char chr,unsigned int color) {
	if(chr>127) return;
	if(chr=='\n') {
		cursor_x = 0;
		cursor_y++;
		return;
	}
	if((cursor_y+1)*8>fb->height) return; 

	unsigned char char_pattern[8] = {FONT_FIRST[chr*8], FONT_FIRST[chr*8+1], FONT_FIRST[chr*8+2], FONT_FIRST[chr*8+3], FONT_FIRST[chr*8+4],
	FONT_FIRST[chr*8+5], FONT_FIRST[chr*8+6], FONT_FIRST[chr*8+7]};

	volatile unsigned int* fb_ptr = fb->address;

	for(int i=0;i<8;i++) {
		for (int x=0;x<8;x++) {
			fb_ptr[x + (fb->pitch/4*i) + (cursor_x*8) + (fb->pitch/4*cursor_y)*8] = (char_pattern[i]<<x & 0x80) ? color : 0;
		} 
	}
	cursor_x++;
	if ((cursor_x+1)*8 > fb->width) { 
		cursor_x = 0;
		cursor_y++; 
	}
	return;
}

void printk(const char* str,unsigned int color) {
	for(int i = 0;str[i] != '\0';i++) {
		draw_char(str[i],color);
	}
}

void printk_hex(unsigned long long hex) {
	const char digits[] = "0123456789ABCDEF";

	printk("0x",0x00FF0000);

	for (int i=60;i>=0;i-=4) {
		draw_char(digits[(hex >> i) & 0xF],0x00FF0000);
	}
}
