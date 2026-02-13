/* 
 * Author: kilomaroon
 */

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>
#include "pinout.h"
#include "uart.h"
/*
 * 
 */



_Bool pinReset(_Bool []);
_Bool pinSet(_Bool []);
_Bool cellSelect(uint8_t);
void pinInit();

int main(int argc, char** argv) {
    pinInit();
    UART_init();
    
    _Bool c_prev[6] = {1,1,1,1,0,0};
    _Bool c_next[6] = {0,0,1,1,1,1};
    _Bool c_out_s[6];
    _Bool c_out_r[6];
    
    for (int i = 0; i<6;i++){
        c_out_r[i] = c_prev[i]& ~(c_next[i]);
        c_out_s[i] = c_next[i]& ~(c_prev[i]);
    }
    
    while(1){
        pinReset(c_out_r);
        pinSet(c_out_s);
        PORTc0 |= ((1<<C0)); // cell 2
    }
    
    return (EXIT_SUCCESS);
}

void pinInit(){
    // set directions
    DDRr |= (1<<RESET);
    DDRs |= (1<<SET);
    DDRp |= (1<<P0)|(1<<P1)|(1<<P2);
    DDRc12 |= (1<<C1)|(1<<C2);
    DDRc0 |= (1<<C0);
    PORTc12 &= ~((1<<C1)|(1<<C2));
    PORTc0 &= ~((1<<C0));
    PORTc0 |= ((1<<C0));
    
    // start in RESET mode
    PORTs &= ~(1<<SET);
    PORTr |= (1<<RESET);
}

_Bool pinReset(_Bool c_out_r[]){ 
    
    R_MODE
    UART_pputs("\r\n------------------RESET----------------------\r\n");
    if(c_out_r[0]){
        UART_pputs("A");
        UART_pputs("\r\n[*][ ]"
                    "\n[ ][ ]"
                    "\n[ ][ ]\r\n");
        A;
        _delay_ms(2000);
    }
    if(c_out_r[1]){
        UART_pputs("B");
        UART_pputs("\r\n[ ][ ]"
                    "\n[*][ ]"
                    "\n[ ][ ]\r\n");
        B;
        _delay_ms(2000);
    }
    if(c_out_r[2]){
        UART_pputs("C");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][ ]"
            "\n[*][ ]\r\n");
        C;
        _delay_ms(2000);
    }
    if(c_out_r[3]){
        UART_pputs("D");
        UART_pputs("\r\n[ ][*]"
            "\n[ ][ ]"
            "\n[ ][ ]\r\n");
        D;
        _delay_ms(2000);
    }
    if(c_out_r[4]){
        UART_pputs("E");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][*]"
            "\n[ ][ ]\r\n");
        E;
        _delay_ms(2000);
    }
    if(c_out_r[5]){
        UART_pputs("F");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][ ]"
            "\n[ ][*]\r\n");
        F;
        _delay_ms(2000);
    }
    
    
    return 0;
}

_Bool pinSet(_Bool c_out_s[]){ 
    
    S_MODE
    UART_pputs("\r\n-------------------SET-----------------------\r\n");        
    if(c_out_s[0]){
        UART_pputs("A");
        UART_pputs("\r\n[*][ ]"
                    "\n[ ][ ]"
                    "\n[ ][ ]\r\n");
        A;
        _delay_ms(2000);
    }
    if(c_out_s[1]){
        UART_pputs("B");
        UART_pputs("\r\n[ ][ ]"
                    "\n[*][ ]"
                    "\n[ ][ ]\r\n");
        B;
        _delay_ms(2000);
    }
    if(c_out_s[2]){
        UART_pputs("C");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][ ]"
            "\n[*][ ]\r\n");
        C;
        _delay_ms(2000);
    }
    if(c_out_s[3]){
        UART_pputs("D");
        UART_pputs("\r\n[ ][*]"
            "\n[ ][ ]"
            "\n[ ][ ]\r\n");
        D;
        _delay_ms(2000);
    }
    if(c_out_s[4]){
        UART_pputs("E");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][*]"
            "\n[ ][ ]\r\n");
        E;
        _delay_ms(2000);
    }
    if(c_out_s[5]){
        UART_pputs("F");
        UART_pputs("\r\n[ ][ ]"
            "\n[ ][ ]"
            "\n[ ][*]\r\n");
        F;
        _delay_ms(2000);
    }
    UART_pputs(" SET");
    
    return 0;
}

_Bool cellSelect(uint8_t cell){
    
    return 0;
}