#include "BMP280.hh"

BMP280::BMP280(PinName sda, PinName scl, long frequency_i2c): I2C(sda, scl){
  frequency(frequency_i2c); //Inherits frequency from I2C classi
}

BMP280::~BMP280(){}

bool BMP280::init()
{
  constexpr char addr = BMP280_REGISTER_DIG_T1;
  char data[CALIBRATION_PARAM_NUM], first;

  if(reset() == false)
    return false;

  wait_us(5000);

  if(!checkID())
    return false;

  for(int i = 0; i < CALIBRATION_PARAM_NUM; i++){
    if(readCalibrationData(data[i], addr+i) == false)
      return false;
  }

  //Manually reading the first register
  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
    return false;

  if(read(BMP280_ADDRESS << 1, &first, 1) != 0)
    return false;

  data[0] = first;

  calibration.dig_T1 = ((data[1] << 8) | data[0]);
  calibration.dig_T2 = ((data[3] << 8) | data[2]);
  calibration.dig_T3 = ((data[5] << 8) | data[4]);
  calibration.dig_P1 = ((data[7] << 8) | data[6]);
  calibration.dig_P2 = ((data[9] << 8) | data[8]);
  calibration.dig_P3 = ((data[11] << 8) | data[10]);
  calibration.dig_P4 = ((data[13] << 8) | data[12]);
  calibration.dig_P5 = ((data[15] << 8) | data[14]);
  calibration.dig_P6 = ((data[17] << 8) | data[16]);
  calibration.dig_P7 = ((data[19] << 8) | data[18]);
  calibration.dig_P8 = ((data[21] << 8) | data[20]);
  calibration.dig_P9 = ((data[23] << 8) | data[22]);

  data[0] = BMP280_REGISTER_CONTROL;
  data[1] = 0x25; //osrs_t (001) - oversampling x1
                  //osrs_p (001) - oversampling x1
                  //mode   (01)  - normal mode
  
  if(write(BMP280_ADDRESS << 1, data, 2) != 0)
    return false;

  wait_us(5000);

  if(checkControlRegister() == false)
    return false;
  
  // data[0] = BMP280_REGISTER_CONFIG;
  // data[1] = 0x00; // t_sb (000)  - standby time 0.5 ms
  //                 // filter coef (000)
  //                 // reserved bit
  //                 // spi3w_en (0) - 3 wire spi disabled
  // if(write(BMP280_ADDRESS, data, 2) != 0)
  //   return false;
  
  return true;
  
}

bool BMP280::reset()
{
  constexpr char config[] = {BMP280_REGISTER_SOFTRESET};
  char check;

  if(write(BMP280_ADDRESS << 1, config, 2) != 0)
    return false;
  
  if(write(BMP280_ADDRESS << 1, &config[0], 1) != 0)
    return false;
  
  if(read(BMP280_ADDRESS << 1, &check, 1) != 0)
    return false;
  
  if(check == 0x00)
    return true;
  
  else return false;
}

bool BMP280::checkID()
{
  constexpr char addr = BMP280_REGISTER_CHIPID;
  char data;


  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
  {
    return false;
  }

  if(read(BMP280_ADDRESS << 1, &data, 1) != 0)
  {
    return false;
  }
  
  if(data != CHIP_ID)
    return false;
  
  return true;
  
}

bool BMP280::checkControlRegister()
{
  constexpr char addr = BMP280_REGISTER_CONTROL;
  char control;

  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
    return false;
  
  if(read(BMP280_ADDRESS << 1, &control, 1) != 0)
    return false;
  
  if(control != 0x25)
    return false;
  
  return true;
}

bool BMP280::checkConfigRegister()
{
  constexpr char addr = BMP280_REGISTER_CONFIG;
  char config;

  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
    return false;
  
  if(read(BMP280_ADDRESS << 1, &config, 1) != 0)
    return false;
  
  if(config != 0x00)
    return false;
  
  return true;
  
}

bool BMP280::startMeasure(){
  constexpr char data[] = {BMP280_REGISTER_CONTROL, 0x25};
                  //osrs_t (001) - oversampling x1
                  //osrs_p (001) - oversampling x1
                  //mode   (01)  - normal mode
  
  if(write(BMP280_ADDRESS << 1, data, 2) != 0)
    return false;
  
  return true;
}

bool BMP280::readCalibrationData(char &data, char address)
{
  if(write(BMP280_ADDRESS << 1, &address, 1) != 0)
    return false;

  if(read(BMP280_ADDRESS << 1, &data, 1) != 0)
    return false;

  return true;
}

