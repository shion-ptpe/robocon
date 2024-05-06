#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// PWSの基本的な設定をする。

int main(){
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISA = 0xffff;
    TRISB = 0xffff;

    //　初期化
    OC1CON1 = 0x0000;
    OC1CON2 = 0x0000;

    OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
    OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット  (エッジ整列PWMモード: 出力はOCxTMR=0の時にhigh, OCxTMR=OCxRの時にLowにする。)

    OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する
    OC1R = 0x00 ;    // low(OCMでの設定より、OC1Rはlowにするタイミングを表す。)
    OC1RS = 200;  // リセット(high),OCxTMRの最大値 = OCxRS (OCMでの設定より)任意で設定できる

    RPOR3bits.RP7R = 18;   //  使うピン

    while(1){     // これまで書いた文は設定だから、while{}を書かないと動作しない
        __delay_ms(10);
    }
    return 0;
}
