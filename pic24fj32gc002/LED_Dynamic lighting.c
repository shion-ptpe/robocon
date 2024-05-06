// Dynamic lighting

#include <xc.h>
#include "config.h"
#define FCY 4000000
#include <libpic30.h>

#define A LATBbits.LATB4  //11.A
#define B LATBbits.LATB5  //7.B
#define C LATBbits.LATB2  //4.C
#define D LATBbits.LATB0  //2.D
#define E LATAbits.LATA0  //1.E
#define F LATBbits.LATB7  //10.F
#define G LATBbits.LATB9  //5.G

void TRIS();
void conversion();
void minus(int num);
void figure(int num);


int main() {
    int num=-888;
    
    OSCCON = 0x0000;
    CLKDIV = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISA = 0xffff;
    TRISB = 0xffff;
    
    TRIS();
    while(num<0){
        num = num*(-1);
        minus(num);
        conversion(num);
        num = num*(-1);
    }
    while(num>0){
        conversion(num);
    }
    return 0;

}
    



void TRIS(){
    TRISBbits.TRISB7 = 0;
    TRISAbits.TRISA0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB10 = 0; //12・4桁
    TRISBbits.TRISB11 = 0; //9・3桁
    TRISBbits.TRISB13 = 0; //8・2桁
    TRISBbits.TRISB15 = 0; //6・1桁
}

void figure(int count){
    switch(count){
        LATA = 0x0;
        LATB = 0x0;
        case 0:
            A = 1;  //7・A
            B = 1;  //6・B
            C = 1;  //4・C
            D = 1;  //2・D
            E = 1;  //1・E
            F = 1;  //9・F
            break;
        case 1:
            B = 1;  //6・B
            C = 1;  //4・C
            break;
        case 2:
            A = 1;  //7・A
            B = 1;  //6・B
            D = 1;  //2・D
            E = 1;  //1・E
            G = 1;  //10・G
            break;
        case 3:
            A = 1;  //7・A
            B = 1;  //6・B
            C = 1;  //4・C
            D = 1;  //2・D
            G = 1;  //10・G
            break;
        case 4:
            B = 1;  //6・B
            C = 1;  //4・C
            F = 1;  //9・F
            G = 1;  //10・G
            break;
        case 5:
            A = 1;  //7・A
            C = 1;  //4・C
            D = 1;  //2・D
            F = 1;  //9・F
            G = 1;  //10・G
            break;
        case 6:
            A = 1;  //7・A
            C = 1;  //4・C
            D = 1;  //2・D
            E = 1;  //1・E
            F = 1;  //9・F
            G = 1;  //10・G
            break;
        case 7:
            A = 1;  //7・A
            B = 1;  //6・B
            C = 1;  //4・C
            F = 1;  //9・F
            break;
        case 8:
            A = 1;  //7・A
            B = 1;  //6・B
            C = 1;  //4・C
            D = 1;  //2・D
            E = 1;  //1・E
            F = 1;  //9・F
            G = 1;  //10・G
            break;
        default:
            A = 1;  //7・A
            B = 1;  //6・B
            C = 1;  //4・C
            D = 1;  //2・D
            F = 1;  //9・F
            G = 1;  //10・G
            break;
        }
    
}   

void minus(int num){
    if(num<10){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB13 = 1;
        LATBbits.LATB15 = 0;
        G = 1;
        __delay_ms(5)
    }
    else if(num<100){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 1;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 0;
        G = 1;
        __delay_ms(5)
    }
    
    else if(num<1000){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB10 = 1;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 0;
        G = 1;
        __delay_ms(5)
    }
    
    else{
        LATA = 0x0;
        LATB = 0x0;
        __delay_ms(5)
    }
}

void conversion(int num){
    if(num<10){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 1;
        figure(num);
        __delay_ms(5)
    }
    
    else if(num<100){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 1;
        LATBbits.LATB15 = 0;
        figure(num/10);
        __delay_ms(5)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 0;    
        LATBbits.LATB15 = 1;
        figure(num%10);
        __delay_ms(5)
    }
    
    else if(num<1000){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 1;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 0;
        figure(num/100);
        __delay_ms(6)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 1;
        LATBbits.LATB15 = 0;
        figure((num%100)/10);
        __delay_ms(7)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 0;    
        LATBbits.LATB15 = 1;
        figure(num%10);
        __delay_ms(7)
    }
    
    else if(num<10000){
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB10 = 1;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 0;
        figure(num/1000);
        __delay_ms(5)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB10 = 0;
        LATBbits.LATB11 = 1;
        LATBbits.LATB13 = 0;
        LATBbits.LATB15 = 0;
        figure((num%1000)/100);
        __delay_ms(5)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB10 = 0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 1;
        LATBbits.LATB15 = 0;
        figure((num%100)/10);
        __delay_ms(5)
        LATA = 0x0;
        LATB = 0x0;
        LATBbits.LATB10 = 0;
        LATBbits.LATB11 = 0;
        LATBbits.LATB13 = 0;    
        LATBbits.LATB15 = 1;
        figure(num%10);
        __delay_ms(5)
    }

    else{
        LATBbits.LATB10 = 1;
        LATBbits.LATB11 = 1;
        LATBbits.LATB13 = 1;    
        LATBbits.LATB15 = 1;
        G = 1;
        __delay_ms(1000)
        G = 0;
        __delay_ms(1000)
        }
}