#include "string.h"
#include "../cpu/types.h"

/**
 * K&R implementation
 * quick and dirty implementation of itoa
 */
void int_to_ascii(int n, char str[]) {

    int i = 0;
    int sign = n;

    if (sign < 0) {

        n = -n;
    }

    do {

        str[ i++ ] = n % 10 + '0';
    } while (( n /= 10) > 0);

    if (sign < 0) {

        str[ i++ ] = '-';
    }

    str[i] = '\0';

}

/**
 * prints hex values
 */
void hex_to_ascii(int n, char str[]) {

    append(str, '0');
    append(str, 'x');

    char zeros = 0;

    int32_t tmp;
    for (int i = 28; i > 0; i -= 4) {

        tmp = (n >> i) & 0xf;
        if (tmp == 0 && zeros == 0) {

            continue;
        }

        zeros = 1;
        if (tmp > 0xa) {

            append(str, tmp - 0xa + 'a');
        } else {

            append(str, tmp + '0');
        }
    }

    tmp = n & 0xf;
    if (tmp >= 0xa) {

        append(str, tmp - 0xa + 'a');
    } else {

        append(str, tmp + '0');
    }
    
}

// K&R
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j ; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// K&R
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

/**
 * Appends a character to a "string"
 */
void append(char s[], char n) {

    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

/**
 * K&R
 * returns < 0 if s1 < s2, 0 if s1 == s2, > 0 if s1 > s2
 */
int strcmp(char s1[], char s2[]) {

    int i;
    for (i = 0; s1[i] == s2[i]; i++) {

        if (s1[i] == '\0') {

            return 0;
        }
    }
     return s1[i] - s2[i];
}

/**
 * removes the last character of the array
 */
void backspace(char s[]) {

    int len = strlen(s);
    s[len-1] = '\0';
}