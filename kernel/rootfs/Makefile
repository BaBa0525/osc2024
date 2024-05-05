CC = clang
CFLAGS = -Wall \
		 -ffreestanding -mcpu=cortex-a53 -mgeneral-regs-only \
		 --target=aarch64-unknown-none-elf \
		 -nostdlib

.PHONY : all
all: hello.img svc.img

%.img: %.elf
	llvm-objcopy -O binary $< $@

%.elf: hello.ld %.c
	$(CC) $(CFLAGS) -T $^ -o $@

%.elf: hello.ld %.S
	$(CC) $(CFLAGS) -T $^ -o $@

.PHONY: clean
clean:
	echo hihi
	-@rm *.img *.elf *.o



