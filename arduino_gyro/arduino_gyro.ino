#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C адрес
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
int16_t accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
uint8_t err[3];

long currentTime, previousTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Wire.begin();                      // Инициализировать связь i2c
  Wire.beginTransmission(MPU);       // Начать связь с MPU6050 MPU = 0x68
  Wire.write(0x6B);                  // обращаемся к регистру 6B
  Wire.write(0x00);                  // сброс - поместите 0 в регистр 6B
  Wire.endTransmission(true);        //закончить передачу

  // Настройка чувствительности акселерометра - полный диапазон шкалы (по умолчанию +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //обращаемся к регистру ACCEL_CONFIG (1C hex)
  Wire.write(0x10);                  //Установить биты регистра как 000 10 000 (диапазон полной шкалы +/- 8g)
  Wire.endTransmission(true);        //закончить передачу

  // Настроить чувствительность гироскопа - полный диапазон шкалы (по умолчанию +/- 250 град / с)
  Wire.write(0x1B);                   // Обращаемся к регистру GYRO_CONFIG (1B hex)
  Wire.write(0x10);                   // Установить биты регистра как 000 10 000 (полная шкала 1000 град / с)
  Wire.endTransmission(true);
  //delay(1);
  // Вызов этой функции, покажет значения ошибок IMU для вашего модуля
  calculate_IMU_error(10000);
  //delay(1);
}

void loop() {
  // данные акселерометра
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);                 // Начинаем с регистра 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);   //Забрашиваем 6 байт информации значение каждой оси сохраняется в 2 регистрах
  accX = (Wire.read() << 8 | Wire.read()) / 32768 * 8; // получаем результат в м/с/с для режима работы 8g
  accY = (Wire.read() << 8 | Wire.read()) / 32768 * 8;
  accZ = (Wire.read() << 8 | Wire.read()) / 32768 * 8;

  // данные гироскопа
  previousTime = currentTime;       // Предыдущее время сохраняется до фактического чтения
  currentTime = millis();           // Текущее время Фактическое время чтения
  Wire.beginTransmission(MPU);
  Wire.write(0x43);                 // Адрес первого регистра данных гироскопа 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);   // Чтение всех 4 регистров, значение каждой оси сохраняется в 2 регистрах
  gyroX = (Wire.read() << 8 | Wire.read()) / 32768 * 1000; // Для диапазона 1000 градусов | град / с
  gyroY = (Wire.read() << 8 | Wire.read()) / 32768 * 1000;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 32768 * 1000;

  gyroX = gyroX + err[0];
  gyroY = gyroY + err[1];
  gyroZ = gyroZ + err[2];

  gyroAngleX = gyroAngleX + gyroX * (currentTime - previousTime);
  gyroAngleY = gyroAngleY + gyroY * (currentTime - previousTime);
  gyroAngleZ = gyroAngleZ + gyroZ * (currentTime - previousTime);
}

/*Найдём ошибку данных*/
void calculate_IMU_error(long cTime) {
  previousTime = millis();

  int16_t GyroErrorX = 0, GyroErrorY = 0, GyroErrorZ = 0, c = 0;

  while (millis() - previousTime < cTime) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    
    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
    
    GyroErrorX = GyroErrorX + (gyroX / 32768 * 1000);
    GyroErrorY = GyroErrorY + (gyroY / 32768 * 1000);
    GyroErrorZ = GyroErrorZ + (gyroZ / 32768 * 1000);

    c++;
  }

  GyroErrorX = GyroErrorX / c;
  GyroErrorY = GyroErrorY / c;
  GyroErrorZ = GyroErrorZ / c;

  int16_t err[] = {GyroErrorX, GyroErrorY, GyroErrorZ};
}
