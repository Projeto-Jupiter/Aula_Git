#include "mbed.h"

DigitalOut main_signal(PC_1);    //Sinal main
DigitalIn main_detect(PC_2);     //Detecção do main

DigitalOut drogue_signal(PA_3);  //Sinal drogue
DigitalIn drogue_detect(PA_4);   //Detecção drogue

DigitalOut buzzer(PB_7);

int main(){   
    buzzer = 0;
    drogue_signal = 1;
    main_signal = 1;
    // ThisThread::sleep_for(2000ms);
    wait_us(2000000);
    buzzer = 1;
    // ThisThread::sleep_for(2000ms);
    wait_us(2000000);
    buzzer = 0;
    while(1){
        if (drogue_detect == 0){
            buzzer = 1;
            wait_us(100000);
            //ThisThread::sleep_for(100ms);

        } else {
            buzzer = 0;

        }
        if (main_detect == 0){
            buzzer = 1;
            wait_us(100000);
            //ThisThread::sleep_for(100ms);

        }
    }

    return 0;
}