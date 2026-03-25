/* 
 * Author: kilomaroon
 */

#include <stdio.h>
#include <stdlib.h>
#include<avr/io.h>
#include "firepins.h"
#include <string.h>
#define F_CPU 8000000UL
#include<util/delay.h>
#include "pinout.h"
#include "uart.h"
#include "a2b.h"
/*
 * 
 */




#define MAX_TL_LEN 128


void fire(uint8_t *testOutput,uint32_t size) {
    pinInit();
    UART_init();
   
    
    _Bool c_prev[MAX_TL_LEN];
    _Bool c_next[MAX_TL_LEN];
    _Bool translation[MAX_TL_LEN];
    uint8_t output_len = 1;
    
    _Bool cells[] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
    
    int i = 0,p=0,s;
    s = CompileArray(translation,testOutput, size);
    UART_pputs("\r\nTL: \r\n");
    for (int n = 0; n<MAX_TL_LEN;n++){
        UART_putbit(translation[n]);
        p++;
        if (p>=6){
            UART_pputs("\n");
            p=0;
        }
        
    }
    
    
    for(int j = 0; j<output_len*6; j++){
        c_prev[j] = 0;
    }
    
    while(1){
        UART_pputs("\r\nDISPLAYING: \r\n");
        for(int j = 0; j<output_len*6; j++){
            c_next[j] = translation[j+i*6];
            UART_putbit(c_next[j]);
        }

        cellSelect(cells, c_prev, c_next);

        for (int j = 0; j<output_len*6; j++){
            c_prev[j]=c_next[j];
        }

        // wait for button press
        while(1){
            if (!(PINbtn & (1<<FWDBTN))){
                
                if (i+ output_len > (s-output_len)){
                        i = (s-output_len);
                } else {
                    i += output_len;
                }
                UART_pputs("\r\nFWDBTN");
                break;
            }
            if (!(PINbtn & (1<<BCKBTN))){
                
                if (i - output_len < 0){
                    i = 0;
                } else {
                    i -= output_len;
                }
                UART_pputs("\r\nBCKBTN");
                break;
            }
        }
        
        
        
    }
    
}

void pinInit(){
    // set directions
    DDRr |= (1<<RESET);
    DDRs |= (1<<SET);
    
    // pin select array 2
    DDR_PS2_1 |= (1<<P2_1)|(1<<P2_2);
    DDR_PS2_2 |= (1<<P2_0);
    
    // cell select array 2
    DDR_CS2_1 |= (1<<C2_1)|(1<<C2_2);
    DDR_CS2_2 |= (1<<C2_0);
    
    // buttons
    DDRbtn &= ~((1<<FWDBTN)|(1<<BCKBTN)); // negation for inputs
    PORTbtn |= ((1<<FWDBTN)|(1<<BCKBTN)); // internal pull up
    
    // start in RESET mode
    PORTs &= ~(1<<SET);
    PORTr |= (1<<RESET);
}

_Bool pinSet_2(_Bool c_prev[],_Bool c_next[]){ 
    
    S_MODE
            
    _Bool c_out[6];
    
    for (int i = 0; i<6;i++){
        c_out[i] = c_prev[i]& ~(c_next[i]);
    }
    
    for (uint8_t i = 0; i < 6; i++) {
        if (c_out[i]){
            UART_pputs("Resetting Pin: 0x");
            switch(i){
                case 0:
                    A_2
                    UART_puthex8(i+1);
                    break;
                case 1:
                    B_2
                    UART_puthex8(i+1);
                    break;
                case 2:
                    C_2
                    UART_puthex8(i+1);
                    break;
                case 3:
                    D_2
                    UART_puthex8(i+1);
                    break;
                case 4:
                    E_2
                    UART_puthex8(i+1);
                    break;
                case 5:
                    F_2
                    UART_puthex8(i+1);
                    break;
            }
            _delay_ms(250);
            UART_pputs("\r\n");
        }
        RESETPINS_2
    }
    
    R_MODE
    
    for (int i = 0; i<6;i++){
        c_out[i] = c_next[i]& ~(c_prev[i]);
    }
    
    for (int i = 0; i < 6; i++) {
        if (c_out[i]){
            UART_pputs("Setting Pin: 0x");
            switch(i){
                case 0:
                    A_2
                    UART_puthex8(i+1);
                    break;
                case 1:
                    B_2
                    UART_puthex8(i+1);
                    break;
                case 2:
                    C_2
                    UART_puthex8(i+1);
                    break;
                case 3:
                    D_2
                    UART_puthex8(i+1);
                    break;
                case 4:
                    E_2
                    UART_puthex8(i+1);
                    break;
                case 5:
                    F_2
                    UART_puthex8(i+1);
                    break;
            }
            _delay_ms(250);
            UART_pputs("\r\n");
        }
        RESETPINS_2
    }
    
    return 0;
    
}

_Bool pinSet_1(_Bool c_out_s[]){ 
    
    S_MODE
            
    if(c_out_s[0]){
        RESETPINS_2
        A_2
        _delay_ms(1000);
    }
    if(c_out_s[1]){
        RESETPINS_2
        B_2
        _delay_ms(1000);
    }
    if(c_out_s[2]){
        RESETPINS_2
        C_2
        _delay_ms(1000);
    }
    if(c_out_s[3]){
        RESETPINS_2
        D_2
        _delay_ms(1000);
    }
    if(c_out_s[4]){
        RESETPINS_2
        E_2
        _delay_ms(1000);
    }
    if(c_out_s[5]){
        RESETPINS_2
        F_2
        _delay_ms(1000);
    }
    
    return 0;
}

_Bool cellSelect(_Bool cells[],_Bool c_prev[],_Bool c_next[]){
    _Bool cp[6];
    _Bool cn[6];
    int x = 0;
    
    for (int i = 0; i < 16; i++) {
        if (cells[i]){
            for (int j = 0; j <6;j++){
                cp[j] = c_prev[j+x];
                cn[j] = c_next[j+x];
            }
            x += 6;
            switch(i){
                case 8:
                    CS2_1
                    UART_pputs("\r\nARRAY 2 --- CELL 1\r\n");
                    break;
                case 9:
                    CS2_2
                    UART_pputs("\r\nARRAY 2 --- CELL 2\r\n");
                    break;
                case 10:
                    CS2_3
                    UART_pputs("\r\nARRAY 2 --- CELL 3\r\n");
                    break;
                case 11:
                    CS2_4
                    UART_pputs("\r\nARRAY 2 --- CELL 4\r\n");
                    break;
                case 12:
                    CS2_5
                    UART_pputs("\r\nARRAY 2 --- CELL 5\r\n");
                    break;
                case 13:
                    CS2_6
                    UART_pputs("\r\nARRAY 2 --- CELL 6\r\n");
                    break;
                case 14:
                    CS2_7
                    UART_pputs("\r\nARRAY 2 --- CELL 7\r\n");
                    break;
                case 15:
                    CS2_8
                    UART_pputs("\r\nARRAY 2 --- CELL 8\r\n");
                    break;
            }
            if (i>7){
                pinSet_2(cp, cn);
                
            } else {
                UART_pputs("\r\nInvalid Cell: Array 1\r\n");
            }
            _delay_ms(1000);
            
        }
    }

    return 0;
}