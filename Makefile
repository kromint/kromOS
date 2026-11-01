
CFILES = src/main.c src/limine.c src/interrupts/idt.c src/console/print.c src/interrupts/isr_handlersC.c src/inout.c src/pic.c
SFILES = src/interrupts/isr_handlers.S
OBJFILES = $(CFILES:.c=.o)
OBJFILES += $(SFILES:.S=.o)
OUTFILE = kernel.elf
ISOFILE = image.iso

CC = clang
LD = ld.lld

CFLAGS = -target x86_64-unknown-elf -ffreestanding -fno-stack-protector -fno-pic -mgeneral-regs-only -mno-red-zone -mcmodel=kernel -O2 -Wall -Wextra -I./include
CFLAGS += -g -gdwarf-4
SFLAGS = -target x86_64-unknown-elf
LDFLAGS = -nostdlib -static -z max-page-size=0x1000 -T linker.lds

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(SFLAGS) -c $< -o $@
	
$(OUTFILE): $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $(OUTFILE)

all: $(OUTFILE)

.PHONY: clean iso

clean:
	rm -f $(OUTFILE) $(OBJFILES) image.iso

$(ISOFILE): $(OUTFILE)
	cp $(OUTFILE) iso
	xorriso -as mkisofs -R -r -J -b limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        ./iso -o $(ISOFILE)
	limine bios-install $(ISOFILE)

iso: $(ISOFILE)
