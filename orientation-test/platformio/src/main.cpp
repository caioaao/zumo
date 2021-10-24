#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>
#include <HardwareSerial.h>

#include <stdio.h>

#include "Vector3D.h"

Zumo32U4LCD lcd;
L3G gyro;
LSM303 magAcc;
const int16_t LSM303_G_CONV_RATIO = 16384;

vector3D<float> accelValues;
vector3D<float> pos;
vector3D<float> vel;

inline void setupGyro() {
    gyro.init();
    // 800 Hz output data rate,
    // low-pass filter cutoff 100 Hz.
    gyro.writeReg(L3G::CTRL1, 0b11111111);

    // 2000 dps full scale.
    gyro.writeReg(L3G::CTRL4, 0b00100000);

    // High-pass filter disabled.
    gyro.writeReg(L3G::CTRL5, 0b00000000);
}

inline void setupMagAcc() {
    magAcc.init();
    magAcc.enableDefault();
    // 50 Hz output data rate
    magAcc.writeReg(LSM303::CTRL1, 0x57);
}

inline void readAcc(bool triggerSensor) {
    if(triggerSensor) magAcc.read();
    accelValues.x = (float)magAcc.a.x / LSM303_G_CONV_RATIO;
    accelValues.y = (float)magAcc.a.y / LSM303_G_CONV_RATIO;
    accelValues.z = (float)magAcc.a.z / LSM303_G_CONV_RATIO;
}

void setupSensors() {
    setupGyro();
    setupMagAcc();
    setupMagAcc();
}

void setup() {
    ledYellow(1);
    Wire.begin();
    setupSensors();
    lcd.clear();
    Serial.begin(9600);
    ledYellow(0);
}

char serial_buf [100];

void loop() {
    readAcc(true);
    sprintf(serial_buf, "accel: (%.3f, %.3f, %.3f) ;; pos (%d, %d, %d)\n",
            accelValues.x, accelValues.y, accelValues.z,
            magAcc.m.x, magAcc.m.y, magAcc.m.z
        );

    Serial.print(serial_buf);
    delay(500);
}
