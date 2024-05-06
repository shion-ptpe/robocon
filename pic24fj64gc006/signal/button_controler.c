#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define QUEUE_SIZE 1024
#define SUCCESS 1
#define FAILURE 0
typedef unsigned char data_t;

void config();
void UART1_config();
void UART2_config();
void read(char * msg);
void button_analysis();
void stick_analysis(data_t *x, data_t *y);


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);

int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t TF_date;
data_t queue_data[QUEUE_SIZE];

struct field{
    data_t b1 : 1;
    data_t b2 : 1;
    data_t b3 : 1;
    data_t b4 : 1;
    data_t b5 : 1;
    data_t b6 : 1;
    data_t b7 : 1;
    data_t b8 : 1;
    data_t b9 : 1;
    data_t b10 : 1;
    data_t b11 : 1;
    data_t b12 : 1;
    data_t b13 : 1;
    data_t b14 : 1;
    data_t b15 : 1;
    data_t b16 : 1;
};

union{
    unsigned short int data;
    struct field bits;
} bi;

int main(){
    config();
    UART1_config();
    UART2_config();
    double total;
    char data[256];
    int i;

    while(1){

        while(queue_data[0]!=0x80 || total != queue_data[7]){
            dequeue(&queue_data[0]);
            LATDbits.LATD3=~LATDbits.LATD3;
            break;
        }
        LATDbits.LATD7=~LATDbits.LATD7;
        total=((queue_data[1]+queue_data[2]+queue_data[3]+queue_data[4]+queue_data[5]+queue_data[6])&0x7F);
        if(queue_data[0]==0x80 && total == queue_data[7]){
            sprintf(data,"queue_data[0]:%x\nqueue_data[1]:%x\nqueue_data[2]:%x\nqueue_data[3]:%x\nqueue_data[4]:%x\nqueue_data[5]:%x\nqueue_data[6]:%x\nqueue_data[7]:%x\n", queue_data[0], queue_data[1], queue_data[2], queue_data[3], queue_data[4], queue_data[5], queue_data[6], queue_data[7]);
            read(data);

            if(queue_data[1]==0x00 && queue_data[2]==0x00){  // アナログスティック
                if(queue_data[3]==0x40 && queue_data[4]==0x40){
                    stick_analysis(&queue_data[5], &queue_data[6]);  // 右アナログステック
                }
                else{
                    stick_analysis(&queue_data[3], &queue_data[4]);  // 左アナログステック
                }
            }
            else{
                bi.data = ((queue_data[1]<<8) | queue_data[2]);  // ボタン
                button_analysis();

            }



            for(i=0;i<=7;i++){
                dequeue(&queue_data[0]);
            }
        }
   }
   return 0;
}

void button_analysis(){
    if(bi.bits.b1){ // 0000_0001
        U2TXREG = 'A';  // ↑
    }
    if(bi.bits.b2){  // 0000_0010
        U2TXREG = 'B';  // ↓
    }
    if(bi.bits.b3){  // 0000_0100
        U2TXREG = 'C';  // →
    }
    if(bi.bits.b4){  // 0000_1000
        U2TXREG = 'D';  // ←
    }
    if(bi.bits.b5){  // 0001_0000
        U2TXREG = 'E';  // △
    }
    if(bi.bits.b6){  // 0010_0000
        U2TXREG = 'F';  // ×
    }
    if(bi.bits.b7){  // 0100_0000
        U2TXREG = 'G';  // ○
    }
    if(bi.bits.b9){  // 0000_0001
        U2TXREG = 'H';  // □
    }
    if(bi.bits.b10){  // 0000_0010
        U2TXREG = 'I';  // L1
    }
    if(bi.bits.b11){  // 0000_0100
        U2TXREG = 'J';  // L2
    }
    if(bi.bits.b12){  // 0000_1000
        U2TXREG = 'K';  // R1
    }
    if(bi.bits.b13){  // 0001_0000
        U2TXREG = 'L';  // R2
    }
    bi.data=0;
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

 //    LATBbits.LATB3=1;
 //    LATDbits.LATD3=1;
 //    LATDbits.LATD7=1;
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
