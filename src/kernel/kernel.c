#include "../cpu/isr.h"
#include "kernel.h"
#include "../lib/string.h"
#include "../lib/mem.h"
#include "../lib/stdio.h"

void kernel_main() {
    isr_install();
    irq_install();

    asm("int $2");
    asm("int $3");

    printf("Type something, it will go through the kernel\n"
        "Type END to halt the CPU or PAGE to request a kmalloc()\n> ");

}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        printf("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        printf("Page: ");
        printf(page_str);
        printf(", physical address: ");
        printf(phys_str);
        printf("\n");
    }
    printf("You said: ");
    printf(input);
    printf("\n> ");
}
