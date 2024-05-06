#include <xc.h>
#include "user.h"
#include "function.h"
#include <libpic30.h>


int head;  // headは配列の先頭の番号
int num;  // numはデータ数
data_t queue_data[QUEUE_SIZE];

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


void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void){  // コントローラー
    enqueue(U1RXREG);
    IFS0bits.U1RXIF = 0;
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){  // マイコン間の通信
//    enqueue(U2RXREG);
    IFS1bits.U2RXIF = 0;  // フラグを消す
}


void UART1_config(void){
    TRISDbits.TRISD0= 0;  // TX1
    TRISDbits.TRISD11 = 1;  // RX1

    RPOR5bits.RP11R = 3;  // (U1TX)。使うピン=RP11, U1TX(RPnをUART1送信に接続)
    RPINR18bits.U1RXR = 12;  // (U1RX)。使うピン=RP11, UART1 受信

    U1MODEbits.BRGH=0;  // 「0」ボーレート低速、「1」ボーレート高速
   // UxBRGレジスタ(ボーレート)
    U1BRG=25;  // (9600)Set Baudrate(ボートレートの設定)

    U1MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
    U1MODEbits.STSEL=0;  // 1ストップビット

// 受信割り込み
    IEC0bits.U1RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
    IPC2bits.U1RXIP=4;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定1~7?)

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
