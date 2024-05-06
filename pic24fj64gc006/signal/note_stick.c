struct field{
    data_t b0 : 1;
    data_t b1 : 1;
    data_t b2 : 1;
    data_t b3 : 1;
    data_t b4 : 1;
    data_t b5 : 1;
    data_t b6 : 1;
    data_t b7 : 1;
};

union{
    data_t data;
    struct field bits;
}bi;


void analysis(data_t *queue_data){
    bi.data = ((queue_data[1]<<8) | queue_data[2]);

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
}



void analysis(data_t *queue_data){
    int data, num;
    data = ((queue_data[1]<<8) | queue_data[2]);
    for(num=0;num<12;num++){
        if((data>>num) & 0xFE){
            switch(2**num){
                case 0x0001: // 0000_0001
                    motor_control(forward);  // ↑
                    break;
                case 0x0002:  // 0000_0010
                    motor_control(back);  // ↓
                    break;
                case 0x0004:  // 0000_0100
                    motor_control(right);  // →
                    break;
                case 0x0008:  // 0000_1000
                    motor_control(left);  // →
                    break;
                case 0x0010:  // 0001_0000
                    motor_control(triangle);  // △
                    break;
                case 0x0020:  // 0010_0000
                    motor_control(back);  // ×
                    break;
                case 0x0040:  // 0100_0000
                    motor_control(circle);  // ○
                    break;
                case 0x0100:  // 0000_0001_0000_0000
                    motor_control(square);  // □
                    break;
                case 0x0200:  // 0000_0010_0000_0000
                    motor_control(L1);  // L1
                    break;
                case 0x0400:  // 0000_0100_0000_0000
                    motor_control(L2);  // L2
                    break;
                case 0x0800:  // 0000_1000_0000_0000
                    motor_control(R1);  // R1
                    break;
                case 0x1000:  // 0001_0000_0000_0000
                    motor_control(R2);  // R2
                    break;
            }
        }
    }
}
