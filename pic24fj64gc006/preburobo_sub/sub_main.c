#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "function.h"

void config();

int main(){
    config();
    UART2_config();

#define RIGHT  'R'  // マイコン間通信(TXに使用)
#define LEFT 'L'
#define SELECT_M7 'S7'
#define SELECT_M8 'S8'
#define SELECT_RIGHT 'SR'
#define SELECT_LEFT 'SL'


    while(1){
        LATBbits.LATB6=1; // LED
        if(dequeue(&queue_data[0])){   // 「return SUCCESS;」によって、  dequeue()がTrueの間はこの中をループする
            if(queue_data[0]==0x52){ // R
                //
            }
            else if(queue_data[0]==0x4C){  // L
                //
            }
            else if(queue_data[0]==0x53 && queue_data[1]==0x37){  // S7
                //

                dequeue(&queue_data[0])
            }
            else if(queue_data[0]==0x53 && queue_data[1]==0x38){  // S8
                //

                dequeue(&queue_data[0])
            }
            else if(queue_data[0]==0x53 && queue_data[1]==0x52){  // SR
                //

                dequeue(&queue_data[0])
            }
            else if(queue_data[0]==0x53 && queue_data[1]==0x4C){  // SL
                //

                dequeue(&queue_data[0])
            }
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

//    LATBbits.LATB3=1;
//    LATDbits.LATD3=1;
//    LATDbits.LATD7=1;
}
