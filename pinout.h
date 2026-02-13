/* 
 * Author: kilomaroon
 */


#ifndef PINOUT_H
#define    PINOUT_H

/*----------------------------------------------*/
/* DECODER SELECTS */
/*----------------------------------------------*/
/* SET/RESET ENABLE */
#define DDRr DDRB
#define DDRs DDRD
#define PORTr PORTB
#define PORTs PORTD

#define SET PD4
#define RESET PB1

#define R_MODE PORTr |= (1<<RESET); \
PORTs &= ~(1<<SET);

#define S_MODE PORTs |= (1<<SET); \
PORTr &= ~(1<<RESET);

/* PIN SELECT */

 // pin select direction register
#define DDRp DDRC
// pin select output register
#define PORTp1 PORTB
#define PORTp2 PORTD
// P0:2 - decoder pin selects
#define P0 PD5
#define P1 PB7
#define P2 PB6

/* CELL SELECT */

// cell select direction register
#define DDRc0 DDRB
#define DDRc12 DDRD
// cell select output register
#define PORTc0 PORTB
#define PORTc12 PORTD
// C0:2 - decoder pin selects
#define C2 PD6
#define C1 PD7
#define C0 PB0

/* Format: P refers to pin select, A:F refers to braille pin*/
/* A  D */
/* B  E */
/* C  F */
#define CELL_MASK1 (1<<P1)|(1<<P2)
#define CELL_MASK2 (1<<P0)

#define A PORTp1 = (PORTp1|CELL_MASK1) & ~(1<<P1); \
PORTp2 = (PORTp2|CELL_MASK2);

#define B PORTp2 = (PORTp2|CELL_MASK2) & ~(1<<P0); \
PORTp2 = (PORTp2|CELL_MASK2);

#define C PORTp2 = (PORTp2|CELL_MASK2) & ~((1<<P0)); \
PORTp1 = (PORTp1|CELL_MASK1) & ~(1<<P1);

#define D PORTp1 = (PORTp1|CELL_MASK1) & ~((1<<P2)|(1<<P1)); \
PORTp2 = (PORTp2|CELL_MASK2); 

#define E PORTp1 = (PORTp1|CELL_MASK1) & ~((1<<P2)); \
PORTp2 = (PORTp2|CELL_MASK2) & ~(1<<P0);

#define F PORTp1 = (PORTp1|CELL_MASK1) & ~((1<<P2)|(1<<P1)); \
PORTp2 = (PORTp2|CELL_MASK2) & ~(1<<P0);


#ifdef    __cplusplus
extern "C" {
#endif

#ifdef    __cplusplus
}
#endif

#endif    /* PINOUT_H */