#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

// private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// public kernel api functions

/**
 * Print a message at the specified location
 * If col, row are negative, the current offset will be used
 */
void kprint_at(char *message, int col, int row) {

    // set cursor if col, row are negative
    int offset;
    if (col >= 0 && row >= 0) {

        offset = get_offset(col, row);
    } else {

        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    // loop through the message and print it
    int i=0;
    while (message[i] != 0) {

        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);

        // calculate row/col for next iteration
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

/**
 * Prints a message at the current cursor location
 */
void kprint(char *message) {

    kprint_at(message, -1, -1);
}


/**
 * Prints a blank character at the previous cursor location (offset - 1) and places the cursor to the previous location
 */
void kprint_backspace() {

    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}

// private kernel functions

/**
 * Innermost kernel print function, directly accesses the video memory
 * if col and row are negative, it will print at current cursor location
 * If attr is zero, it will use white on black as default
 * returns the offset to the next character
 * sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {

    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    
    if (!attr) {

        attr = WHITE_ON_BLACK;
    }

    // print a red E if the coords aren't right
    if (col >= MAX_COLS || row >= MAX_ROWS) {

        vidmem[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        vidmem[2 * MAX_COLS * MAX_ROWS - 1] = RED_ON_WHITE;
        
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) {

        offset = get_offset(col, row);
    } else {

        offset = get_cursor_offset();
    }

    if (c == '\n') {

        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } else if (c == 0x08) { // backspace

        vidmem[offset] = ' ';
        vidmem[offset + 1] = attr;
    } else {

        vidmem[offset] = c;
        vidmem[offset + 1] = attr;
        offset += 2;
    }

    // check if offset is ourside of the screen and scroll
    if (offset >= MAX_ROWS * MAX_COLS * 2) {

        int i;
        for (i = 1; i < MAX_ROWS; i++) {

            memory_copy((uint8_t *)(get_offset(0, i) + VIDEO_ADDRESS),
                        (uint8_t *)(get_offset(0, i-1) + VIDEO_ADDRESS),
                        MAX_COLS * 2);
        }

        // blank last line
        char *last_line = (char *)(get_offset(0, MAX_ROWS - 1) + (uint8_t *)VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++) {
            if (i % 2 == 0) {

                last_line[i] = 0;
            } else {

                last_line[i] = WHITE_ON_BLACK;
            }
        }

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);

    return offset;
}


/**
 * Uses the vga ports to get the current cursor position
 * Check ports.c for more details
 */
int get_cursor_offset() {

    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; // high byte

    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2; // position * size of character cell
}


/**
 * Similar to get_cursor_offset but it writes the data instead of reading it
 */
void set_cursor_offset(int offset) {

    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));

    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}


/**
 * Clears the screen and places the cursor at 0,0
 */
void clear_screen() {

    int screen_size = MAX_COLS * MAX_ROWS;

    int i;
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;

    for(i = 0; i < screen_size; i++) {

        screen[ i * 2 ] = ' ';
        screen[ i * 2 + 1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) {

    return 2 * (row * MAX_COLS + col);
}

int get_offset_row(int offset) {

    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {

    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

int get_current_row() {

    return get_offset_row(get_cursor_offset());
}

int get_current_col() {

    return get_offset_col(get_cursor_offset());
}