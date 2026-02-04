/* 
 * Author: kilomaroon
 */


#ifndef PINOUT_H
#define	PINOUT_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PINOUT_H */

/*----------------------------------------------*/
/* DECODER SELECTS */
/*----------------------------------------------*/
/* SET/RESET ENABLE */
#define DDRr DDRD
#define DDRs DDRC
#define PORTr PORTD
#define PORTs PORTC

#define SET PC5
#define RESET PD0

#define R_MODE PORTr |= (1<<RESET); \
PORTs &= ~(1<<SET);

#define S_MODE PORTs |= (1<<SET); \
PORTr &= ~(1<<RESET);

/* PIN SELECT */

 // pin select direction register
#define DDRp DDRC
// pin select output register
#define PORTp PORTC
// P0:2 - decoder pin selects
#define P0 PC2
#define P1 PC3
#define P2 PC4

/* Format: P refers to pin select, A:F refers to braille pin*/
/* A  D */
/* B  E */
/* C  F */
#define CELL_MASK (1<<P0)|(1<<P1)|(1<<P2)
#define A (PORTp|CELL_MASK) & ~(1<<P1)
#define B (PORTp|CELL_MASK) & ~(1<<P0)
#define C (PORTp|CELL_MASK) & ~((1<<P1)|(1<<P0))
#define D (PORTp|CELL_MASK) & ~((1<<P2)|(1<<P1))
#define E (PORTp|CELL_MASK) & ~((1<<P2)|(1<<P0))
#define F (PORTp|CELL_MASK) & ~((1<<P2)|(1<<P1)|(1<<P0))