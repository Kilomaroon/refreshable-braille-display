/*
 * Author: kilomaroon
 */

#ifndef A2B_H
#define A2B_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converts an ASCII character to its Braille representation.
 *
 * @param c ASCII character to convert
 * @return 16-bit Braille pattern 
 */
uint16_t asc2braille(char c);

#ifdef __cplusplus
}
#endif

#endif /* A2B_H */
