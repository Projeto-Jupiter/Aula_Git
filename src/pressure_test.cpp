#include "mbed.h"
#include "BMP280.hh"

BufferedSerial pc(PB_10, PC_5, 115200);

FileHandle *mbed::mbed_override_console(int fd){
    return &pc;
};

int main(){
    BMP280 sensor(PB_9, PB_8, FAST_FREQUENCY);
    float pressure;
    printf("Teste iniciado\n");
    while (!sensor.init());
    printf("Sensor inicializado\n");
    while (true){
        while(!sensor.startMeasure());
        wait_us(7000);
        sensor.getPressure(&pressure);
        printf("%f\n", pressure);


    }
    return 0;
}

// #include "mbed.h"
// #include "BMP280.hh"


// BufferedSerial pc(PB_10, PC_5, 115200);

// FileHandle *mbed::mbed_override_console(int fd) {
//     return &pc;
// };

// int main(){

//         BMP280 sensor(PB_9, PB_8, FAST_FREQUENCY);
//         float pressure;
//         printf("Teste iniciado\n");
//         while (!sensor.init());
//         printf("Sensor inicializado\n");
//         while(!sensor.startMeasure()){;
//             wait_us(6400);
//             sensor.getPressure(&pressure);
//             printf("%f\n", pressure);
//         };
//         while (true){
//             while(!sensor.startMeasure());
//             wait_us(6400);
//             sensor.getPressure(&pressure);
//             printf("\r%f\n", pressure);

//         };
        
//     return 0;
// }