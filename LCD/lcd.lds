SECTIONS {
    . = 0x00000000;
    .init : AT(0){ head.o init.o nand.o}
    . = 0x30000000;
    .text : AT(4096) { *(.text) }
    .rodata ALIGN(4) : {*(.rodata*)} 
    .data ALIGN(4)   : { *(.data) }
    __bss_start = .;
    .bss ALIGN(4)  : { *(.bss)  *(COMMON) }
    __bss_end = .;
}
