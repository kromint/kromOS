#include <print.h>
#include <interrupts.h>
#include <limine.h>
#include <inout.h>
#include <pic.h>

#define RED 0x00FF0000

void hcf() {
	__asm__ volatile ("cli");
	for (;;) {
		__asm__ volatile ("hlt");
	}
}

void print_panic(const char* reason,struct InterruptFrame* frame) {
	printk("\nKERNEL PANIC!!!\n",RED);
	printk(reason,RED);
	printk("\nRIP: ",RED);
	printk_hex(frame->rip);
	printk("\nERROR: ",RED);
	printk_hex(frame->err_code);
	printk("\nsystem is now halted.\n",RED);
	hcf();
}

unsigned char scancode = 0;

void isr_exception_handler(struct InterruptFrame* frame) {
	switch (frame->interrupt_vector) {
		case 0:
			print_panic("division by zero",frame);
			break;
		case 1:
			print_panic("debug (this is not really an panic)",frame);
			break;
		case 2:
			print_panic("nmi interrupt I dont know what this is",frame);
			break;
		case 3:
			print_panic("breakpoint reached", frame);
			break;
		case 4:
			print_panic("overflow", frame);
			break;
		case 5: 
			print_panic("BOUND range exceeded",frame);
			break;
		case 6:
			print_panic("invalid opcode",frame);
			break;
		case 7:
			print_panic("No available Math CoProcessor",frame);
			break;
		case 8: 
			print_panic("CoProcessor segment overrun",frame);
			break;
		case 32:
			printk("timer interrupt",~0);
			PIC_sendEOI(0);
			break;
		case 33:
			scancode = inb(0x60);
			draw_char(scancode,~0);
			PIC_sendEOI(1);
			break;

		default:
			print_panic("this interrupt shouldn't be forwarded to exceptions",frame);
			break;
	}
}
