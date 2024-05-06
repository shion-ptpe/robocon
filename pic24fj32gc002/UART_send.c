#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>


void send(char * msg);

int main(){
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISA = 0xffff;  //出力(初期化)
    TRISB = 0xffff;  //出力
    U1MODE = 0x0000;
    U1STA = 0x0000;

   TRISBbits.TRISB8=0;  // TX(送信)
   TRISBbits.TRISB14 = 1;  // RX(受信)
   TRISBbits.TRISB5 = 0;  // LED
   // PC側の設定(UART1)
   // UxTXピンを使用するためのコード
   RPOR4bits.RP8R = 3;  // (U1TX)。使うピン=RP8（RB8）, U1TX(RPnをUART1送信に接続)
//   // UxRXピンを使用するためのコード
//   RPINR18bits.U1CTSR=14;  // (U1RX)。使うピン=RP14（RB14）, UART1 受信
//
//
//
//
   U1MODEbits.BRGH=0;  // ボーレート低速
   // UxBRGレジスタ(ボーレート)
   U1BRG=25;  // (9600)Set Baudrate(ボートレートの設定)
//   U1MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする



   U1MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
   U1MODEbits.STSEL=0;  // 1ストップビット

   U1STAbits.UTXISEL1=00;  // 仮決め、送信シフトレジスタに転送された時割り込み発生。
   U1MODEbits.UARTEN=1;  // UTRTxを有効化する。
   U1STAbits.UTXEN=1;  // UART送信を有効化する。
   IFS0bits.U1TXIF=0;  // 「=1」で割り込みフラグを立てる, 「=0」で割り込みフラグをリセット

   while(1){
       send("Hello_world");

   }
   return 0;

}


void send(char *msg){
    int i;
    for(i=0; i<strlen(msg); i++) {
        if(U1STAbits.TRMT){
        U1TXREG = msg[i];
        }
        else{
            --i;
        }
    }
    i=0;
    U1TXREG = 0xA;
}

//#include <xc.h>
//#include "config.h"
//#define FCY 4000000
//#include <libpic30.h>
//#include <stdio.h>
//#include <string.h>
//
//
//void send(char * msg);
//
//int main(){
//    OSCCON = 0x0000;
//    CLKDIV = 0x0000;
//    AD1PCFG = 0xFFFF;
//    TRISA = 0xffff;  //出力(初期化)
//    TRISB = 0xffff;  //出力
//    U1MODE = 0x0000;
//    U1STA = 0x0000;
//
//   TRISBbits.TRISB8=0;  // TX(送信)
//   TRISBbits.TRISB14 = 1;  // RX(受信)
//   TRISBbits.TRISB5 = 0;  // LED
//
//   // UxTXピンを使用するためのコード
//   RPOR4bits.RP8R = 5;  // (U1TX)。使うピン=RP8（RB8）, U1TX(RPnをUART1送信に接続)
//
//
//   U2MODEbits.BRGH=0;  // ボーレート低速
//   // UxBRGレジスタ(ボーレート)
//   U2BRG=25;  // (9600)Set Baudrate(ボートレートの設定)
////   U1MODEbits.RXINV=0;  // 受信極性反転ビット、UxRXのアイドル状態を「1」にする
//
//   U2MODEbits.PDSEL=00;  // 8ビットデータ、パリティなし
//   U2MODEbits.STSEL=0;  // 1ストップビット
//
//
//   U2STAbits.UTXISEL1=00;  // 仮決め、送信シフトレジスタに転送された時割り込み発生。
//   U2MODEbits.UARTEN=1;  // UTRTxを有効化する。
//   U2STAbits.UTXEN=1;  // UART送信を有効化する。
//
//   IFS1bits.U2RXIF = 0;  // フラグを下ろす
//
//   while(1){
//       U2TXREG = 0x55;
//       //send("Hello_world");
//
//   }
//   return 0;
//
//}
//
//
//void send(char *msg){
//    int i;
//    for(i=0; i<strlen(msg); i++) {
//        if(U2STAbits.TRMT){
//        U2TXREG = msg[i];
//        }
//        else{
//            --i;
//        }
//    }
//    i=0;
//    U2TXREG = 0xA;
//}
