#include <limine.h>
#include <print.h>
#include <interrupts.h>
#include <inout.h>
#include <bootinfo.h>
#include <pic.h>
#define NULL ((void*)0)
#define size_t unsigned long long

void hfe(void) { 
	__asm__ volatile ("cli");
	for(;;) __asm__ volatile ("hlt");
}

static struct InterruptDescriptor idt[256] = { 0 };

void ps2_flush_output(void) {
	while(inb(0x64) & 1) {
		volatile unsigned char dummy = inb(0x60);
		(void)dummy;
	}
}

static inline unsigned long long rdmsr(unsigned int msr)
{
    unsigned int low, high;

    asm volatile("rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr));

    return ((unsigned long long)high << 32) | low;
}

static inline void wrmsr(unsigned int msr, unsigned long long value)
{
    asm volatile("wrmsr"
        :
        : "c"(msr),
          "a"((unsigned int)value),
          "d"((unsigned int)(value >> 32)));
}

void disable_apic(void)
{
    unsigned long long apic_base = rdmsr(0x1B);

    apic_base &= ~(1ULL << 11);

    wrmsr(0x1B, apic_base);
}


void kmain(void) {
	struct BootInfo* bootinfo = get_bootinfo();
	if (!(LIMINE_BASE_REVISION_SUPPORTED(bootinfo->base_revision))) hfe();

	if (bootinfo->framebuffer_request->response == NULL || bootinfo->framebuffer_request->response->framebuffer_count < 1 ) hfe();

	disable_apic();

	unsigned short kernel_code = get_cs();

	fill_InterruptDescriptor((struct InterruptDescriptor*)(&idt[3]),&isr_3,kernel_code,0,0x8E);
	fill_InterruptDescriptor(&idt[0], &isr_0, kernel_code, 0, 0x8E);
	fill_InterruptDescriptor(&idt[32], &isr_32, kernel_code, 0 ,0x8E);
	fill_InterruptDescriptor(&idt[33], &isr_33, kernel_code, 0, 0x8E);

	struct IDTR64 idtr = {
		4095,
		(unsigned long long)&idt[0]
	};

	load_idt(&idtr);

	PIC_remap();
	PIC_disable();
	irq_clear_mask(1);

	__asm__ volatile ("sti");
	

	struct limine_framebuffer *framebuffer = bootinfo->framebuffer_request->response->framebuffers[0];
	init_print(framebuffer);


	printk("\nI like kebab",~0);

	for (;;) {
	
	}

	PIC_disable();	

	hfe();
}
