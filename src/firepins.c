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




#define MAX_TL_LEN 84 // 6*out output length.
// file size should be an integer multiple of output lentgh
#define PIN_DELAY 200


void fire(uint8_t *testOutput,uint32_t size) {
    pinInit();
    UART_init();
   
    
    _Bool c_prev[MAX_TL_LEN];
    _Bool c_next[MAX_TL_LEN];
    _Bool translation[MAX_TL_LEN];
    int output_len = 14; // In the current system state, you can do from 1-14
    int scale = 0;
    
    // Actual layout of attached braille cells
    _Bool cells[] = {0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0};
    
    int i = 0,s;
    s = CompileArray(translation,testOutput, scale, size);
    
    UART_pputs("\r\nFWDBTN : i = ");
    UART_puthex32(i);
    UART_pputs("\r\noutput_len = ");
    UART_puthex32(output_len);
    UART_pputs("\r\n         s = ");
    UART_puthex32(s);
    UART_pputs("\r\n      size = ");
    UART_puthex32(size);
    UART_pputs("\r\n     scale = ");
    UART_puthex32(scale);
                
    UART_pputs("\r\nTL: \r\n");
    for (int n = 0; n<s;n++){
        UART_putbit(translation[n]);
        if (!((n+1)%6)){
            UART_pputs(" ");
            if (!((n+1)%72)){
                UART_pputs("\n");
            }
        }
        
    }
    
    UART_pputs("\r\nSET CPREV: \r\n");
    for(int j = 0; j<output_len*6; j++){
        c_prev[j] = !translation[j];
        UART_putbit(translation[j]);
        UART_pputs("\r\nSET TL--s: \r\n");
        UART_putbit(c_prev[j]);
    }
    
    while(1){
        UART_pputs("\r\nDISPLAYING: \r\n");
        for(int j = 0; j<output_len*6; j++){
            c_next[j] = translation[j+i*6];
            UART_putbit(c_next[j]);
        }

        cellSelect(cells, c_prev, c_next);

        for (int j = 0; (j<output_len*6)&(j<size*6); j++){
            c_prev[j]=c_next[j];
        }

        // wait for button press
        while(1){
            if (!(PINbtn & (1<<FWDBTN))){
                
                if (scale+output_len <= size-output_len){
                    scale+=output_len;
                    s = CompileArray(translation, testOutput, scale, size);
                    UART_pputs("\r\nTL: \r\n");
                    for (int n = 0; n<s*6;n++){
                        UART_putbit(translation[n]);
                        if (!((n+1)%6)){
                            UART_pputs(" ");
                            if (!((n+1)%72)){
                                UART_pputs("\n");
                            }
                        }

                    }
                    
                    
                }
                
                UART_pputs("\r\nFWDBTN : i = ");
                UART_puthex32(i);
                UART_pputs("\r\noutput_len = ");
                UART_puthex32(output_len);
                UART_pputs("\r\n         s = ");
                UART_puthex32(s);
                UART_pputs("\r\n      size = ");
                UART_puthex32(size);
                UART_pputs("\r\n     scale = ");
                UART_puthex32(scale);
                
                break;
            }
            if (!(PINbtn & (1<<BCKBTN))){
                UART_pputs("\r\n BEFORE : i = ");
                UART_puthex32(i);
                
                if (scale-output_len >= 0){
                    scale-=output_len;
                    s = CompileArray(translation,testOutput, scale,size);
                    UART_pputs("\r\nTL: \r\n");
                    for (int n = 0; n<s*6;n++){
                        UART_putbit(translation[n]);
                        if (!((n+1)%6)){
                            UART_pputs(" ");
                            if (!((n+1)%72)){
                                UART_pputs("\n");
                            }
                        }

                    }
                } else {
                    i = 0;
                }
                
                UART_pputs("\r\nBCKBTN : i = ");
                UART_puthex32(i);
                UART_pputs("\r\noutput_len = ");
                UART_puthex32(output_len);
                UART_pputs("\r\n         s = ");
                UART_puthex32(s);
                UART_pputs("\r\n      size = ");
                UART_puthex32(size);
                UART_pputs("\r\n     scale = ");
                UART_puthex32(scale);
                
                break;
            }
        }
        
        
        
    }
    
}

