/**
 * {
 * \file       DataControl.h
 * \brief      The controller for data come form peripheral
 * \copyright  (C) 2022 Cerberus team
 *             The reproduction, distribution and utilization of this file as
 *             well as the communication of its contents to others without express
 *             authorization is prohibited. Offenders will be held liable for the
 *             payment of damages. All rights reserved in the event of the grant
 *             of a patent, utility model or design.
 * }
 */

#ifndef __DATA_CONTROL__
#define __DATA_CONTROL__

#include "Arduino.h"
#include <MPU6050_tockn.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class DataControl {
public:
    DataControl(){};
    void UpdateAndProcessData();
    void InitPeripheral();
    float GetRoll();
    float GetPitch();

private:
    void InitMpu();
    void InitDisplay();
    void UpdateRollPitch();
    void SendDataToPc();
    void DisplayText();
    float roll;
    float pitch;
    MPU6050 *mpu6050;
    Adafruit_SSD1306 *display;
};

extern DataControl dataController;
#endif
