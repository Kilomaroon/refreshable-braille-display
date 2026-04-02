/* 
 * Author: kilomaroon
 */


#ifndef PINOUT_H
#define    PINOUT_H

/* BUTTONS */
#define DDRbtn DDRC
#define PORTbtn PORTC
#define PINbtn PINC

#define FWDBTN PC0
#define BCKBTN PC1

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

#define N_MODE PORTr &= ~(1<<RESET);\
PORTs &= ~(1<<SET);

/*----------------------------------------------*/
/* DECODER SELECTS - ARRAY 2                    */
/*----------------------------------------------*/

/* PIN SELECT */

 // pin select direction register
#define DDR_PS2_1 DDRB
#define DDR_PS2_2 DDRD
// pin select output register
#define PORT_PS2_1 PORTB
#define PORT_PS2_2 PORTD
// P2_0:2 - decoder pin selects
#define P2_0 PD5
#define P2_1 PB7
#define P2_2 PB6

/* CELL SELECT */

// cell select direction register
#define DDR_CS2_1 DDRD
#define DDR_CS2_2 DDRB
// cell select output register
#define PORT_2_1 PORTD
#define PORT_2_2 PORTB
// C0:2 - decoder pin selects
#define C2_2 PD6
#define C2_1 PD7
#define C2_0 PB0

/* SETS */

/* Format: P refers to pin select, A:F refers to braille pin*/
/* A  D */
/* B  E */
/* C  F */
#define CELL2_MASK1 (1<<P2_1)|(1<<P2_2)
#define CELL2_MASK2 (1<<P2_0)

#define RESETPINS_2 PORT_PS2_1 |= ((1<<P2_1)|(1<<P2_2)); \
PORT_PS2_2 |= (1<<P2_0);

#define A_2 PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1) & ~((1<<P2_2)|(1<<P2_1)); \
PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2) & ~(1<<P2_0); 

#define B_2 PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2); \
PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1) & ~((1<<P2_1)|(1<<P2_2));

#define C_2 PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2)  & ~(1<<P2_0); \
PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1)& ~((1<<P2_2));

#define D_2 PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1) & ~(1<<P2_2); \
PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2); 

#define E_2 PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1) & ~((1<<P2_1)); \
PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2) & ~(1<<P2_0);

#define F_2 PORT_PS2_1 = (PORT_PS2_1|CELL2_MASK1) & ~(1<<P2_1); \
PORT_PS2_2 = (PORT_PS2_2|CELL2_MASK2);

#define CS2_1 PORT_2_1 |= ((1<<C2_1)|(1<<C2_2)); \
        PORT_2_2 |= ((1<<C2_0));

#define CS2_2 PORT_2_1 |= ((1<<C2_1)|(1<<C2_2)); \
        PORT_2_2 &= ~((1<<C2_0));

#define CS2_3  PORT_2_1 &= ~(1<<C2_1); \
        PORT_2_1 |= (1<<C2_2); \
        PORT_2_2 |= ((1<<C2_0));

#define CS2_4 PORT_2_1 &= ~(1<<C2_1); \
        PORT_2_1 |= (1<<C2_2); \
        PORT_2_2 &= ~((1<<C2_0));

#define CS2_5 PORT_2_1 &= ~(1<<C2_2); \
        PORT_2_1 |= (1<<C2_1); \
        PORT_2_2 |= ((1<<C2_0));

#define CS2_6 PORT_2_1 &= ~(1<<C2_2); \
        PORT_2_1 |= (1<<C2_1); \
        PORT_2_2 &= ~((1<<C2_0));

#define CS2_7 PORT_2_1 &= ~(1<<C2_1); \
        PORT_2_1 &= ~(1<<C2_2); \
        PORT_2_2 |= ((1<<C2_0));

#define CS2_8 PORT_2_1 &= ~(1<<C2_1); \
        PORT_2_1 &= ~(1<<C2_2); \
        PORT_2_2 &= ~((1<<C2_0));


#ifdef    __cplusplus
extern "C" {
#endif

#ifdef    __cplusplus
}
#endif

#endif    /* PINOUT_H */