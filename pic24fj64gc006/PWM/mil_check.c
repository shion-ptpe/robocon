#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>

#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define frequency (200)   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


void config();
void motor_config();
int Duty_Calculation(double Duty);

int main(){
   config();
   motor_config();
   while(1){
       RPOR1bits.RP2R = 18;   //  OC1, M1S1, M1S2, (RP2-RP16)
       RPOR5bits.RP10R = 19;   //  OC2, M2S1, M2S2, (RP10-RP17)
       RPOR7bits.RP14R = 20;   //  OC3, M3S1, M3S2, (RP14-RP29)
       RPOR13bits.RP27R = 21;   //  OC4, M4S1, M4S2, (RP27-RP19)
       RPOR14bits.RP28R = 22;   //  OC5, M5S1, M5S2, (RP28-RP6)
       RPOR3bits.RP7R = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)

       OC1R = (Duty_Calculation(99));
       OC2R = (Duty_Calculation(99));
       OC3R = (Duty_Calculation(99));
       OC4R = (Duty_Calculation(99));
       OC5R = (Duty_Calculation(99));
       OC6R = (Duty_Calculation(99));
       __delay_ms(1000);

       RPOR8bits.RP16R = 18;   //  OC1, M1S1, M1S2, (RP2-RP16)
       RPOR8bits.RP17R = 19;   //  OC2, M2S1, M2S2, (RP10-RP17)
       RPOR14bits.RP29R = 20;   //  OC3, M3S1, M3S2, (RP14-RP29)
       RPOR9bits.RP19R = 21;   //  OC4, M4S1, M4S2, (RP27-RP19)
       RPOR3bits.RP6R = 22;   //  OC5, M5S1, M5S2, (RP28-RP6)
       RPOR6bits.RP13R = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)
       OC1R = (Duty_Calculation(99));
       OC2R = (Duty_Calculation(99));
       OC3R = (Duty_Calculation(99));
       OC4R = (Duty_Calculation(99));
       OC5R = (Duty_Calculation(99));
       OC6R = (Duty_Calculation(99));
       __delay_ms(1000);
   }
   return 0;
}
int Duty_Calculation(double Duty){
   int Voltage, OC_config;
   Voltage =frequency;
   OC_config = Voltage * (Duty*0.01);  // TMR(タイマ)が幾つで切り替わるのか(Voltage_switch_H)にDuty比を掛けて、電圧をlowに下げるタイミングを計算する
   return OC_config;
}

void config(void){
   OSCCON = 0x0000;
   CLKDIV = 0x0000;
   TRISB = 0xffff;
   TRISD = 0xffff;
   TRISE = 0xffff;
   U1MODE = 0x0000;
   U1STA = 0x0000;

   OC1CON1 = 0x0000;
   OC1CON2 = 0x0000;

   ANSB = 0;
   ANSD = 0;
   ANSE = 0;

   TRISBbits.TRISB3 = 0;  // LED
   TRISDbits.TRISD3 = 0;  // LED
   TRISDbits.TRISD7 = 0;  // LED

//    LATBbits.LATB3=1;  // green
//    LATDbits.LATD3=1;  // red, (RP22)
//    LATDbits.LATD7=1;  // yellow
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
//    RPOR11bits.RP22R = 18;  // red, (RD3)

//    RPORxbits.RPxR = 18;   //  OC1, M1S1, M1S2, (RP2-RP16)
//    RPORxbits.RPxR = 19;   //  OC2, M2S1, M2S2, (RP10-RP17)
//    RPORxbits.RPxR = 20;   //  OC3, M3S1, M3S2, (RP14-RP29)
//    RPORxbits.RPxR = 21;   //  OC4, M4S1, M4S2, (RP27-RP19)
//
//    RPORxbits.RPxR = 22;   //  OC5, M5S1, M5S2, (RP28-RP6)  M1enc
//    RPORxbits.RPxR = 23;   //  OC6, M6S1, M6S2,  (RP7-RP13)  M2enc

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
