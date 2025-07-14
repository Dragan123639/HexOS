# Target binary
all: kernel.iso

# Link all object files into kernel.elf
kernel.elf: boot.o kernel.o string.o io.o
	ld -m elf_i386 -T linker.ld -o kernel.elf boot.o kernel.o string.o io.o
# Make ISO
kernel.iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/kernel.elf
	cp iso/boot/grub/grub.cfg isodir/boot/grub
	grub-mkrescue -o kernel.iso isodir

# Assemble bootloader
boot.o: boot.s
	nasm -f elf32 boot.s -o boot.o

# Compile kernel
kernel.o: kernel.c
	gcc -m32 -ffreestanding -c kernel.c -o kernel.o


# Compile custom string functions
string.o: string.c
	gcc -m32 -ffreestanding -c string.c -o string.o

# compile io
io.o: io.c
		gcc -m32 -ffreestanding -c io.c -o io.o

# Cleanup
clean:
	rm -f *.o *.elf *.iso
	rm -rf isodir
# Run
run:
	qemu-system-x86_64 -cdrom kernel.iso -m 512
