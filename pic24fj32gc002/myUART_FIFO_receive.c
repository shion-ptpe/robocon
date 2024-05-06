// 配列の上書きはできているが、部分ごとの抽出ができていない。
// オーバーフロー時の出力画面が綺麗じゃない。

#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#define MAX_capacity 10
void UART_config(void);
void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void);

int count=0;
int interrupt_check=0;// 文字が入ったかをチェックする
char queue_data[MAX_capacity] = {};


void read(char * msg);


int main(){
    UART_config();
    while(1){
        if(interrupt_check==1){
            read(&queue_data[0]);
         }
        }
    return 0;
}

void read(char *msg){
    int i;
    __delay_ms(10)
    for(i=0; i<strlen(msg); i++) {
        if(msg[i]=='\0' || msg[i]=='\n'){
            U1TXREG = 0x2f;
            continue;
        }
        if(U1STAbits.TRMT){  // 送信シフト レジスタが空ビット。「1」最後の送信が完了した
        U1TXREG = msg[i];  // UARTx送信レジスタ(書き込み専用))
        }
        else{
            --i;
        }
    }
    i=0;
    interrupt_check=0;
}

//void read(char *msg){
//    int i=0;
//    while(msg[i]!='\0'){
//        if (U1STAbits.TRMT){
//            U1TXREG = msg[(i%MAX_capacity)];  // UARTx送信レジスタ(書き込み専用))
//            i++;
//        }
//    }
//    interrupt_check=0;
//}


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void){  // 文字が受信される都度割り込みフラグビットをセット。

    queue_data[count] = U1RXREG;
    count ++;

    if(count==MAX_capacity-1){
        count=0;
    }
//    queue_data[count] = 0xA;
//    count ++;
    interrupt_check=1;

    IFS0bits.U1RXIF = 0;  // フラグを消す
}

//// countで配列の場所を指定して++でずれる。一周まわったら上書きしていく
//void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void){  // 文字が受信される都度割り込みフラグビットをセット。
//
//    queue_data[count] = U1RXREG;
//    count ++;
//    interrupt_check=1;  // 文字が入ったかをチェックする
//
//    if(count==MAX_capacity-1){
//        count=0;
//    }
//    IFS0bits.U1RXIF = 0;  // フラグを消す
//
//}


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


   U1MODEbits.BRGH=0;  // ボーレート低速
   // UxBRGレジスタ(ボーレート)
   U1BRG=25;  // (9600)Set Baudrate(ボートレートの設定)
//   U1MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする

   U1MODEbits.PDSEL=0;  // 8ビットデータ、パリティなし
   U1MODEbits.STSEL=0;  // 1ストップビット


   // 割り込み関係の設定
//// 送信割り込み
//   IFS0bits.U1TXIF=0;  // 「=1」で割り込みフラグを立てる, 「=0」で割り込みフラグをリセット
//// UxTXIFビットの動作を制御する
//   U1STAbits.UTXISEL1=00;  // 仮決め、送信シフトレジスタに転送された時割り込み発生。
//   IEC0bits.U1TXIE=1;  // Enable Transmit Interrupt
//   IPC3bits.U1TXIP=1;  // 割り込み優先度制御レジスタ

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
