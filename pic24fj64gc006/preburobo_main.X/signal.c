#include <xc.h>
#include "user.h"
#include "function.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

union bit_look bi;

void button_analysis(){
    if(bi.bits.b1){ // 0000_0001
        U2TXREG = 'A';  // ↑
    }
    if(bi.bits.b2){  // 0000_0010
        U2TXREG = 'B';  // ↓
    }
    if(bi.bits.b3){  // 0000_0100
        U2TXREG = 'C';  // →
    }
    if(bi.bits.b4){  // 0000_1000
        U2TXREG = 'D';  // ←
    }
    if(bi.bits.b5){  // 0001_0000
        U2TXREG = 'E';  // △
    }
    if(bi.bits.b6){  // 0010_0000
        U2TXREG = 'F';  // ×
    }
    if(bi.bits.b7){  // 0100_0000
        U2TXREG = 'G';  // ○
    }
    if(bi.bits.b9){  // 0000_0001
        U2TXREG = 'H';  // □
    }
    if(bi.bits.b10){  // 0000_0010
        U2TXREG = 'I';  // L1
    }
    if(bi.bits.b11){  // 0000_0100
        U2TXREG = 'J';  // L2
    }
    if(bi.bits.b12){  // 0000_1000
        U2TXREG = 'K';  // R1
    }
    if(bi.bits.b13){  // 0001_0000
        U2TXREG = 'L';  // R2
    }
    bi.data=0;
}



void stick_analysis(data_t datax, data_t datay){
    int x=datax, y=datay;
    float theta;
    char data[256];
    sprintf(data,"x:%x\ny:%x\n", x,y);
    read(data);

    x -= 0x40;
    y -= 0x40;

    theta = atan2(y, x);
//    sprintf(data,"x:%x\ny:%x\ntheta:%f", x, y, theta);
//    read(data);

//    motor_control_roricon((double)theta);
}


