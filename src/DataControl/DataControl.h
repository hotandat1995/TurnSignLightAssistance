#include "Arduino.h"
#include <MPU6050_tockn.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class DataControl {
public:
    DataControl(){};
    void UpdateAndProcessData();
    void InitPeripheral();

private:
    void InitMpuAndDisplay(MPU6050 &mpu, Adafruit_SSD1306 &display);
    void UpdateRollPitch();
    void SendDataToPc();
    void DisplayText();
    float roll;
    float pitch;
    MPU6050 *mpu6050;
    Adafruit_SSD1306 *display;
};
