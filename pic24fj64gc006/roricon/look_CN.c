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
void UART1_config();
void UART2_config();
void read(char * msg);

void rotary_encoder_config();
void CN_process();
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);



void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);

int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t TF_date;
data_t queue_data[QUEUE_SIZE];



double angle[]={0,0,0,0,0};


int main(){
    config();
    UART1_config();
    UART2_config();
    rotary_encoder_config();
    char data[256];

    while(1){
        sprintf(data,"angle[0]:%f\nangle[1]:%f\nangle[2]:%f\nangle[3]:%f\nangle[4]:%f\n", angle[0]*0.17578125, angle[1]*0.17578125, angle[2]*0.17578125, angle[3]*0.17578125, angle[4]*0.17578125);
        read(data);
        __delay_ms(500)
    }
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    CN_process();
    IFS1bits.CNIF=0;
}

void CN_process(){
    angle[0]+=PORTDbits.RD5;  // 反時計回り
    angle[0]-=PORTDbits.RD6;  // 時計回り
//    angle[1]+=PORTFbits.RF1;
//    angle[1]-=PORTFbits.RF0;
//    angle[2]+=PORTEbits.RE2;
//    angle[2]-=PORTEbits.RE1;
//    angle[3]+=PORTEbits.RE3;
//    angle[3]-=PORTEbits.RE4;
//    angle[4]+=PORTEbits.RE6;
//    angle[4]-=PORTEbits.RE5;
}

void rotary_encoder_config(void){
    CNEN1bits.CN14IE=1;// RD5__CN14__CN1-1
    CNEN1bits.CN15IE=1; // RD6__CN15__CN1-2
//    CNEN5bits.CN69IE=1; // RF1__CN69__CN2-1
//    CNEN5bits.CN68IE=1; // RF0__CN68__CN2-2
//    CNEN4bits.CN60IE=1; // RE2__CN60__CN3-1
//    CNEN4bits.CN59IE=1; // RE1__CN59__CN3-2
//    CNEN4bits.CN61IE=1; // RE3__CN61__CN4-1
//    CNEN4bits.CN62IE=1; // RE4__CN62__CN4-2
//    CNEN5bits.CN64IE=1; // RE6__CN64__CN5-1
//    CNEN4bits.CN63IE=1; // RE5__CN63__CN5-2

    IEC1bits.CNIE=1;
    IFS1bits.CNIF=0;
    IPC4bits.CNIP = 5;
}

void read(char *msg){
   int i=0;
   while(msg[i]!='\0'){
       if (U2STAbits.TRMT){
           U2TXREG = msg[i];  // UARTx送信レジスタ(書き込み専用))
           i++;
       }
   }
   U2TXREG =0x0A;
}

void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void){  // コントローラー
   enqueue(U1RXREG);
   IFS0bits.U1RXIF = 0;
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){  // UART
//   enqueue(U2RXREG);
   IFS1bits.U2RXIF = 0;
}


int enqueue(data_t enq_data){
   if (head + num < QUEUE_SIZE){
       queue_data[(head + num)% QUEUE_SIZE] = enq_data;
       num ++;
       return SUCCESS;
   }else {
       return FAILURE;
   }
}

int dequeue(data_t *deq_data){
   if (num > 0) {
       *deq_data = queue_data[head];
       head = (head + 1) % QUEUE_SIZE;
       num --;
       return SUCCESS;
   } else {
       return FAILURE;
   }
}

void config(){
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

//    LATBbits.LATB3=1;  //green
//    LATDbits.LATD3=1;  //red
//    LATDbits.LATD7=1;  //yellow
}

void UART1_config(void){
    TRISDbits.TRISD0= 0;  // TX1
    TRISDbits.TRISD11 = 1;  // RX1

    RPOR5bits.RP11R = 3;  // (U1TX)。使うピン=RP11, U1TX(RPnをUART1送信に接続)
    RPINR18bits.U1RXR = 12;  // (U1RX)。使うピン=RP11, UART1 受信

    U1MODEbits.BRGH=1;  // 「0」ボーレート低速、「1」ボーレート高速
   // UxBRGレジスタ(ボーレート)
    U1BRG=25;  // 38400)Set Baudrate(ボートレートの設定)

    U1MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
    U1MODEbits.STSEL=0;  // 1ストップビット

 // 受信割り込み
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
    IPC2bits.U1RXIP=5;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定1~7?)

 // 受信割り込みを制御する
    U1STAbits.URXISEL1=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。
    U1STAbits.URXISEL0=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。


    U1MODEbits.UARTEN=1;  // UTRTxを有効化する。
    U1STAbits.UTXEN=1;  // UART送信を有効化する。

    IFS0bits.U1RXIF = 0;  // フラグを下ろす
}

void UART2_config(void){
    TRISDbits.TRISD2= 0;  // TX2
    TRISDbits.TRISD1 = 1;  // RX2

    RPOR12bits.RP24R = 5;  // (U2TX)。使うピン=RP24, U2TX(RPnをUART1送信に接続)
    RPINR19bits.U2RXR = 23;  // (U2RX)。使うピン=RP23, UART2受信

    U2MODEbits.BRGH=0;  // 「0」ボーレート低速、「1」ボーレート高速

    // UxBRGレジスタ(ボーレート)
    U2BRG=25;  // (9600)Set Baudrate(ボートレートの設定)

    U2MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
    U2MODEbits.STSEL=0;  // 1ストップビット

 // 受信割り込み
    IEC1bits.U2RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
    IPC7bits.U2RXIP=5;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定7段階(0は割り込み無効))

 // 受信割り込みを制御する
    U2STAbits.URXISEL1=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。
    U2STAbits.URXISEL0=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。


    U2MODEbits.UARTEN=1;  // UTRTxを有効化する。
    U2STAbits.UTXEN=1;  // UART送信を有効化する。

    IFS1bits.U2RXIF = 0;  // フラグを下ろす
}
