#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_SIZE 1024  // QUEUE_SIZEはサイズの大きさ
#define SUCCESS 1
#define FAILURE 0
typedef unsigned char data_t;

void config();
void UART_config();
void rotary_encoder_config();
void CN_process();
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);



void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);

int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t TF_date;
data_t queue_data[QUEUE_SIZE];

double angle[]={0,0,0,0,0};

int main(){
    TRISBbits.TRISB3=0;
    TRISDbits.TRISD3=0;
    TRISDbits.TRISD7=0;
    LATBbits.LATB3=1;
    LATDbits.LATD3=1;
    LATDbits.LATD7=1;
    __delay_ms(1000)

    config();
    UART_config();
    rotary_encoder_config();
    while(1){
        int i;
        for(i=0;i<5;i++){
            enqueue(angle[i]*0.175);  // 分解能2048?
        }
        __delay_ms(1000)
        if(dequeue(&queue_data[0])){   // 「return SUCCESS;」によって、  配列を読み取っている間はif文が実行される。
            U1TXREG = queue_data[0];
        }
    }
    return 0;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    CN_process();
    _CNIF = 0;  // 割り込みフラグを下げる
}

void CN_process(){
    if (PORTDbits.RD5==1){
        angle[0]=(PORTDbits.RD6==1) ? angle[1]+1 : angle[1]-1;
    }
    if (PORTFbits.RF1==1){
        angle[1]=(PORTFbits.RF0==1) ? angle[2]+1 : angle[2]-1;
    }
    if (PORTEbits.RE2==1){
        angle[2]=(PORTEbits.RE1==1) ? angle[3]+1 : angle[3]-1;
    }
    if (PORTEbits.RE3==1){
        angle[3]=(PORTEbits.RE4==1) ? angle[4]+1 : angle[4]-1;
    }
    if (PORTEbits.RE6==1){
        angle[4]=(PORTEbits.RE5==1) ? angle[5]+1 : angle[5]-1;
    }
}


void rotary_encoder_config(void)
{
    //レジスタ内の適切なビットを設定する事により、選択したCNピンの割り込みを有効にする
    CNEN1bits.CN14IE=1; // RD5__CN14__CN1-1
    CNEN1bits.CN15IE=1; // RD6__CN15__CN1-2
    CNEN5bits.CN69IE=1; // RF1__CN69__CN2-1
    CNEN5bits.CN68IE=1; // RF0__CN68__CN2-2
    CNEN4bits.CN60IE=1; // RE2__CN60__CN3-1
    CNEN4bits.CN59IE=1; // RE1__CN59__CN3-2
    CNEN4bits.CN61IE=1; // RE3__CN61__CN4-1
    CNEN4bits.CN62IE=1; // RE4__CN62__CN4-2
    CNEN5bits.CN64IE=1; // RE6__CN64__CN5-1
    CNEN4bits.CN63IE=1; // RE5__CN63__CN5-2

    IFS1bits.CNIF=0;  // 割り込みのフラグを下げる。入力変化通知割り込みフラグステータスビット
    IPC4bits.CNIP1 = 1; // 割り込み優先度の設定。入力変化通信割り込み優先ビット

    IEC1bits.CNIE=1;  // 外部割り込みイネーブルビット(1=割り込み要求が有効)  // 入力変化通知割り込みイネーブルビット(1=割り込み要求を有効にする)
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
void config(void){
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    TRISB = 0xffff;
    TRISD = 0xffff;
    TRISE = 0xffff;
    U1MODE = 0x0000;
    U1STA = 0x0000;

    TRISBbits.TRISB3 = 0;  // LED
    TRISDbits.TRISD3 = 0;  // LED
    TRISDbits.TRISD7 = 0;  // LED

//    LATBbits.LATB3=1;
//    LATDbits.LATD3=1;
//    LATDbits.LATD7=1;
}



void UART_config(void){
    TRISDbits.TRISD0= 0;  // TX
    TRISDbits.TRISD11 = 1;  // RX

    RPOR5bits.RP11R = 3;  // (U1TX)。使うピン=RP11, U1TX(RPnをUART1送信に接続)

    RPINR18bits.U1RXR = 12;  // (U1RX)。使うピン=RP11, UART1 受信

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
