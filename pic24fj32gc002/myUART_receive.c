// FIFOの概念を無視している
#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>


void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void init(void);
void UART1_config(void);  // 距離センサ
void UART2_config(void);  // PCとの接続

void read(char * msg);

int U2interrupt_check=0;// 文字が入ったかをチェックする
char queue_data[1] = {};  // 1文字割り込みするごとにmain文の処理ができている?から[1]でも大丈夫?



int main(){
   init();
   UART1_config();
   UART2_config();

   while(1){
   if(U2interrupt_check==1){
      read(&queue_data[0]);
   }
      }
   return 0;
}

void read(char *msg){
    int i=0;
    while(msg[i]!='\0'){
        if (U2STAbits.TRMT){
            U2TXREG = msg[i];  // UARTx送信レジスタ(書き込み専用))
            i++;
        }
    }
    U2interrupt_check=0;
}


void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void){  // 文字が受信される都度割り込みフラグビットをセット。
    int count=0;    // 割り込みが入るたびに"count"が0になるからリストを上書きし続けられる

    queue_data[count] = U2RXREG;
    count ++;
//
//    if(count==MAX_capacity-1){
//        count=0;
//    }
//    queue_data[count] = 0xA;
//    count ++;
    U2interrupt_check=1;

    IFS1bits.U2RXIF = 0;  // フラグを消す
}

void init(void){
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
//   U2MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする

   U1MODEbits.PDSEL0=1;  // 9ビットデータ、パリティなし
   U1MODEbits.PDSEL1=1;  // 9ビットデータ、パリティなし
   U1MODEbits.STSEL=0;  // 1ストップビット

// 受信割り込み
   IEC0bits.U1RXIE=1;  // Enable Receive Interrupt(受信割り込みを使用)
   IPC2bits.U1RXIP=5;  // Set UART RX Interrupt Priority(受信割り込みの優先度を設定7段階(0は割り込み無効))

// 受信割り込みを制御する
   U1STAbits.URXISEL1=1;  // 「11」受信バッファが一杯になった時割り込みフラグをセット
   U1STAbits.URXISEL0=1;  // 「11」受信バッファが一杯になった時割り込みフラグをセット


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
   U2STAbits.URXISEL0=0;  // 「0x」で文字が受信される都度割り込みフラグビットをセット。


   U2MODEbits.UARTEN=1;  // UTRTxを有効化する。
   U2STAbits.UTXEN=1;  // UART送信を有効化する。

   IFS1bits.U2RXIF = 0;  // フラグを下ろす
}
