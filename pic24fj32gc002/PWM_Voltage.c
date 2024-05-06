#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// 任意のVを出力する

void Duty_Calculation(double MAX_Voltage, double Voltage, double Duty, double Voltage_switch_H, double * pVoltage_switch_L);

int main(){
    double MAX_Voltage = 3.3;   //最大の電圧(マイコンの電圧)
    double Voltage = 1.4;      // 出したい電圧 (必ず決める)
    double Duty;
    double Voltage_switch_H = 3000;   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)
    double Voltage_switch_L;

    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISA = 0xffff;
    TRISB = 0xffff;
    //　初期化
    OC1CON1 = 0x0000;
    OC1CON2 = 0x0000;


RPOR3bits.RP7R = 18;   //  使うピン
OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット

OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する


while(1){
    Duty_Calculation(MAX_Voltage, Voltage, Duty, Voltage_switch_H, &Voltage_switch_L);
    OC1R = Voltage_switch_L;   // low(OCMでの設定より)
    OC1RS = Voltage_switch_H;   // OCxTMRの最大値 = OCxRS (OCMでの設定より)任意で設定できる,リセットされたときにhighになる

    __delay_ms(1000)
   }

   return 0;
}

void Duty_Calculation(double MAX_Voltage, double Voltage, double Duty, double Voltage_switch_H, double * pVoltage_switch_L){
    Duty = Voltage/MAX_Voltage;
    *pVoltage_switch_L = Voltage_switch_H*Duty;
}

// while(1){   // PWM 90%のコード
//     LATBbits.LATB5=1;
//     __delay_ms(100)
//     LATBbits.LATB5=1;
//     __delay_ms(900)
// }


/* 書き換えたコード
#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

// 任意のVを出力する

#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define Voltage_switch_H = 3000;   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


void Duty_Calculation(double Voltage);

int main(){
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
OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット

OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する


while(1){
    Duty_Calculation(1.4);
    __delay_ms(1000)
   }

   return 0;
}

void Duty_Calculation(double Voltage){   // Voltage=出したい電圧 (必ず決める)
    double Duty
    Duty = Voltage/MAX_Voltage;   // Duty比はどれくらいかを計算
    OC1R = Voltage_switch_H*Duty;  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
}
*/
