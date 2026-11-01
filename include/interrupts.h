#ifndef KERNEL_INTERRUPTS_HEADER_123321
#define KERNEL_INTERRUPTS_HEADER_123321

struct InterruptDescriptor {
	unsigned short offset_1;		//offset bits 15-0
	unsigned short selector;		//code segment selector
	unsigned char  ist;			//bits 2-0 have ist offset rest are zero
	unsigned char  type_attrib;		//gate type,dpl and p fields
	unsigned short offset_2;		//offset bits 31-16
	unsigned int   offset_3;		//offset bits 63-32
	unsigned int   zero;			//reserved zero
} __attribute__((packed));


struct IDTR64 {
	unsigned short size;
	unsigned long long offset;
} __attribute__((packed));

void fill_InterruptDescriptor(struct InterruptDescriptor* IDptr,void* offset,unsigned short selector,unsigned char ist, unsigned char type_attribute);

void load_idt(struct IDTR64* idtr);
unsigned short get_cs();

void isr_0(void);
void isr_1(void);
void isr_2(void);
void isr_3(void);
void isr_4(void);
void isr_5(void);
void isr_6(void);
void isr_7(void);
void isr_8(void);
void isr_9(void);
void isr_10(void);
void isr_11(void);
void isr_12(void);
void isr_13(void);
void isr_14(void);
void isr_15(void);
void isr_16(void);
void isr_17(void);
void isr_32(void);
void isr_33(void);

struct InterruptFrame {
	unsigned long long r15,r14,r13,r12,r11,r10,r9,r8;
	unsigned long long rbp,rdi,rsi,rdx,rcx,rbx,rax;

	unsigned long long interrupt_vector,err_code;

	unsigned long long rip,cs,rflags,rsp,ss;
};

#endif
