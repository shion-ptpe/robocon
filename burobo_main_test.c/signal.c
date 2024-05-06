#include <xc.h>
#include "user.h"
#include "function.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

union bit_look bi;
extern double angle[];


// ○, ×はオムニの正転・逆転。止めるのはL2, R2ボタン、手を離しただけじゃ止まらない。
// R1, L1はロリコンの正転・逆転を切り替える。止めるのはL2, R2ボタン、手を離しただけじゃ止まらない。

void button_analysis(){

    if(bi.bits.b1){ // 0000_0001

        OC1R = (Duty_Calculation(50));
        OC5R = 0;
//        U2TXREG = 'A';  // ↑
    }
    if(bi.bits.b2){  // 0000_0010
        OC2R = (Duty_Calculation(50));
        OC6R = 0;
//        U2TXREG = 'B';  // ↓
    }
    if(bi.bits.b3){  // 0000_0100
        OC3R = (Duty_Calculation(50));
        OC7R = 0;
//        U2TXREG = 'C';  // →
    }
    if(bi.bits.b4){  // 0000_1000
        OC4R = (Duty_Calculation(50));
        OC8R = 0;
//        U2TXREG = 'D';  // ←
    }
    if(bi.bits.b5){  // 0001_0000
        OC1R = 0;
        OC5R = (Duty_Calculation(50));

//        OC5R = (angle[0] == 0) ? (Duty_Calculation(50)) : 0;
//        OC6R = (angle[1] == 0) ? (Duty_Calculation(50)) : 0;
//        OC7R = (angle[2] == 0) ? (Duty_Calculation(50)) : 0;
//        OC8R = (angle[3] == 0) ? (Duty_Calculation(50)) : 0;
//        U2TXREG = 'E';  // △
    }
    if(bi.bits.b6){  // 0010_0000
        LATBbits.LATB3=1;  //green

        OC2R = 0;
        OC6R = (Duty_Calculation(50));
//        U2TXREG = 'F';  // ×　逆転
    }
    if(bi.bits.b7){  // 0100_0000
        OC3R = 0;
        OC7R = (Duty_Calculation(50));
//        U2TXREG = 'G';  // ○  正転
    }
    if(bi.bits.b9){  // 0000_0001
        OC4R = 0;
        OC8R = (Duty_Calculation(50));
//        OC5R = (angle[0] == 0) ? (Duty_Calculation(50)) : 0;
//        OC6R = (angle[1] == 0) ? (Duty_Calculation(50)) : 0;
//        OC7R = (angle[2] == 0) ? (Duty_Calculation(50)) : 0;
//        OC8R = (angle[3] == 0) ? (Duty_Calculation(50)) : 0;
//        U2TXREG = 'H';  // □
    }
    if(bi.bits.b10){  // 0000_0010
        OC5R = 0;
        OC6R = 0;
        OC7R = 0;
        OC8R = 0;
        OC1R = Duty_Calculation(50);
        OC2R = Duty_Calculation(50);
        OC3R = Duty_Calculation(50);
        OC4R = Duty_Calculation(50);
//        U2TXREG = 'I';  // L1
    }
    if(bi.bits.b11){  // 0000_0100
        motor_stop();
//        U2TXREG = 'J';  // L2
        LATDbits.LATD7=0;

    }
    if(bi.bits.b12){  // 0000_1000
        OC1R = 0;
        OC2R = 0;
        OC3R = 0;
        OC4R = 0;
        OC5R = Duty_Calculation(50);
        OC6R = Duty_Calculation(50);
        OC7R = Duty_Calculation(50);
        OC8R = Duty_Calculation(50);
//        U2TXREG = 'K';  // R1
    }
    if(bi.bits.b13){  // 0001_0000
        motor_stop();
//        U2TXREG = 'L';  // R2  停止
    }
    bi.data=0;
}



void stick_analysis(data_t datax, data_t datay){
    int x=datax, y=datay;
    double theta;
//    char data[256];


    x -= 0x40;
    y -= 0x40;

    theta = atan2(y, x);  // 出力される値と正負が逆、上下反転してる=>(-1掛ける)「theta = atan2(y, x) * -1; 」。が、歯車でロリコンとタイヤが繋がってるから、上下反転の座標でいい？
//    sprintf(data,"x:%x\ny:%x\ntheta:%f\nabs:%f\n", x, y, theta, sqrt(pow(x, 2.0) + pow(y, 2.0)));
//    read(data);

    if(35 < sqrt(pow(x, 2.0) + pow(y, 2.0))){  // 原点からの距離、誤作動防ぐ
        motor_control_roricon((double)theta);
        LATDbits.LATD7=1;  //yellow
//        LATBbits.LATB3=~LATBbits.LATB3;  //green
    }

}


