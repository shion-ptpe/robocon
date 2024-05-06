#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// 任意のDuty比で出力する

#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define Voltage_switch_H (200);   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


int Duty_Calculation(double Duty);

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



RPOR3bits.RP7R = 18;   //  使うピン
RPOR4bits.RP8R = 19;   //  使うピン
RPOR7bits.RP14R = 20;   //  使うピン
RPOR7bits.RP15R = 21;   //  使うピン

OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する

//OC2CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
//OC2CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
//OC2CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する
//
//OC3CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
//OC3CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
//OC3CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する
//
//OC4CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
//OC4CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
//OC4CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する

    TRISAbits.TRISA3=0;  // S1
    TRISBbits.TRISB4=0;  // S1
//    TRISAbits.TRISA4=0;  // S2
//    TRISBbits.TRISB5=0;  // S2
//    TRISBbits.TRISB9=0;  // S3
//    TRISBbits.TRISB10=0;  // S3
//    TRISBbits.TRISB11=0;  // S4
//    TRISBbits.TRISB13=0;  // S4


while(1){
    Duty_Calculation(50);  // 50%
    LATAbits.LATA3=1;  // S1
    LATBbits.LATB4=0;  // S1
    ｓｓ = (Duty_Calculation(99));  // M1
    __delay_ms(1000)
    OC1R = (Duty_Calculation(0));
    __delay_ms(500)
    LATAbits.LATA3=0;  // S1
    LATBbits.LATB4=1;  // S1
    OC1R = (Duty_Calculation(10));  // M2
    __delay_ms(1000)
    OC1R = (Duty_Calculation(0));
    __delay_ms(500)


//    OC3R = (Duty_Calculation(50));  // M3
//    __delay_ms(1000)
//    OC4R = (Duty_Calculation(50));  // M4
//    __delay_ms(1000)
    }
    return 0;

}
int Duty_Calculation(double Duty){
    int Voltage, OC_config;
    Voltage =Voltage_switch_H
    OC_config = Voltage * (Duty*0.01);  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
    return OC_config;
}
