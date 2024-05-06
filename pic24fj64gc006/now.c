#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_SIZE 1024
#define SUCCESS 1
#define FAILURE 0
typedef unsigned char data_t;

void config();
void UART_config();
void rotary_encoder_config();
void CN_process();
void read(char * msg);


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);

int head;  // head?????????
int num;  // num?????
data_t TF_date;
data_t queue_data[QUEUE_SIZE];

double angle[]={0,0,0,0,0};

int main(){
    config();
    UART_config();
    rotary_encoder_config();

    char data[64];

    while(1){
        int i, e;
        e = ((0<PORTDbits.RD5) ? 0x41 : 0x42);
        i = ((0<PORTDbits.RD6) ? 0x41 : 0x42);
        U1TXREG = e;
        __delay_ms(100)
        U1TXREG = i;
        __delay_ms(100)
        U1TXREG =0x0A;
        __delay_ms(100)
//        for(i=0;i<5;i++){
//            sprintf(data,"CN1:%lf", angle[i]);
////            sprintf(data,"CN1:%lf", angle[i]*0.175);
//            read(data);
//            __delay_ms(100)
//        }
//        sprintf(data,"CN1:%f", angle[0]);
//        read(data);
//        sprintf(data,"%f", angle[0]);
//        read(data);
//        __delay_ms(100)

    }
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    CN_process();
    IFS1bits.CNIF=0;
}

void CN_process(){
    if (PORTDbits.RD5==1){
        angle[0]=angle[0]+ ((PORTDbits.RD6==1) ? 1 : -1);
    }
    if (PORTFbits.RF1==1){
        angle[1]=angle[1] + ((PORTFbits.RF0==1) ? 1 : -1);
    }
    if (PORTEbits.RE2==1){
        angle[2]=angle[2] + ((PORTEbits.RE1==1) ? 1 : -1);
    }
    if (PORTEbits.RE3==1){
        angle[3]=angle[3] + ((PORTEbits.RE4==1) ? 1 : -1);
    }
    if (PORTEbits.RE6==1){
        angle[4]=angle[4] + ((PORTEbits.RE5==1) ? 1 : -1);
    }
}


void rotary_encoder_config(void){
    CNEN1bits.CN14IE=1;// RD5__CN14__CN1-1
    CNEN1bits.CN15IE=1; // RD6__CN15__CN1-2
    CNEN5bits.CN69IE=1; // RF1__CN69__CN2-1
    CNEN5bits.CN68IE=1; // RF0__CN68__CN2-2
    CNEN4bits.CN60IE=1; // RE2__CN60__CN3-1
    CNEN4bits.CN59IE=1; // RE1__CN59__CN3-2
    CNEN4bits.CN61IE=1; // RE3__CN61__CN4-1
    CNEN4bits.CN62IE=1; // RE4__CN62__CN4-2
    CNEN5bits.CN64IE=1; // RE6__CN64__CN5-1
    CNEN4bits.CN63IE=1; // RE5__CN63__CN5-2

    IEC1bits.CNIE=1;
    IFS1bits.CNIF=0;
    IPC4bits.CNIP = 5;
}


void read(char *msg){
    int i=0;
    while(msg[i]!='\0'){
        if (U1STAbits.TRMT){
            U1TXREG = msg[i];  // UARTx??????(??????))
            i++;
        }
    }
    U1TXREG =0x0A;
}

// ????????????????enqueue
int enqueue(data_t enq_data){  // ??????enqueue? queue???????????????unsigned??enq_data
    if (head + num < QUEUE_SIZE){  // queuedata????????QUEUE_SIZE????????
        queue_data[(head + num) % QUEUE_SIZE] = enq_data;   // QUEUE_SIZE?????????????????????????????,head?num??????????????????
        num ++;
        return SUCCESS;
    }else {
        return FAILURE;
    }
}

// ?????????????????dequeue(?????????)
int dequeue(data_t *deq_data){  // ????????????????????*deq_data????????
    if (num > 0) {  // 0????
        *deq_data = queue_data[head];  // deq_data????????????
        head = (head + 1) % QUEUE_SIZE;  // ??????????
        num --;
        return SUCCESS;
    } else {
        return FAILURE;
    }
}
void config(void){
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    TRISB = 0xffff;
    TRISD = 0xffff;
    TRISE = 0xffff;
    U1MODE = 0x0000;
    U1STA = 0x0000;

    ANSB = 0;
    ANSD = 0;
    ANSE = 0;

    TRISBbits.TRISB3 = 0;  // LED
    TRISDbits.TRISD3 = 0;  // LED
    TRISDbits.TRISD7 = 0;  // LED

//    LATBbits.LATB3=1;  green
//    LATDbits.LATD3=1;  red
//    LATDbits.LATD7=1;  yellow
}



void UART_config(void){
    TRISDbits.TRISD0= 0;  // TX
    TRISDbits.TRISD11 = 1;  // RX

    RPOR5bits.RP11R = 3;  // (U1TX)?????=RP11, U1TX(RPn?UART1?????)

    RPINR18bits.U1RXR = 12;  // (U1RX)?????=RP11, UART1 ??

    U1MODEbits.BRGH=0;  // ?0??????????1????????
   // UxBRG????(?????)
    U1BRG=25;  // (9600)Set Baudrate(?????????)
//   U1MODEbits.RXINV=0;  // ??????????UxRX?????????1????

    U1MODEbits.PDSEL=0;  // 8?????????????
    U1MODEbits.STSEL=0;  // 1???????

// ??????
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt(?????????)
    IPC2bits.U1RXIP=5;  // Set UART RX Interrupt Priority(?????????????1~7?)

// ???????????
    U1STAbits.URXISEL1=0;  // ?0x???????????????????????????
    U1STAbits.URXISEL0=0;  // ?0x???????????????????????????


    U1MODEbits.UARTEN=1;  // UTRTx???????
    U1STAbits.UTXEN=1;  // UART?????????

    IFS0bits.U1RXIF = 0;  // ???????

}
