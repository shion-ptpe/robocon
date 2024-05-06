#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// 任意のDuty比で出力する

#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define Voltage_switch_H (200);   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


void Duty_Calculation(double Duty);

int main(){
//    enum{Voltage_switch_H =3000};
    OC1RS = Voltage_switch_H;   // OCxTMRの最大値 = OCxRS (OCMでの設定より)任意で設定できる,リセットされたときにhighになる
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISA = 0xffff;
    TRISB = 0xffff;
    //　初期化
    OC1CON1 = 0x0000;
    OC1CON2 = 0x0000;



RPOR1bits.RP3R = 18;   //  使うピン
OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット

OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する


while(1){
    while(1){
    Duty_Calculation(50);  // 50%
    }}
    return 0;
}

void Duty_Calculation(double Duty){
    int a;
    a=Voltage_switch_H;   // aに代入せずにdefineを使うと、「invalid type argument of 'unary *' (have 'xxx')」というエラーが表示される。
    OC1R = a * (Duty*0.01);  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
    __delay_ms(1000)
}
