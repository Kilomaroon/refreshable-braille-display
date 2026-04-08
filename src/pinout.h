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
// array 1
#define DDRr_1 DDRD
#define DDRs_1 DDRC
#define PORTr_1 PORTD
#define PORTs_1 PORTC

#define SET_1 PC5
#define RESET_1 PD0

#define S_MODE_1 PORTr_1 |= (1<<RESET_1); \
PORTs_1 &= ~(1<<SET_1);

#define R_MODE_1 PORTs_1 |= (1<<SET_1); \
PORTr_1 &= ~(1<<RESET_1);

#define N_MODE_1 PORTr_1 &= ~(1<<RESET_1);\
PORTs_1 &= ~(1<<SET_1);

// array 2
#define DDRr_2 DDRB
#define DDRs_2 DDRD
#define PORTr_2 PORTB
#define PORTs_2 PORTD

#define SET_2 PD4
#define RESET_2 PB1

#define S_MODE_2 PORTr_2 |= (1<<RESET_2); \
PORTs_2 &= ~(1<<SET_2);

#define R_MODE_2 PORTs_2 |= (1<<SET_2); \
PORTr_2 &= ~(1<<RESET_2);

#define N_MODE_2 PORTr_2 &= ~(1<<RESET_2);\
PORTs_2 &= ~(1<<SET_2);

/*----------------------------------------------*/
/* DECODER SELECTS - ARRAY 2                    */
/*----------------------------------------------*/

/* PIN SELECT */

// ARRAY 1

// pin select direction register
#define DDR_PS1 DDRC
// pin select output register
#define PORT_PS1 PORTC
// P2_0:2 - decoder pin selects
#define P1_0 PC4
#define P1_1 PC3
#define P1_2 PC2

// ARRAY 2

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

// ARRAY 1
// cell select direction register
#define DDR_CS1 DDRD
// cell select output register
#define PORT_1 PORTD
// C0:2 - decoder pin selects
#define C1_2 PD1
#define C1_1 PD2
#define C1_0 PD3

// ARRAY 2
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

#define CELL1_MASK (1<<P1_0)|(1<<P1_1)|(1<<P1_2)

#define RESETPINS_1 PORT_PS1 |= (1<<P1_0)|(1<<P1_1)|(1<<P1_2);

#define A_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_2)|(1<<P1_1)|(1<<P1_0)) ;

#define B_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_2)|(1<<P1_1)) ; 

#define C_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_2)|(1<<P1_0)) ;

#define D_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_2)) ;

#define E_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_1)|(1<<P1_0)) ;

#define F_1 PORT_PS1 = (PORT_PS1|CELL1_MASK)& ~((1<<P1_1)) ;

#define CS1_1  PORT_1 |= ((1<<C1_1)|(1<<C1_2)|(1<<C1_0)); 

#define CS1_2  PORT_1 &= ~((1<<C1_0)); \
PORT_1 |= ((1<<C1_1)|(1<<C1_2)); 

#define CS1_3  PORT_1 &= ~((1<<C1_1)); \
PORT_1 |= ((1<<C1_0)|(1<<C1_2));  

#define CS1_4 PORT_1 &= ~((1<<C1_0)|(1<<C1_1)); \
PORT_1 |= ((1<<C1_2)); 

#define CS1_5 PORT_1 &= ~((1<<C1_2)); \
PORT_1 |= ((1<<C1_0)|(1<<C1_1)); 

#define CS1_6 PORT_1 &= ~((1<<C1_0)|(1<<C1_2)); \
PORT_1 |= ((1<<C1_1)); 

#define CS1_7 PORT_1 &= ~((1<<C1_1)|(1<<C1_2)); \
PORT_1 |= ((1<<C1_0)); 

#define CS1_8 PORT_1 &= ~((1<<C1_1)|(1<<C1_2)|(1<<C1_0)); 

// ARRAY 2

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