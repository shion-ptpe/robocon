#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// 音階を演奏する
#define C3 130    // ド
#define D3 146    // レ
#define E3 164    // ミ
#define F3 174    // ファ
#define G3 195    // ソ
#define A3 220    // ラ
#define B3 246    // シ

#define C4 261    // ド
#define D4 293    // レ
#define E4 329    // ミ
#define F4 349    // ファ
#define G4 391    // ソ
#define A4 440    // ラ
#define B4 494    // シ

#define C5 523    // ド
#define D5 587    // レ
#define E5 659    // ミ
#define F5 698    // ファ
#define G5 783    // ソ
#define A5 880    // ラ
#define B5 987    // シ

int main(){
    int i;
    double ans;
    double clock=4000000;
    int note[]={261, 293, 329, 349, 391, 440, 494, 523};

   OSCCON = 0x0000;
   CLKDIV = 0x0000;
   AD1PCFG = 0xFFFF;
   TRISA = 0xffff;
   TRISB = 0xffff;
   //　初期化
   OC1CON1 = 0x0000;
   OC1CON2 = 0x0000;

   OC1R = 5000;   // low(OCMでの設定より)

RPOR3bits.RP7R = 18;   //  使うピン
OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット

OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する


while(1){
    for(i=0;i<=9;i++){
       ans=clock/note[i];
       OC1RS = ans;   // OCxTMRの最大値 = OCxRS (OCMでの設定より)任意で設定できる,リセットされたときにhighになる
       __delay_ms(500)
   }
}
   return 0;
}