void pinInit(){
    // set directions array 2
    DDRr_2 |= (1<<RESET_2);
    DDRs_2 |= (1<<SET_2);
    
    // pin select array 2
    DDR_PS2_1 |= (1<<P2_1)|(1<<P2_2);
    DDR_PS2_2 |= (1<<P2_0);
    
    // cell select array 2
    DDR_CS2_1 |= (1<<C2_1)|(1<<C2_2);
    DDR_CS2_2 |= (1<<C2_0);
    
    // pin select array 1
    DDR_PS1 |= (1<<P1_0)|(1<<P1_1)|(1<<P1_2);
    
    // cell select array 1
    DDR_CS1 |= (1<<C1_0)|(1<<C1_1)|(1<<C1_2);
    
    // set directions array 1
    DDRr_1 |= (1<<RESET_1);
    DDRs_1 |= (1<<SET_1);
    
    // buttons
    DDRbtn &= ~((1<<FWDBTN)|(1<<BCKBTN)); // negation for inputs
    PORTbtn |= ((1<<FWDBTN)|(1<<BCKBTN)); // internal pull up
    
    // start in RESET mode
    N_MODE_2
    N_MODE_1
}

_Bool pinSet_2(_Bool c_prev[],_Bool c_next[]){ 
    
    S_MODE_2
            
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
            _delay_ms(PIN_DELAY);
            UART_pputs("\r\n");
        }
        RESETPINS_2
    }
    
    R_MODE_2
    
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
            _delay_ms(PIN_DELAY);
            UART_pputs("\r\n");
        }
        RESETPINS_2
    }
    N_MODE_2
    
    return 0;
    
}

_Bool pinSet_1(_Bool c_prev[],_Bool c_next[]){ 
    
    S_MODE_1
            
    _Bool c_out[6];
    
    for (int i = 0; i<6;i++){
        c_out[i] = c_prev[i]& ~(c_next[i]);
    }
    
    for (uint8_t i = 0; i < 6; i++) {
        if (c_out[i]){
            UART_pputs("Resetting Pin: 0x");
            switch(i){
                case 0:
                    A_1
                    UART_puthex8(i+1);
                    break;
                case 1:
                    B_1
                    UART_puthex8(i+1);
                    break;
                case 2:
                    C_1
                    UART_puthex8(i+1);
                    break;
                case 3:
                    D_1
                    UART_puthex8(i+1);
                    break;
                case 4:
                    E_1
                    UART_puthex8(i+1);
                    break;
                case 5:
                    F_1
                    UART_puthex8(i+1);
                    break;
            }
            _delay_ms(PIN_DELAY);
            UART_pputs("\r\n");
        }
        RESETPINS_1
    }
    
    R_MODE_1
    
    for (int i = 0; i<6;i++){
        c_out[i] = c_next[i]& ~(c_prev[i]);
    }
    
    for (int i = 0; i < 6; i++) {
        if (c_out[i]){
            UART_pputs("Setting Pin: 0x");
            switch(i){
                case 0:
                    A_1
                    UART_puthex8(i+1);
                    break;
                case 1:
                    B_1
                    UART_puthex8(i+1);
                    break;
                case 2:
                    C_1
                    UART_puthex8(i+1);
                    break;
                case 3:
                    D_1
                    UART_puthex8(i+1);
                    break;
                case 4:
                    E_1
                    UART_puthex8(i+1);
                    break;
                case 5:
                    F_1
                    UART_puthex8(i+1);
                    break;
            }
            _delay_ms(PIN_DELAY);
            UART_pputs("\r\n");
        }
        RESETPINS_1
    }
    
    N_MODE_1
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
                case 0:
                    CS1_1
                    UART_pputs("\r\nARRAY 1 --- CELL 1\r\n");
                    break;
                case 1:
                    CS1_2
                    UART_pputs("\r\nARRAY 1 --- CELL 2\r\n");
                    break;
                case 2:
                    CS1_3
                    UART_pputs("\r\nARRAY 1 --- CELL 3\r\n");
                    break;
                case 3:
                    CS1_4
                    UART_pputs("\r\nARRAY 1 --- CELL 4\r\n");
                    break;
                case 4:
                    CS1_5
                    UART_pputs("\r\nARRAY 1 --- CELL 5\r\n");
                    break;
                case 5:
                    CS1_6
                    UART_pputs("\r\nARRAY 1 --- CELL 6\r\n");
                    break;
                case 6:
                    CS1_7
                    UART_pputs("\r\nARRAY 1 --- CELL 7\r\n");
                    break;
                case 7:
                    CS1_8
                    UART_pputs("\r\nARRAY 1 --- CELL 8\r\n");
                    break;
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
                pinSet_1(cp, cn);
            }
            _delay_ms(PIN_DELAY);
            
        }
    }

    return 0;
}