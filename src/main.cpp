/**
 * {
 * \file       main.cpp
 * \brief      Main functionality of board
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

#include <Wire.h>
#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Adafruit_SSD1306.h>

/***********************************************************************************************************************
 **                                                   DEFINES/MACROS                                                  **
 **********************************************************************************************************************/

#define SCREEN_WIDTH 128        ///< OLED display width, in pixels
#define SCREEN_HEIGHT 32        ///< OLED display height, in pixels
#define OLED_RESET     4        ///< Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     ///< See data sheet for Address; 0x3D for 128x64, 0x3C for 128x32

/***********************************************************************************************************************
 **                                                 INTERNAL VARIABLES                                                **
 **********************************************************************************************************************/

MPU6050 mpu6050(Wire);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
long timer = 0;

static struct {
    float temperature;

    float x;
    float y;
    float z;

    struct
    {
        float x;
        float y;
        float z;
        float angleX;
        float angleY;
    } accelerometers;
    struct
    {
        float x;
        float y;
        float z;
        float angleX;
        float angleY;
        float angleZ;
    } gyroscope;
} self;

/***********************************************************************************************************************
 **                                           INTERNAL FUNCTION DECLARATIONS                                          **
 **********************************************************************************************************************/

void displayTestText(void);
void updateInternalData(void);
void sendDataToPc(void);

/***********************************************************************************************************************
 **                                                FUNCTION DEFINITIONS                                               **
 **********************************************************************************************************************/

void setup()
{
    Serial.begin(112500);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;){}; // Don't proceed, loop forever
    }

    display.display();

    displayTestText();

    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void loop()
{
    mpu6050.update();
    updateInternalData();

    if (millis() - timer > 30)
    {
        sendDataToPc();
        displayTestText();
        timer = millis();
    }
}

float roll,pitch,rollF,pitchF=0;

void sendDataToPc(void)
{
    float X_out, Y_out, Z_out;

    X_out = mpu6050.getRawAccX();
    Y_out = mpu6050.getRawAccY();
    Z_out = mpu6050.getRawAccZ();

    // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
    roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
    pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;

    // Low-pass filter
    rollF = 0.94 * rollF + 0.06 * roll;
    pitchF = 0.94 * pitchF + 0.06 * pitch;

    union
    {
        int32_t data;
        byte bytes[4];
    } block;

    byte bytes[11];

    bytes[0] = 0x24;

    block.data = rollF;
    bytes[1] = block.bytes[0];
    bytes[2] = block.bytes[1];
    bytes[3] = block.bytes[2];
    bytes[4] = block.bytes[3];

    block.data = pitchF;
    bytes[5] = block.bytes[0];
    bytes[6] = block.bytes[1];
    bytes[7] = block.bytes[2];
    bytes[8] = block.bytes[3];

    bytes[9] = '\r';
    bytes[10] = '\n';

    for (int index = 0; index < 11; index++)
    {
       Serial.write(bytes[index]);
    }

    // Serial.write((uint8_t *)&frame.bytes[0], sizeof(frame));
}

/**
 ***********************************************************************************************************************
 * \brief Display the data get from MPU6050 to the SSD1306
 **********************************************************************************************************************/
void displayTestText(void)
{
    display.clearDisplay();
    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner

    display.println(F("Acc: AnX, AnY"));
    display.print(self.accelerometers.x);
    display.print(" ");
    display.println(self.accelerometers.y);

    display.println(F("Gyro: AnX, AnY, AnZ"));
    display.print(self.gyroscope.angleX);
    display.print(" ");
    display.print(self.gyroscope.angleY);
    display.print(" ");
    display.print(self.gyroscope.angleZ);

    display.display();
}

/**
 ***********************************************************************************************************************
 * \brief Read and store the data read from the MPU6050
 **********************************************************************************************************************/
void updateInternalData(void)
{
    self.temperature = mpu6050.getTemp();

    self.accelerometers.x = mpu6050.getAccX();
    self.accelerometers.y = mpu6050.getAccY();
    self.accelerometers.z = mpu6050.getAccZ();
    self.accelerometers.angleX = mpu6050.getAccAngleX();
    self.accelerometers.angleY = mpu6050.getAccAngleY();

    self.gyroscope.x = mpu6050.getGyroX();
    self.gyroscope.y = mpu6050.getGyroY();
    self.gyroscope.z = mpu6050.getGyroZ();
    self.gyroscope.angleX = mpu6050.getGyroAngleX();
    self.gyroscope.angleY = mpu6050.getGyroAngleY();
    self.gyroscope.angleZ = mpu6050.getGyroAngleZ();

    self.x = mpu6050.getAngleX();
    self.y = mpu6050.getAngleY();
    self.z = mpu6050.getAngleZ();
}
