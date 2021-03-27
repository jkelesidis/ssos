#ifndef IDT_H
#define IDT_H

#include "types.h"

// segment selectors
#define KERNEL_CS 0x08

// how every interrupt handler is defined
typedef struct {
    uint16_t low_offset; // lower 16bits of handler function address
    uint16_t sel; // kernel segment selector
    uint8_t always0;

    // first byte
    // bit 7: interrupt is present
    // bits 6-5: privilesfe level of caller (0=kernel...3=user)
    // bit 4: set to 0 for interrupt gates
    // bits 3-0: bits 1110 = decimal 14 = 32 bit interrupt gate
    uint8_t flags;
    uint16_t high_offset; // higher 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

// pointer to the array of interrupt handlers
// the lidt assembly instruction will read it
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

// implemented in idt.c
void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif