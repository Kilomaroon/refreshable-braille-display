/* 
 * File:   firepins.h
 * Author: adt1
 *
 * Created on February 27, 2026, 9:49 PM
 */

#ifndef FIREPINS_H
#define	FIREPINS_H

#ifdef	__cplusplus
extern "C" {
#endif
_Bool pinSet_2(_Bool [],_Bool []);
_Bool pinSet_1(_Bool [],_Bool []);
_Bool cellSelect(_Bool [], _Bool [], _Bool []);
void pinInit();
void fire(uint8_t * , uint32_t);
/**
 * Converts an ASCII character to its Braille representation.
 *
 * @param c ASCII character to convert
 * @return 16-bit Braille pattern 
 */
uint16_t asc2braille(char c);
/**
 * Converts a 2 byte int into the braille output
 * 
 * @param val value to convert
 * @return  braille array 
 */
_Bool *braille2array(uint16_t);
int CompileArray(_Bool *, uint8_t *, uint32_t ,uint32_t );


#ifdef	__cplusplus
}
#endif

#endif	/* FIREPINS_H */
