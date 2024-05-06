#ifndef USER_H
#define	USER_H


// main.c
typedef unsigned char data_t;


// motor.c
#define MAX_Voltage (3.3)   //最大の電圧(マイコンの電圧)
#define frequency (200)   // TMRの周波数(OCxRSの値)ただし、4MHz以下(決める)


// uart_config.c
#define QUEUE_SIZE 1024
#define SUCCESS 1
#define FAILURE 0

#endif	/* USER_H */
