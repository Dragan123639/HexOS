# 🐍 HexOS

A minimal 32-bit hobby operating system written in C and Assembly.
HexOS is a toy kernel + bootable operating system written from scratch for x86.  
It features a basic command line interface (CLI), system info (like `neofetch`), and keyboard input.

✅ Written in C with no standard library  
✅ Multiboot-compliant  
✅ Runs on QEMU and real hardware  
✅ Fully open source
## 🛠️ Features

- Custom bootloader (GRUB)
- Text-based shell interface
- Built-in command(s):
  - `neofetch` – show system info
- Keyboard input handling (letters, space, backspace)
- Clean codebase (freestanding, no libc)
## 💻 Build & Run

### Requirements
- `gcc` (32-bit cross build support)
- `nasm`
- `grub-mkrescue`
- `qemu`

### Build

```bash
make
## 🧑‍💻 License

This project is licensed under the GPLv3 License.  
See [LICENSE](LICENSE) for details.
