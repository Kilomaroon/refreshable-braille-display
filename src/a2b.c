/* 
 * Author: kilomaroon
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"

#define BRAILLE_UPPER       0b000001   // ,
#define BRAILLE_NUMBER      0b001111   // #
#define BRAILLE_COMMA       0b010000
#define BRAILLE_SEMICOLON   0b011000
#define BRAILLE_COLON       0b010010
#define BRAILLE_PERIOD      0b010011
#define BRAILLE_EXCLAM      0b011010
#define BRAILLE_SPACE       0b000000
#define BRAILLE_FILL        0b111111
#define EOL                 0b11111111
#define MAX_TL_LEN 84

/* Links
 * https://en.wikipedia.org/wiki/English_Braille
 * https://en.wikipedia.org/wiki/ASCII
 *  https://en.wikipedia.org/wiki/Braille_ASCII
*/

const uint8_t braille_letters[26] PROGMEM = {
    0b100000, // A, 1 
    0b110000, // B, 2
    0b100100, // C, 3
    0b100110, // D, 4
    0b100010, // E, 5
    0b110100, // F, 6
    0b110110, // G, 7
    0b110010, // H, 8
    0b010100, // I, 9
    0b010110, // J, 0
    0b101000, // K
    0b111000, // L
    0b101100, // M
    0b101110, // N
    0b101010, // O
    0b111100, // P
    0b111110, // Q
    0b111010, // R
    0b011100, // S
    0b011110, // T
    0b101001, // U
    0b111001, // V
    0b010111, // W
    0b101101, // X
    0b101111, // Y
    0b101011  // Z
};


uint16_t asc2braille(char c)
{
    uint8_t letter = 0;
    uint8_t prefix = 0;

    // Uppercase letters
    if (c >= 'A' && c <= 'Z') {
        letter = pgm_read_byte(&braille_letters[c - 'A']);
        prefix = BRAILLE_UPPER;
    }

    // Lowercase letters
    else if (c >= 'a' && c <= 'z') {
        letter = pgm_read_byte(&braille_letters[c - 'a']);
    }

    // Numbers
    else if (c >= '1' && c <= '9') {
        letter = pgm_read_byte(&braille_letters[c - '1']);
        prefix = BRAILLE_NUMBER;
    }

    else if (c == '0') {
        letter = pgm_read_byte(&braille_letters['J' - 'A']);
        prefix = BRAILLE_NUMBER;
    }

    // Basic punctuation
    else {
        switch (c) {
            case ',': letter = BRAILLE_COMMA; break;
            case ';': letter = BRAILLE_SEMICOLON; break;
            case ':': letter = BRAILLE_COLON; break;
            case '.': letter = BRAILLE_PERIOD; break;
            case '!': letter = BRAILLE_EXCLAM; break;
            case '=': letter = BRAILLE_FILL; break;
            case '\0': letter = EOL; break;
            default:  letter = BRAILLE_SPACE;
        }
        prefix = BRAILLE_SPACE;
    }

    // Return format:
    //   High byte = prefix (0 if none)
    //   Low byte  = braille cell
    return (uint16_t)letter | ((uint16_t)prefix << 8);
}

_Bool* braille2array(uint16_t val){
    static _Bool pins [12];
    for (int i = 11; i >= 0; i--) {
        pins[i] = (val >> (11-i)) & 1;
    }
    return pins;
}

int CompileArray(_Bool *translation,  uint8_t *str, uint32_t s, uint32_t size) {
    static _Bool op[MAX_TL_LEN] = {0};
    _Bool *tmp, match, firstPref = 0;
    uint16_t prev, next;
    _Bool prevPref[6], nextPref[6];
    int x = 0;
    
    prev = asc2braille(str[s]);  // Get the first 2 Braille cells
    tmp = braille2array(prev);    // Convert the Braille cell to the pin array
    UART_pputs("\r\n\r\n\r\nBEGIN TRANSLATION\r\n");
    for (int k = 0; k<6; k++){
        prevPref[k] = tmp[k];
        if (prevPref[k]){
            firstPref = 1;
        }
    }
    
    if(firstPref){
        for (int n = 0; n < 12; n++) {
            op[n] = tmp[n];
            UART_putbit(op[n]);
        }
        x += 12;
    } else {
        for (int n = 0; n < 6; n++) {
            op[n] = tmp[n+6];
            UART_putbit(op[n]);
        }
        x+=6;
    }
    
    for (int i = 1; (i<size)&(i<MAX_TL_LEN/6); i++) {
        next = asc2braille(str[s+i]);  // Get the next Braille cell
        tmp = braille2array(next);
        match = 1;

        // Extract the prefixes of both the previous and next characters
        for (int k = 0; k<6; k++){
            nextPref[k] = tmp[k];
            if (nextPref[k] != prevPref[k]){
                match = 0;
            }
        }
        

        // if prefixes are same -> skip prefix for next 
        if (match) {
            UART_pputs("\r\nMATCH - ");
            for (int n = 0; n < 6; n++) {
                op[x + n] = tmp[6 + n];  // Skip the prefix for the current character
                UART_putbit(op[x + n]);
            }
            x += 6;  // Increment by half the cell size to account for the prefix
        }
        else {
              // No prefix match, process the full cell
            UART_pputs("\r\nNOMATCH - ");
            for (int n = 0; n < 12; n++) {
                op[x + n] = tmp[n];
                UART_putbit(op[x + n]);
            }
            x += 12;  // Standard increment
        }
        
        prev = next;  // Update the previous character for the next iteration
    }
    
    for (int i = 0; i<MAX_TL_LEN; i++){
        *translation++ = op[i];
    }
    UART_pputs("\r\n X IS ");
    UART_puthex32(x/6);
    
    
    
    return x/6;
}