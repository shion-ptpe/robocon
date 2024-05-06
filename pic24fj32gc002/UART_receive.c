#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>

#define QUEUE_SIZE 1024  // QUEUE_SIZEはサイズの大きさ
#define SUCCESS 1
#define FAILURE 0
typedef unsigned char data_t;

// FIFOのプログラム

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void UART_config(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);


int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t TF_date;
data_t queue_data[QUEUE_SIZE];

int main(){   
    UART_config();
    while(1){
        if(dequeue(&queue_data[0])){   // 「return SUCCESS;」によって、  配列を読み取っている間はif文が実行される。
            U1TXREG = queue_data[0];
        }        
    }
    return 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void){
    enqueue(U1RXREG);
    IFS0bits.U1RXIF = 0;
}
// 受信したデータを配列に入れる関数enqueue
int enqueue(data_t enq_data){  // 入れることをenqueue、 queueは引数としてデータを持ってる。unsigned型のenq_data
    if (head + num < QUEUE_SIZE){  // queuedataは配列のサイズ、QUEUE_SIZEはサイズの大きさ
        queue_data[(head + num) % QUEUE_SIZE] = enq_data;   // QUEUE_SIZEというデータのサイズで割ることで、循環する様子を表現できる,headとnumからデータが埋まってないかを確認する
        num ++;
        return SUCCESS; 
    }else {
        return FAILURE;
    }
}

// 読み取ったデータを配列から出す関数dequeue(一文字ずつ取り出す)
int dequeue(data_t *deq_data){  // 受け取る引数は、処理する部分のアドレス。*deq_dataは処理するデータ
    if (num > 0) {  // 0以上なら
        *deq_data = queue_data[head];  // deq_dataは引数として渡されている
        head = (head + 1) % QUEUE_SIZE;  // 先頭が変わるから割る
        num --;
        return SUCCESS;
    } else {
        return FAILURE;
    }
}


void UART_config(void){
   OSCCON = 0x0000;
   CLKDIV = 0x0000;
   AD1PCFG = 0xFFFF;
   TRISA = 0xffff;  //入力(初期化)
   TRISB = 0xffff;  // 入力
   U1MODE = 0x0000;
   U1STA = 0x0000;

   TRISBbits.TRISB8=0;  // TX(送信)
   TRISBbits.TRISB14 = 1;  // RX(受信)
   TRISBbits.TRISB5 = 0;  // LED

   // PC側の設定(UART1)
   // UxTXピンを使用するためのコード
   RPOR4bits.RP8R = 3;  // (U1TX)。使うピン=RP8（RB8）, U1TX(RPnをUART1送信に接続)
   // UxRXピンを使用するためのコード
   RPINR18bits.U1RXR= 14;  // (U1RX)。使うピン=RP14（RB14）, UART1 受信

   U1MODEbits.BRGH=0;  // 「0」ボーレート低速、「1」ボーレート高速
   // UxBRGレジスタ(ボーレート)
   U1BRG=25;  // (9600)Set Baudrate(ボートレートの設定)
//   U1MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする
   
   U1MODEbits.PDSEL=0;  // 8ビットデータ、パリティなし
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
