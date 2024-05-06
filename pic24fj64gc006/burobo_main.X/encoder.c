#include <xc.h>
#include "user.h"
#include "function.h"
#include <libpic30.h>

double angle[]={0,0,0,0,0};

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    CN_process();
    IFS1bits.CNIF=0;
}

void CN_process(){
    angle[0]+=PORTDbits.RD5;  // 反時計回り
    angle[0]-=PORTDbits.RD6;  // 時計回り
//    angle[1]+=PORTFbits.RF1;
//    angle[1]-=PORTFbits.RF0;
//    angle[2]+=PORTEbits.RE2;
//    angle[2]-=PORTEbits.RE1;
//    angle[3]+=PORTEbits.RE3;
//    angle[3]-=PORTEbits.RE4;
//    angle[4]+=PORTEbits.RE6;
//    angle[4]-=PORTEbits.RE5;
}

void rotary_encoder_config(void){
    CNEN1bits.CN14IE=1;// RD5__CN14__CN1-1
    CNEN1bits.CN15IE=1; // RD6__CN15__CN1-2
//    CNEN5bits.CN69IE=1; // RF1__CN69__CN2-1
//    CNEN5bits.CN68IE=1; // RF0__CN68__CN2-2
//    CNEN4bits.CN60IE=1; // RE2__CN60__CN3-1
//    CNEN4bits.CN59IE=1; // RE1__CN59__CN3-2
//    CNEN4bits.CN61IE=1; // RE3__CN61__CN4-1
//    CNEN4bits.CN62IE=1; // RE4__CN62__CN4-2
//    CNEN5bits.CN64IE=1; // RE6__CN64__CN5-1  // 上
//    CNEN4bits.CN63IE=1; // RE5__CN63__CN5-2  // 上

    IEC1bits.CNIE=1;
    IFS1bits.CNIF=0;
    IPC4bits.CNIP = 5;
}

