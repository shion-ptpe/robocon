#ifndef USER_H
#define	USER_H


// main.c
typedef unsigned char data_t;
#define M_PI        3.14159265358979323846264338327950288   //pi


// motor.c
#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define frequency (200)   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


// signal.c
struct field{
    data_t b1 : 1;
    data_t b2 : 1;
    data_t b3 : 1;
    data_t b4 : 1;
    data_t b5 : 1;
    data_t b6 : 1;
    data_t b7 : 1;
    data_t b8 : 1;
    data_t b9 : 1;
    data_t b10 : 1;
    data_t b11 : 1;
    data_t b12 : 1;
    data_t b13 : 1;
    data_t b14 : 1;
    data_t b15 : 1;
    data_t b16 : 1;
};

union bit_look{
    unsigned short int data;
    struct field bits;
};


// uart_config.c
#define QUEUE_SIZE 16
#define SUCCESS 1
#define FAILURE 0

#endif	/* USER_H */
