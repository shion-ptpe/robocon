#include <xc.h>
#define FCY 4000000
#include "config.h"
#include "user.h"
#include "function.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

extern data_t queue_data[QUEUE_SIZE];

void config();


int main(){
    config();
    motor_config();
    UART1_config();
    UART2_config();


    while(1){

        while(U2STAbits.UTXBF==1){  // もしUTXBF==「1」なら送信バッファは一杯
            // waiting
        }

        if(queue_data[0]==0x80){
            motor_control_move(queue_data[1]);
            dequeue(&queue_data[0]);
            dequeue(&queue_data[0]);
            }
        else{
            dequeue(&queue_data[0]);
        }
    }
    return 0;
}

void config(){
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    TRISB = 0xffff;
    TRISD = 0xffff;
    TRISE = 0xffff;
    TRISG = 0xffff;
    U1MODE = 0x0000;
    U1STA = 0x0000;
    ANSB = 0;
    ANSD = 0;
    ANSE = 0;
    TRISBbits.TRISB3 = 0;  // LED
    TRISDbits.TRISD3 = 0;  // LED
    TRISDbits.TRISD7 = 0;  // LED

//    LATBbits.LATB3=1;  //green
//    LATDbits.LATD3=1;  //red
//    LATDbits.LATD7=1;  //yellow
}
