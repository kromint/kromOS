#include <inout.h>

#define PIC1 0x20 //master pic
#define PIC1_DATA  0x21

#define PIC2 0xA0 //slave pic
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

// this part is taken from wiki.osdev.org

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_IRQ 2

//this part is taken from wiki.osdev.org

void PIC_remap(void) {
	outb(PIC1,ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA,0x20); // offset so interrupt vector 0x20-0x27 is PIC1 interrupts
	io_wait();
	outb(PIC2_DATA,0x28); //same thing but with 0x28-0x2f
	io_wait();

	outb(PIC1_DATA, 1 << CASCADE_IRQ);
	io_wait();
	outb(PIC2_DATA, CASCADE_IRQ);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// unmask both master and slave
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);

}

void PIC_disable(void) {
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}

void PIC_sendEOI(unsigned char irq) {
	if (irq>7) outb(PIC2,PIC_EOI);
	outb(PIC1,PIC_EOI);
}

void irq_set_mask(unsigned char irq) {
	unsigned short port;
	unsigned char  value;

	if(irq<8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	value = inb(port) | (1<<irq);
	outb(port,value);
}


void irq_clear_mask(unsigned char irq) {
	unsigned short port;
	unsigned char  value;

	if(irq<8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	value = inb(port) & ~(1<<irq);
	outb(port,value);
}
