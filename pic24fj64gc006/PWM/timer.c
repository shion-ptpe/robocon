#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>
#include <stdio.h>
#include <string.h>


void config();
void timer_config();

int main(){
    config();
    timer_config();
    while(1){
        //
    }
    return 0;
}


void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    static unsigned int counter = 0;
    if( ++counter > 1000 ){	//2.5 sec
        counter = 0;
        LATDbits.LATD3=0;  // red;
    }else if( counter > 500 ){//2.0 sec
        LATDbits.LATD3=1;  // red;
    }

    IFS0bits.T1IF = 0;
}

void timer_config(){
    T1CON = 0b0000000000010000; //Fcy=Fosc/2(=16MHz), Prescaler(1/8) -> 2MHz
    PR1 = 400;  // 400/4MHz = 0.1ms
    TMR1 = 0;

    IPC0bits.T1IP = 7; //INterrupt priority = 7
    IEC0bits.T1IE = 1; //Enable Interrupt

    T1CONbits.TON = 1;  //start
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
