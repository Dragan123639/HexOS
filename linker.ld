ENTRY(_start)

SECTIONS
{
  . = 0x100000;

  .text : {
    *(.multiboot)
    *(.text)
  }

  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss : { *(.bss COMMON) }
}

