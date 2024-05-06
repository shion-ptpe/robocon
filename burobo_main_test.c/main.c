#include <xc.h>
#define FCY 4000000
#include "config.h"
#include "user.h"
#include "function.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

extern double angle[];
extern int num;
extern union bit_look bi;


extern data_t queue_data[QUEUE_SIZE];

void config();


int main(){
    int total;
    char data[256];
    int i;

    config();
    motor_config();
    UART1_config();
    UART2_config();
//    rotary_encoder_config();

    motor_stop();
    while(1){

//        sprintf(data,"angle[0]:%f\nangle[1]:%f\nangle[2]:%f\nangle[3]:%f\nangle[4]:%f\n\n", angle[0]*0.17578125, angle[1]*0.17578125, angle[2]*0.17578125, angle[3]*0.17578125, angle[4]*0.17578125);
//        read(data);

        while(queue_data[0]!=0x80){
            dequeue(&queue_data[0]);
        }

        if(num>8){
//            total=((queue_data[1]+queue_data[2]+queue_data[3]+queue_data[4]+queue_data[5]+queue_data[6])&0x7F);
            LATDbits.LATD7=~LATDbits.LATD7;  //yellow
//            if(queue_data[0]==0x80 && total == queue_data[7]){


//                sprintf(data,"queue_data[0]:%x\nqueue_data[1]:%x\nqueue_data[2]:%x\nqueue_data[3]:%x\nqueue_data[4]:%x\nqueue_data[5]:%x\nqueue_data[6]:%x\nqueue_data[7]:%x\n\n", queue_data[0], queue_data[1], queue_data[2], queue_data[3], queue_data[4], queue_data[5], queue_data[6], queue_data[7]);
//                read(data);
                LATDbits.LATD3=~LATDbits.LATD3;  //red

                if(queue_data[1]==0x00 && queue_data[2]==0x00){  // アナログスティック
                    if(queue_data[3]==0x40 && queue_data[4]==0x40){
//                        stick_analysis(queue_data[5], queue_data[6]);  // 右アナログステック
                    }
                    else if(queue_data[5]==0x40 && queue_data[6]==0x40){
//                        stick_analysis(queue_data[3], queue_data[4]);  // 左アナログステック
                    }
                    else{
                        // pass
                    }
                }
                else{
//                    motor_stop();

                    bi.data = ((queue_data[1]<<8) | queue_data[2]);  // ボタン
                    button_analysis();
                }
//            }
            for(i=0;i<=7;i++){
                dequeue(&queue_data[0]);
            }
//            total=0;
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
