/* 
 * Author: kilomaroon
 */
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define BRAILLE_UPPER       0b000001   // ,
#define BRAILLE_NUMBER      0b001111   // #
#define BRAILLE_COMMA       0b010000
#define BRAILLE_SEMICOLON   0b011000
#define BRAILLE_COLON       0b010010
#define BRAILLE_PERIOD      0b010011
#define BRAILLE_EXCLAM      0b011010
#define BRAILLE_SPACE       0b000000

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
            default:  letter = BRAILLE_SPACE;
        }
    }

    // Return format:
    //   High byte = prefix (0 if none)
    //   Low byte  = braille cell
    return (uint16_t)letter | ((uint16_t)prefix << 8);
}




