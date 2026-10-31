

#ifndef KERNEL_PRINT_123321
#define KERNEL_PRINT_123321

#include <limine.h>

void init_print(struct limine_framebuffer* framebuffer);

void draw_char(char chr, unsigned int color);

void printk(const char* str, unsigned int color);

void printk_hex(unsigned long long hex);

#endif
