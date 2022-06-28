/**
 * {
 * \file       DataControl.cpp
 * \brief      The controller for data come form peripheral
 * \copyright  (C) 2022 Cerberus team
 *             The reproduction, distribution and utilization of this file as
 *             well as the communication of its contents to others without express
 *             authorization is prohibited. Offenders will be held liable for the
 *             payment of damages. All rights reserved in the event of the grant
 *             of a patent, utility model or design.
 * }
 */

/***********************************************************************************************************************
 **                                                      INCLUDES                                                     **
 **********************************************************************************************************************/

#include "Configure/Cfg.h"
#include <MPU6050_tockn.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include "DataControl.h"

/***********************************************************************************************************************
 **                                                   DEFINES/MACROS                                                  **
 **********************************************************************************************************************/

#define SCREEN_WIDTH   128      ///< OLED display width, in pixels
#define SCREEN_HEIGHT  32       ///< OLED display height, in pixels
#define OLED_RESET     4        ///< Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     ///< See data sheet for Address; 0x3D for 128x64, 0x3C for 128x32

/***********************************************************************************************************************
 **                                                 INTERNAL VARIABLES                                                **
 **********************************************************************************************************************/

/***********************************************************************************************************************
 **                                                FUNCTION DEFINITIONS                                               **
 **********************************************************************************************************************/

void DataControl::InitMpuAndDisplay(MPU6050 &mpu, Adafruit_SSD1306 &display)
{
    this->mpu6050 = &mpu;
    this->display = &display;
}

/**
 ***********************************************************************************************************************
 * \brief Use to init the list of peripheral that need to be use
 **********************************************************************************************************************/
void DataControl::InitPeripheral()
{
    MPU6050 mpu6050(Wire);
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    this->InitMpuAndDisplay(mpu6050, display);
    Serial.begin(112500);

    if (!this->display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;){};
    }

    this->display->display();

    Wire.begin();
    this->mpu6050->begin();
    this->mpu6050->calcGyroOffsets(true, 1000, 1000);

    pinMode(SIGNAL_RIGHT_PIN, INPUT);
    pinMode(SIGNAL_LEFT_PIN, INPUT);
    pinMode(ALIVE_LED_PIN, OUTPUT);
    pinMode(LIGHT_CONTROL_PIN, OUTPUT);
}

/**
 ***********************************************************************************************************************
 * \brief Send the Pitch and Roll to PC to visualize data from MPU6050
 **********************************************************************************************************************/
void DataControl::SendDataToPc(void)
{
    union
    {
        int32_t data;
        byte bytes[4];
    } block;

    byte dataFrame[11];

    dataFrame[0] = 0x24;

    block.data = this->roll;
    dataFrame[1] = block.bytes[0];
    dataFrame[2] = block.bytes[1];
    dataFrame[3] = block.bytes[2];
    dataFrame[4] = block.bytes[3];

    block.data = this->pitch;
    dataFrame[5] = block.bytes[0];
    dataFrame[6] = block.bytes[1];
    dataFrame[7] = block.bytes[2];
    dataFrame[8] = block.bytes[3];

    dataFrame[9] = '\r';
    dataFrame[10] = '\n';

    Serial.write(dataFrame, sizeof(dataFrame));
}

/**
 ***********************************************************************************************************************
 * \brief Update the data form MPU6050 and send data to Display or UART if any
 **********************************************************************************************************************/
void DataControl::UpdateAndProcessData()
{
    this->mpu6050->update();
    this->UpdateRollPitch();
    this->DisplayText();
}

/**
 ***********************************************************************************************************************
 * \brief Display the data get from MPU6050 to the SSD1306
 **********************************************************************************************************************/
void DataControl::DisplayText(void)
{
    this->display->clearDisplay();
    this->display->setTextSize(1);              // Normal 1:1 pixel scale
    this->display->setTextColor(SSD1306_WHITE); // Draw white text
    this->display->setCursor(0, 0);             // Start at top-left corner

    this->display->print(F("Roll value: "));
    this->display->println(this->roll);

    this->display->print(F("Pitch value: "));
    this->display->println(this->pitch);

    this->display->display();
}

/**
 ***********************************************************************************************************************
 * \brief Update the Roll value and Pitch value after get X,Y,Z value form MPU6050
 *
 * \param [in] RawAccX - Converted from raw 16-bit accelerometers X axis to float
 * \param [in] RawAccY - Converted from raw 16-bit accelerometers Y axis to float
 * \param [in] RawAccZ - Converted from raw 16-bit accelerometers Z axis to float
 **********************************************************************************************************************/
void DataControl::UpdateRollPitch()
{
    float RawAccX = (float)this->mpu6050->getAccX();
    float RawAccY = (float)this->mpu6050->getAccY();
    float RawAccZ = (float)this->mpu6050->getAccZ();
    float rawRoll;
    float rawPitch;

    // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
    rawRoll = atan(RawAccY / sqrt(pow(RawAccX, 2) + pow(RawAccZ, 2))) * 180 / PI;
    rawPitch = atan(-1 * RawAccX / sqrt(pow(RawAccY, 2) + pow(RawAccZ, 2))) * 180 / PI;

    // Low-pass filter
    this->roll = 0.94 * this->roll + 0.06 * rawRoll;
    this->pitch = 0.94 * this->pitch + 0.06 * rawPitch;
}

