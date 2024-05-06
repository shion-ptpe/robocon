#ifndef FUNCTION_H
#define	FUNCTION_H



// main.c
void config();

// motor.c
int Duty_Calculation(double Duty);
void motor_config();
void motor_control_move(int sign);
void motor_stop();


//uart_config.c
void UART1_config();  // コントローラー用のUART
void UART2_config();  // マイコン間のUART
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);
int enqueue(data_t enq_data);
int dequeue(data_t *deq_data);
void read(char * msg);



#endif	/* FUNCTION_H */
