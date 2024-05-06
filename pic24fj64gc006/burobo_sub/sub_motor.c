#include <xc.h>
#include <libpic30.h>
#include "user.h"
#include "function.h"

void motor_control_move(int sign){
    if(sign==0x41){
        OC1R = (Duty_Calculation(99));
        OC2R = (Duty_Calculation(99));
        OC3R = (Duty_Calculation(99));
        OC4R = (Duty_Calculation(99));
        OC5R = 0;
        OC6R = 0;
        OC7R = 0;
        OC8R = 0;
    }
    else if(sign==0x42){
        OC1R = 0;
        OC2R = 0;
        OC3R = 0;
        OC4R = 0;
        OC5R = (Duty_Calculation(99));
        OC6R = (Duty_Calculation(99));
        OC7R = (Duty_Calculation(99));
        OC8R = (Duty_Calculation(99));
        if (U2STAbits.TRMT){
            U2TXREG = sign;  // UARTx送信レジスタ(書き込み専用))
        }
    }
    else{
        //
    }

}




int Duty_Calculation(double Duty){
    int Voltage, OC_config;
    Voltage =frequency;
    OC_config = Voltage * (Duty*0.01);  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
    return OC_config;
}

void motor_config(){
    TRISDbits.TRISD8 = 0;
    TRISFbits.TRISF3 = 0;
    TRISFbits.TRISF4 = 0;
    TRISFbits.TRISF5 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;
    TRISGbits.TRISG9 = 0;
    TRISGbits.TRISG8 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB7 = 0;

    OC1RS = 200;
    OC2RS = 200;
    OC3RS = 200;
    OC4RS = 200;
    OC5RS = 200;
    OC6RS = 200;
    OC7RS = 200;
    OC8RS = 200;
    OC9RS = 200;

    RPOR1bits.RP2R = 18;   //  OC1, M1S1, M1S2, (RP2-RP16)  M1enc
    RPOR5bits.RP10R = 19;   //  OC2, M2S1, M2S2, (RP10-RP17)  M2enc
    RPOR7bits.RP14R = 20;   //  OC3, M3S1, M3S2, (RP14-RP29)  M3enc
    RPOR13bits.RP27R = 21;   //  OC4, M4S1, M4S2, (RP27-RP19)  M4enc
    RPOR8bits.RP16R = 22;   //  OC5, M1S1, M1S2, (RP2-RP16)  M1enc
    RPOR8bits.RP17R = 23;   //  OC6, M2S1, M2S2, (RP10-RP17)  M2enc
    RPOR14bits.RP29R = 24;   //  OC7, M3S1, M3S2, (RP14-RP29)  M3enc
    RPOR9bits.RP19R = 25;   //  OC8, M4S1, M4S2, (RP27-RP19)  M4enc


//    RPORxbits.RPxR = 22;   //  OC5, M5S1, M5S2, (RP28-RP6)  M5enc
//    RPORxbits.RPxR = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)  M6enc


    OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
    OC2CON1bits.OCTSEL = 0b111;
    OC3CON1bits.OCTSEL = 0b111;
    OC4CON1bits.OCTSEL = 0b111;
    OC5CON1bits.OCTSEL = 0b111;
    OC6CON1bits.OCTSEL = 0b111;
    OC7CON1bits.OCTSEL = 0b111;
    OC8CON1bits.OCTSEL = 0b111;
    OC9CON1bits.OCTSEL = 0b111;

    OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
    OC2CON1bits.OCM=0b110;
    OC3CON1bits.OCM=0b110;
    OC4CON1bits.OCM=0b110;
    OC5CON1bits.OCM=0b110;
    OC6CON1bits.OCM=0b110;
    OC7CON1bits.OCM=0b110;
    OC8CON1bits.OCM=0b110;
    OC9CON1bits.OCM=0b110;

    OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する
    OC2CON2bits.SYNCSEL = 0b11111;
    OC3CON2bits.SYNCSEL = 0b11111;
    OC4CON2bits.SYNCSEL = 0b11111;
    OC5CON2bits.SYNCSEL = 0b11111;
    OC6CON2bits.SYNCSEL = 0b11111;
    OC7CON2bits.SYNCSEL = 0b11111;
    OC8CON2bits.SYNCSEL = 0b11111;
    OC9CON2bits.SYNCSEL = 0b11111;
}
