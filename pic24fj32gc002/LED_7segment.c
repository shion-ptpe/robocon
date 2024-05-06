// 7segment

#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

int main() {
int count=-1;

OSCCON = 0x0000;
CLKDIV = 0x0000;
AD1PCFG = 0xFFFF;
TRISA = 0xffff;
TRISB = 0xffff;

TRISBbits.TRISB7 = 0;
TRISAbits.TRISA0 = 0;
TRISBbits.TRISB0 = 0;
TRISBbits.TRISB2 = 0;
TRISBbits.TRISB4 = 0;
TRISBbits.TRISB5 = 0;
TRISBbits.TRISB9 = 0;

while(1){
    __delay_ms(1000)
    count=count +1;
    LATBbits.LATB4 = 0;  //7・A
    LATBbits.LATB5 = 0;  //6・B
    LATBbits.LATB2 = 0;  //4・C
    LATBbits.LATB0 = 0;  //2・D
    LATAbits.LATA0 = 0;  //1・E
    LATBbits.LATB7 = 0;  //9・F
    LATBbits.LATB9 = 0;  //10・G
    switch(count){
        case 0:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATAbits.LATA0 = 1;  //1・E
            LATBbits.LATB7 = 1;  //9・F
            break;
        case 1:
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            break;
        case 2:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB0 = 1;  //2・D
            LATAbits.LATA0 = 1;  //1・E
            LATBbits.LATB9 = 1;  //10・G
            break;
        case 3:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATBbits.LATB9 = 1;  //10・G
            break;
        case 4:
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB7 = 1;  //9・F
            LATBbits.LATB9 = 1;  //10・G
            break;
        case 5:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATBbits.LATB7 = 1;  //9・F
            LATBbits.LATB9 = 1;  //10・G
            break;
        case 6:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATAbits.LATA0 = 1;  //1・E
            LATBbits.LATB7 = 1;  //9・F
            LATBbits.LATB9 = 1;  //10・G
            break;
        case 7:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB7 = 1;  //9・F
            break;
        case 8:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATAbits.LATA0 = 1;  //1・E
            LATBbits.LATB7 = 1;  //9・F
            LATBbits.LATB9 = 1;  //10・G
            break;
        default:
            LATBbits.LATB4 = 1;  //7・A
            LATBbits.LATB5 = 1;  //6・B
            LATBbits.LATB2 = 1;  //4・C
            LATBbits.LATB0 = 1;  //2・D
            LATBbits.LATB7 = 1;  //9・F
            LATBbits.LATB9 = 1;  //10・G
            count = -1;
            break;
        }

}
return 0;

}