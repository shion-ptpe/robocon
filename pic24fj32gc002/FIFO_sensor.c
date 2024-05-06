#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_SIZE   64// QUEUE_SIZEはサイズの大きさ
#define SUCCESS 1
#define FAILURE 0
typedef unsigned char data_t;


    
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void init(void);
void UART1_config(void);  // 距離センサ
void UART2_config(void);  // PCとの接続

void process(char *list, double *Distance, double *Strength, double *Temperature);
void read(char * msg);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);

int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t TF_date;
data_t queue_data[QUEUE_SIZE];
double Distance, Strength, Temperature, CHECK, SUM;

int main(){   
    init();
    UART1_config();
    UART2_config();
    
    int i;
    char data[64];
    
    
    while(1){
        if(queue_data[0]==0x59 && queue_data[1]==0x59){
            process(&queue_data, &Distance, &Strength, &Temperature);
            CHECK=queue_data[8];
            SUM=(queue_data[0]+queue_data[1]+queue_data[2]+queue_data[3]+queue_data[4]+queue_data[5]+queue_data[6]+queue_data[7])&0xFF;
            if(0<Distance  && CHECK==SUM){ // Strength<100かDistance<0か
                sprintf(data,"Distance(m):%lf",Distance/100);
                read(data);
                sprintf(data,"Strength:%lf",Strength);
                read(data);
                sprintf(data,"Temperature:%lf",Temperature/8256);
                read(data);
                __delay_ms(1000)
                for(i=0;i<QUEUE_SIZE;i++){
                    queue_data[i]=0;
                }
            }
        }
        dequeue(&queue_data[0]);
    }
    return 0;
}

void process(char *list,double *Distance, double *Strength, double *Temperature){
    *Distance=((list[3]<<8) | list[2]);
    *Strength=((list[5]<<8) | list[4]);
    *Temperature=((list[6]<<8) | list[7]);
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

// 読み取ったデータを配列から出す関数dequeue(一文字ずつ取り出す)  // 配列を一つ右に進める
int dequeue(data_t *deq_data){  // 受け取る引数は、処理する部分のアドレス。*deq_dataは処理するデータ
    if (num > 0) {  // 0以上なら
        *deq_data = queue_data[head];  // deq_dataは引数として渡されている
        head = (head + 1) % QUEUE_SIZE;  // 先頭が変わるから割る
        num --;
        return SUCCESS;
    } else {
        return FAILURE;
    }
}void init(void){
   OSCCON = 0x0000;
   CLKDIV = 0x0000;
   AD1PCFG = 0xFFFF;
   TRISA = 0xffff;  //入力(初期化)
   TRISB = 0xffff;  // 入力
   U1MODE = 0x0000;
   U1STA = 0x0000;

   TRISBbits.TRISB9=1;  // UART2のTX(送信)
   TRISBbits.TRISB7 = 0;  // UART2のRX(受信)
   TRISBbits.TRISB5 = 0;  // LED
   TRISBbits.TRISB15= 0;  // UART1のTX(送信)、距離センサのRXと繋ぐ
   TRISBbits.TRISB13 = 1;  // UART1のRX(受信)、距離センサのTXと繋ぐ

// ピンの設定
// UxTXピンを使用するためのコード
   RPOR3bits.RP7R = 5;  // (U2TX)。使うピン=RP9（RB9）, U1TX(RPnをUART2送信に接続)
   RPOR7bits.RP15R = 3;// (U1TX)。使うピン=RP15（RB15）, U1TX(RPnをUART1送信に接続)
// UxRXピンを使用するためのコード
   RPINR19bits.U2RXR= 9;  // (U2RX)。使うピン=RP7(RB7), UART2 受信
   RPINR18bits.U1RXR= 13;  // (U1RX)。使うピン=RP13(RB13), UART1 受信
}

void UART1_config(void){
   U1MODEbits.BRGH=1;  // 「0」ボーレート低速、「1」ボーレート高速
   // UxBRGレジスタ(ボーレート)
   U1BRG=8;  // (115200)Set Baudrate(ボートレートの設定)
   
   U1MODEbits.PDSEL0=0;  // 9ビットデータ、パリティなし
   U1MODEbits.PDSEL1=0;  // 9ビットデータ、パリティなし
   U1MODEbits.STSEL=0;  // 1ストップビット
   
// 受信割り込み
   IEC0bits.U1RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
   IPC2bits.U1RXIP=5;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定7段階(0は割り込み無効))

// 受信割り込みを制御する
   U1STAbits.URXISEL1=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。
   U1STAbits.URXISEL0=0;  // 「11」受信バッファが一杯になった時割り込みフラグをセット


   U1MODEbits.UARTEN=1;  // UTRTxを有効化する。
   U1STAbits.UTXEN=1;  // UART送信を有効化する。
   
   IFS0bits.U1RXIF = 0;  // フラグを下ろす
}


void UART2_config(void){
   U2MODEbits.BRGH=0;  // ボーレート低速
   // UxBRGレジスタ(ボーレート)
   U2BRG=25;  // (9600)Set Baudrate(ボートレートの設定)
//   U1MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする
 
   U2MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
   U2MODEbits.STSEL=0;  // 1ストップビット

// 受信割り込み
   IEC1bits.U2RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
   IPC7bits.U2RXIP=5;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定7段階(0は割り込み無効))

// 受信割り込みを制御する
   U2STAbits.URXISEL1=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。
   U2STAbits.URXISEL0=0;  // 「11」受信バッファが一杯になった時割り込みフラグをセット

   
   U2MODEbits.UARTEN=1;  // UTRTxを有効化する。
   U2STAbits.UTXEN=1;  // UART送信を有効化する。
   
   IFS1bits.U2RXIF = 0;  // フラグを下ろす
}
