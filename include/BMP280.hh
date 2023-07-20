#include "mbed.h"

//*******************************************************************************

#define      BMP280_ADDRESS                     (0x77)

#define      BMP280_REGISTER_DIG_T1             (0x88)
#define      BMP280_REGISTER_DIG_T2             (0x8A)
#define      BMP280_REGISTER_DIG_T3             (0x8C)

#define      BMP280_REGISTER_DIG_P1             (0x8E)
#define      BMP280_REGISTER_DIG_P2             (0x90)
#define      BMP280_REGISTER_DIG_P3             (0x92)
#define      BMP280_REGISTER_DIG_P4             (0x94)
#define      BMP280_REGISTER_DIG_P5             (0x96)
#define      BMP280_REGISTER_DIG_P6             (0x98)
#define      BMP280_REGISTER_DIG_P7             (0x9A)
#define      BMP280_REGISTER_DIG_P8             (0x9C)
#define      BMP280_REGISTER_DIG_P9             (0x9E)

#define      BMP280_REGISTER_CHIPID             (0xD0)
#define      BMP280_REGISTER_VERSION            (0xD1)
#define      BMP280_REGISTER_SOFTRESET          (0xE0)

#define      BMP280_REGISTER_CAL26              (0xE1)  // R calibration stored in 0xE1-0xF0

#define      BMP280_REGISTER_CONTROL            (0xF4)
#define      BMP280_REGISTER_CONFIG             (0xF5)
#define      BMP280_REGISTER_PRESSUREDATA       (0xF7)
#define      BMP280_REGISTER_TEMPDATA           (0xFA)

#define      POWER_ON_RESET                     (0xB6) // Used in the power-on-reset procedure

#define      CHIP_ID                            (0x58)

#define      STD_FREQUENCY                      (100000)
#define      FAST_FREQUENCY                     (400000)
#define      HIGH_SPEED_FREQUENCY               (3400000)

#define      CALIBRATION_PARAM_NUM              (24)

//*******************************************************************************


class BMP280: public I2C
{
public:
  BMP280(PinName sda, PinName scl, long frequency_i2c=STD_FREQUENCY);
  ~BMP280();
  bool init();
  bool checkControlRegister();
  bool checkConfigRegister();
  bool getRawTemperature(int32_t *temp);
  bool getRawPressure(int32_t *press);
  bool getTemperature(float *tempC);
  float getAltitude(float seaLevelhPa=1013.25);
  bool getPressure(float *pressure);
  bool getPressure_d(double *pressure);
  bool getTemperature_d(double *temp);
  bool startMeasure();

  typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
  } Calibration;

  Calibration calibration;  

private:
  int32_t t_fine;
  bool reset();
  bool checkID();
  bool getTemperature_d(double &tempC);
  bool readCalibrationData(char &data, char address);

};