bool BMP280::getRawTemperature(int32_t *temp)
{
  constexpr char addr = BMP280_REGISTER_TEMPDATA;
  char rawTemperature[3];
  
  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
    return false;

  if(read(BMP280_ADDRESS << 1, rawTemperature, 3) != 0)
    return false;
  
  *temp = rawTemperature[0];
  *temp <<= 8;
  *temp |= rawTemperature[1];
  *temp <<= 8;
  *temp |= rawTemperature[2];
  *temp >>= 4;
  
  return true;
}

bool BMP280::getTemperature(float *tempC)
{
  int32_t x1, x2;
  int32_t rawTemp;
  
  if(getRawTemperature(&rawTemp) == false)
    return false;
  
  x1 = ((((rawTemp>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2))  >> 11;
  x2 = (((((rawTemp>>4)- ((int32_t)calibration.dig_T1)) * ((rawTemp >> 4) - ((int32_t)calibration.dig_T1))) >> 12 ) * ((int32_t)calibration.dig_T3)) >> 14;
  t_fine = x1 + x2;

  *tempC = ((t_fine*5) + 128) >> 8;
  *tempC /= 100;

  return true;
}

bool BMP280::getRawPressure(int32_t *press)
{
  constexpr char addr = BMP280_REGISTER_PRESSUREDATA;
  char rawPressure[3];
  
  if(write(BMP280_ADDRESS << 1, &addr, 1) != 0)
    return false;

  if(read(BMP280_ADDRESS << 1, rawPressure, 3) != 0)
    return false;

  *press = rawPressure[0];
  *press <<= 8;
  *press |= rawPressure[1];
  *press <<= 8;
  *press |= rawPressure[2];
  *press >>= 4;

  return true;
}

bool BMP280::getPressure(float *pressure){
  int64_t var1, var2, p;
  float temp;
  int32_t rawPress;

  // Must be done first to get the t_fine variable set up
  if(getTemperature(&temp) == false)
    return false;
  
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)calibration.dig_P6;
  var2 = var2 + ((var1*(int64_t)calibration.dig_P5)<<17);
  var2 = var2 + (((int64_t)calibration.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)calibration.dig_P3)>>8) +
  ((var1 * (int64_t)calibration.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.dig_P1)>>33;
  
  if (var1 == 0) {
    *pressure = 0;
    return false;  // avoid exception caused by division by zero
  }

  if(getRawPressure(&rawPress) == false)
    return false;
    
  p = 1048576 - rawPress;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)calibration.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)calibration.dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)calibration.dig_P7)<<4);
  
  *pressure = (float)p/256;

  return true;
}

float BMP280::getAltitude(float seaLevelhPa)
{
  float altitude;
  float pressure;
  getPressure(&pressure); // in Si units for Pascal
  pressure /= 100;

  altitude = 44330 * (1.0 - pow(pressure / seaLevelhPa, 0.1903));

  return altitude;
}

bool BMP280::getTemperature_d(double *temp)
{
  double var1, var2;
  int32_t rawTemp;

  if(getRawTemperature(&rawTemp) == false)
    return false;

  var1 = (((double)rawTemp)/16384.0 - ((double)calibration.dig_T1)/1024.0) * ((double)calibration.dig_T2);
  var2 = ((((double)rawTemp/131072.0 - ((double)calibration.dig_T1)/8192.0) * ((double)rawTemp)/131072.0 -
  ((double)calibration.dig_T1)/8192.0)) * ((double)calibration.dig_T3);
  t_fine = (int32_t)(var1 + var2);

  *temp = (var1 + var2)/5120.0;

  return true;
}

bool BMP280::getPressure_d(double *pressure){
  double var1, var2, p;
  double temp;
  int32_t rawPress;

  if(getTemperature_d(&temp) == false)
    return false;
  
  if(getRawPressure(&rawPress) == false)
    return false;

  var1 = ((double)t_fine/2.0) - 64000.0;
  var2 = var1 * var1 * ((double)calibration.dig_P6) / 32768.0;
  var2 = var2 + var1 * ((double)calibration.dig_P5) * 2.0;
  var2 = (var2/4.0)+(((double)calibration.dig_P4) * 65536.0);
  var1 = (((double)calibration.dig_P3) * var1 * var1 / 524288.0 + ((double)calibration.dig_P2) * var1) / 524288.0;
  var1 = (1.0 + var1 / 32768.0)*((double)calibration.dig_P1);
  
  if(var1 == 0.0){ // avoid exception caused by division by zero
    *pressure = 0; 
    return false;
  }

  p = 1048576.0 - (double)rawPress;
  p = (p - (var2 / 4096.0)) * 6250.0 / var1;
  var1 = ((double)calibration.dig_P9) * p * p / 2147483648.0;
  var2 = p * ((double)calibration.dig_P8) / 32768.0;
  *pressure = p + (var1 + var2 + ((double)calibration.dig_P7)) / 16.0;
}
