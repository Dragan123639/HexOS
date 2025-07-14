#include <stdint.h>
#include "string.h"
#include "io.h"
#define VIDEO_MEMORY ((char*) 0xb8000)
#define WIDTH 80
#define HEIGHT 25

void print_string(const char* str);
void print_char(char c);
uint8_t read_scan_code() {
    return inb(0x60);
}


volatile int key_ready = 0;
volatile char key_buffer = 0;

int cursor = 0;
int input_start = 0;

void clear_screen() {
    volatile char* video = (volatile char*) 0xB8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = ' ';
        video[i+1] = 0x0F;
    }
}

extern void outb(unsigned short port, unsigned char val);

void keyboard_handler() {
    char key = read_scan_code(); // however you read input
    key_buffer = key;
    key_ready = 1;
}


// VGA hardware cursor control
void cpuid(int code, uint32_t* a, uint32_t* d) {
    __asm__ volatile("cpuid" : "=a"(*a), "=d"(*d) : "a"(code) : "ecx", "ebx");
}

void get_cpu_vendor(char* vendor) {
    uint32_t ebx, ecx, edx;
    __asm__ volatile("cpuid"
                     : "=b"(ebx), "=d"(edx), "=c"(ecx)
                     : "a"(0));

    *(uint32_t*)&vendor[0] = ebx;
    *(uint32_t*)&vendor[4] = edx;
    *(uint32_t*)&vendor[8] = ecx;
    vendor[12] = 0;
}

void neofetch() {
    print_string("\n");
    print_string("       /^\\/^\\\n");
    print_string("     _|__|  O|\n");
    print_string("\\/     /~     \\_/ \\\n");
    print_string(" \\____|__________/  \\\n");
    print_string("        \\_______      \\\n");
    print_string("                `\\     \\                 \\\n");
    print_string("                  |     |                  \\\n");
    print_string("                 /      /                    \\\n");
    print_string("                /     /                       \\\n");
    print_string("              /      /                         \\ \\\n");
    print_string("             /     /                            \\  \\\n");
    print_string("           /     /             _----_            \\   \\\n");
    print_string("          /     /           _-~      ~-_         |   |\n");
    print_string("         (      (        _-~    _--_    ~-_     _/   |\n");
    print_string("          \\      ~-____-~    _-~    ~-_    ~-_-~    /\n");
    print_string("            ~-_           _-~          ~-_       _-~\n");
    print_string("               ~--______-~                ~-___-~\n");
    print_string("\n");

    char vendor[13];
    get_cpu_vendor(vendor);

    print_string("OS: HexOS v0.0.2\n");
    print_string("Arch: x86\n");
    print_string("CPU Vendor: ");
    print_string(vendor);
    print_char('\n');
    print_string("Shell: HexShell 🐍\n");
    print_string("RAM: 64MB (assumed)\n");
}


void move_cursor(int pos) {
    outb(0x3D4, 14);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0xFF);
}

int keyboard_has_data() {
    return inb(0x64) & 1;
}

// Screen control
void newline() {
    cursor = ((cursor / (WIDTH * 2)) + 1) * (WIDTH * 2);
    if (cursor >= WIDTH * HEIGHT * 2) {
        cursor = 0;
    }
    move_cursor(cursor / 2);
}

char get_char() {
    while (!key_ready); // wait until key is pressed
    key_ready = 0;
    return key_buffer;
}


void print_char(char c) {
    // Output a character to screen
    // Example if using VGA text mode:
    volatile char* video = (volatile char*) 0xB8000;
    static int cursor = 0;
    video[cursor++] = c;
    video[cursor++] = 0x0F;
}

void print_string(const char* str) {
    while (*str) {
        print_char(*str++);
    }
}

void backspace() {
    if (cursor > input_start) {
        cursor -= 2;
        VIDEO_MEMORY[cursor] = ' ';
        VIDEO_MEMORY[cursor + 1] = 0x07;
        move_cursor(cursor / 2);
    }
}

void trim_input(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            break;
        }
    }
}

void kernel_main() {
    print_string("HexOS v0.0.2\n");

    char input[128];
    int input_index = 0;

    while (1) {
        print_string("HexOS> ");
        input_index = 0;

        while (1) {
            // Wait for keypress
            if (!keyboard_has_data()) continue;
            uint8_t scancode = inb(0x60);

            if (scancode & 0x80) continue; // ignore key releases

            // Enter key
            if (scancode == 0x1C) {
                input[input_index] = '\0';
                print_char('\n');
                break;
            }

            // Backspace
            else if (scancode == 0x0E && input_index > 0) {
                input_index--;
                print_char('\b');
                print_char(' ');
                print_char('\b');
            }

            // Spacebar
            else if (scancode == 0x39 && input_index < 127) {
                input[input_index++] = ' ';
                print_char(' ');
            }

            // Letter mapping (a-z)
            else {
                char c = '?';
                if (scancode == 0x1E) c = 'a';
                if (scancode == 0x30) c = 'b';
                if (scancode == 0x2E) c = 'c';
                if (scancode == 0x20) c = 'd';
                if (scancode == 0x12) c = 'e';
                if (scancode == 0x21) c = 'f';
                if (scancode == 0x22) c = 'g';
                if (scancode == 0x23) c = 'h';
                if (scancode == 0x17) c = 'i';
                if (scancode == 0x24) c = 'j';
                if (scancode == 0x25) c = 'k';
                if (scancode == 0x26) c = 'l';
                if (scancode == 0x32) c = 'm';
                if (scancode == 0x31) c = 'n';
                if (scancode == 0x18) c = 'o';
                if (scancode == 0x19) c = 'p';
                if (scancode == 0x10) c = 'q';
                if (scancode == 0x13) c = 'r';
                if (scancode == 0x1F) c = 's';
                if (scancode == 0x14) c = 't';
                if (scancode == 0x16) c = 'u';
                if (scancode == 0x2F) c = 'v';
                if (scancode == 0x11) c = 'w';
                if (scancode == 0x2D) c = 'x';
                if (scancode == 0x15) c = 'y';
                if (scancode == 0x2C) c = 'z';

                if (c != '?' && input_index < 127) {
                    input[input_index++] = c;
                    print_char(c);
                }
            }
        }

        // Process input command
        trim_input(input);

        if (strcmp(input, "neofetch") == 0) {
            neofetch();
        } else if (strcmp(input, "clear") == 0) {
            clear_screen();
        } else if (strcmp(input, "help") == 0) {
            print_string("Available commands: neofetch, clear, help\n");
        } else if (strcmp(input, "") == 0) {
            // Do nothing
        } else {
            print_string("Unknown command: ");
            print_string(input);
            print_char('\n');
        }

        print_char('\n');
    }
}

