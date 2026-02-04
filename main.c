/* 
 * Author: kilomaroon
 */

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>
#include "pinout.h"
/*
 * 
 */



_Bool cellReset(_Bool []);
_Bool cellSet(_Bool []);
void pinInit();

int main(int argc, char** argv) {
    pinInit();
    
    _Bool c_prev[] = {1,0,1,0,1,0};
    _Bool c_next[] = {1,1,1,0,0,0};
    _Bool c_out_s[6];
    _Bool c_out_r[6];
    
    for (int i = 0; i<6;i++){
        c_out_r[i] = c_prev[i]& ~(c_next[i]);
        c_out_s[i] = c_next[i]& ~(c_prev[i]);
    }
    
    while(1){
        cellReset(c_out_r);
        _delay_ms(2000);
        cellSet(c_out_s);
        _delay_ms(2000);
    }
    
    return (EXIT_SUCCESS);
}

void pinInit(){
    // set directions
    DDRr |= (1<<RESET);
    DDRs |= (1<<SET);
    DDRp |= (1<<P0)|(1<<P1)|(1<<P2);
    
    // start in RESET mode
    PORTs &= ~(1<<SET);
    PORTr |= (1<<RESET);
}

_Bool cellReset(_Bool c_out_r[]){ 
    
    R_MODE
    
    if(c_out_r[0]){
        PORTp = A;
        _delay_ms(2000);
    }
    if(c_out_r[1]){
        PORTp = B;
        _delay_ms(2000);
    }
    if(c_out_r[2]){
        PORTp = C;
        _delay_ms(2000);
    }
    if(c_out_r[3]){
        PORTp = D;
        _delay_ms(2000);
    }
    if(c_out_r[4]){
        PORTp = E;
        _delay_ms(2000);
    }
    if(c_out_r[5]){
        PORTp = F;
        _delay_ms(2000);
    }
    return 0;
}

_Bool cellSet(_Bool c_out_s[]){ 
    
    S_MODE
            
    if(c_out_s[0]){
        PORTp = A;
        _delay_ms(2000);
    }
    if(c_out_s[1]){
        PORTp = B;
        _delay_ms(2000);
    }
    if(c_out_s[2]){
        PORTp = C;
        _delay_ms(2000);
    }
    if(c_out_s[3]){
        PORTp = D;
        _delay_ms(2000);
    }
    if(c_out_s[4]){
        PORTp = E;
        _delay_ms(2000);
    }
    if(c_out_s[5]){
        PORTp = F;
        _delay_ms(2000);
    }
    
    return 0;
}