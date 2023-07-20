// #include "mbed.h"

// DigitalOut main_signal(PC_1);    //Sinal main
// DigitalIn main_detect(PC_2);     //Detecção do main
// DigitalOut main_chute (PC_0);    //MAIN

// DigitalOut drogue_signal(PA_3);  //Sinal drogue
// DigitalIn drogue_detect(PA_4);   //Detecção drogue
// DigitalOut drogue_chute(PC_3);   //DROGUE

// DigitalOut buzzer(PB_7);   
// int i = 0;      

// int main(){
//     main_signal = 1;
//     drogue_signal = 1;
//     main_chute = 0;
//     drogue_chute = 0;
//     //ThisThread::sleep_for(1000ms);
//     wait_us(10000000);                 //Aguarda 10seg
//     buzzer = 1;                        //Apito de inicialização (2seg)
//     //ThisThread::sleep_for(2000ms);
//     wait_us(2000000);
//     buzzer = 0;

//     while(1){
//         buzzer = 0;
//         wait_us(10000000);
//         buzzer = 1;
//         wait_us(10000000);
//         }
//         return 0;
//     }

////////////////////////////////////////////
// #include <mbed.h>

// int main () {
//     DigitalOut SIGNAL_DROGUE(PC_1);
//     DigitalIn DETECT_DROGUE(PA_4);
//     DigitalOut SIGNAL_MAIN(PA_3);
//     DigitalIn DETECT_MAIN(PC_2);
//     DigitalOut BUZZER(PB_7);
//     DigitalOut DROGUE(PC_3);
//     DigitalOut MAIN(PC_0);

//     DROGUE = 0;
//     MAIN = 0;
    
//     SIGNAL_DROGUE = 1;
//     SIGNAL_MAIN = 1;
//     BUZZER = 0;
//     wait_us(2000000);
//     //ThisThread::sleep_for(2000ms);
//     // BUZZER = 0;

//     if (DETECT_DROGUE) {            // DETEC_DROGuE == 1, ou seja, não detectou
//         BUZZER = 1;
//         wait_us(500000);
//         //ThisThread::sleep_for(500ms);
//         BUZZER = 0;


//     } else {                        // DETEC_DROGuE == 0, ou seja, detectou
//         BUZZER = 1;
//         wait_us(3000000);
//         //ThisThread::sleep_for(3000ms);
//         BUZZER = 0;

//         if (DETECT_MAIN) {            // DETEC_MAIN == 1, ou seja, não detectou
//         BUZZER = 1;
//         wait_us(500000);
//         //ThisThread::sleep_for(500ms);
//         BUZZER = 0;
//         } else {                        // DETEC_MAIN == 0, ou seja, detectou
//             BUZZER = 1;
//             wait_us(3000000);
//             //ThisThread::sleep_for(3000ms);
//             BUZZER = 0;
//             wait_us(20000000);
//             //ThisThread::sleep_for(20000ms);
//             DROGUE = 1;
//             wait_us(10000000);
//             //ThisThread::sleep_for(10000ms);
//             MAIN = 1;
//     };
//         };

        
//     //wait_us(5000000);
    




// };
