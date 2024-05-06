#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "function.h"

#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define frequency (200)   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


int Duty_Calculation(double Duty);
void motor_config();

// 1=forward, 2=back, 3=right, 4=left
void control(int sign){
    else if(sign==right){  // right
    // 右回転(正転)する
        RPOR14bits.RP28R = 22;   //  OC5, M5S1, M5S2, (RP28-RP6)  M1enc
        RPOR3bits.RP7R = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)  M2enc
        U2TXREG=RIGHT;  // もう片方のマイコン側で関数が呼び出される
        OC5R = (Duty_Calculation(99));
        OC6R = (Duty_Calculation(99));
        __delay_ms(50)
    }
    else if(sign==left){  // left
    // 左回転(逆転)する
        RPOR3bits.RP6R= 22;   //  OC5, M5S1, M5S2, (RP28-RP6)  M1enc
        RPOR6bits.RP13R = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)  M2enc
        U2TXREG=LEFT;  // もう片方のマイコン側で関数が呼び出される
        OC5R = (Duty_Calculation(60));
        OC6R = (Duty_Calculation(60));
        __delay_ms(50)
    }
    else if (sign==select){  // select
        int motor=5  // M5はM1の角度調整モーター
        switch (motor){
        case 5:
            RPOR14bits.RP28R= 22;   //  OC5, M5S1 (RP28)  M1enc
            RPOR3bits.RP6R= 23;  //  OC6,  M5S2, (RP6)  M1enc
            break;
        case 6:
            RPOR3bits.RP7R= 22;  //  OC5, M6S1, (RP7)  M2enc
            RPOR6bits.RP13R= 23;  //  OC6, M6S2,  (RP13)  M2enc
            break;
        case 7:
            U2TXREG=SELECT_M7;  // もう片方のマイコン側で関数が呼び出される
            break;
        case 8:
            U2TXREG=SELECT_M8;  // もう片方のマイコン側で関数が呼び出される
            break;
        default:
            motor=4;
            break;
        motor++;
        }
    }
    else if (sign==select_right){  // △
    // 一つのモーターを右回転(正転)
        OC5R = (Duty_Calculation(60));
        U2TXREG=SELECT_RIGHT;  // もう片方のマイコン側で関数が呼び出される
        __delay_ms(50)
    }
    else if (sign==select_left){  // □
    // 一つのモーターを左回転(逆転)
        OC6R = (Duty_Calculation(60));
        U2TXREG=SELECT_LEFT;  // もう片方のマイコン側で関数が呼び出される
        __delay_ms(50)
    }
    else{
        // error
    }
}





int Duty_Calculation(double Duty){
    int Voltage, OC_config;
    Voltage =frequency;
    OC_config = Voltage * (Duty*0.01);  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
    return OC_config;
}

void motor_config(){
//    RPORxbits.RPxR = 18;   //  OC1, M7S1, M7S2, (RP2-RP16)
//    RPORxbits.RPxR = 19;   //  OC2, M8S1, M8S2, (RP10-RP17)

//    RPORxbits.RPxR = 20;   //  OC3, M9S1, M9S2, (RP14-RP29)
//    RPORxbits.RPxR = 21;   //  OC4, M10S1, M10S2, (RP27-RP19)


    OC1CON1bits.OCTSEL = 0b111;  // カウントアップの設定、利用するクロックの設定
    OC2CON1bits.OCTSEL = 0b111;
    OC3CON1bits.OCTSEL = 0b111;
    OC4CON1bits.OCTSEL = 0b111;
    OC5CON1bits.OCTSEL = 0b111;
    OC6CON1bits.OCTSEL = 0b111;

    OC1CON1bits.OCM=0b110;   // 出力コンペア モード選択ビット
    OC2CON1bits.OCM=0b110;
    OC3CON1bits.OCM=0b110;
    OC4CON1bits.OCM=0b110;
    OC5CON1bits.OCM=0b110;
    OC6CON1bits.OCM=0b110;

    OC1CON2bits.SYNCSEL = 0b11111;  // リセットする要因の設定、トリガ要素、周期を決定する
    OC2CON2bits.SYNCSEL = 0b11111;
    OC3CON2bits.SYNCSEL = 0b11111;
    OC4CON2bits.SYNCSEL = 0b11111;
    OC5CON2bits.SYNCSEL = 0b11111;
    OC6CON2bits.SYNCSEL = 0b11111;
}
