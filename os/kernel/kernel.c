#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../cpu/timer.h"

void kernel_main() {

    clear_screen();
    kprint("Super Simple OS\n");
    kprint("v0.01\n");
    kprint("MailerLite Creative Day 2021-03-26\n\n");
    kprint("Kernel init\n");

    kprint("Init ISRs...");
    
    isr_install();
    kprint("done.\n");

    kprint("Init IRQs...");

    irq_install();

    kprint("done.\n");
    
    test_timer();

    char inp[] = {0x1C, '\0'};
    user_input(inp);
}


void test_timer() {
    
    init_timer(50);

    int i = 0;
    int j = 0;
    kprint("\n\nHey look! I support a timer ->  ");
    int timer1_row = get_current_row();
    int timer1_col = get_current_col();

    kprint("\n\nThis one is faster ->  ");
    int timer2_row = get_current_row();
    int timer2_col = get_current_col();

    kprint("\n\n");

    uint32_t start_ticks = get_ticks();
    uint32_t timer2_ticks = get_ticks();

    while (j < 50) {

        uint32_t current_ticks = get_ticks();
        

        if (current_ticks - start_ticks > 10) {

            if (i % 2 == 0) {

                kprint_at("|", timer1_col, timer1_row);
            } else {

                kprint_at("-", timer1_col, timer1_row);
            }

            start_ticks = current_ticks;
            i++;
        }

        if (current_ticks - timer2_ticks > 5) {

            if (j % 2 == 0) {

                kprint_at("|", timer2_col, timer2_row);
            } else {

                kprint_at("-", timer2_col, timer2_row);
            }

            timer2_ticks = current_ticks;

            j++;
        }
    }

    kprint("\nPassed ticks: ");
    char s[20];
    int_to_ascii(get_ticks(), s);
    kprint(s);
    kprint("\n\n");
    kprint("Ready");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {

        kprint("CPU HALTED\n");
        asm volatile("hlt");
    } else if(strcmp(input, "PAGE") == 0) {

        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");

    } else if (strcmp(input, "TICKS") == 0) {

        kprint("\nPassed ticks: ");
        char s[20];
        int_to_ascii(get_ticks(), s);
        kprint(s);
        kprint("\n");

    } else if (strcmp(input, "HELLO") == 0) {

        kprint("\nHello to you too!\n");
    } else if (strcmp(input, "HELP") == 0) {

        kprint("\n lol, nope\n");
    }


    kprint("\n> ");
}