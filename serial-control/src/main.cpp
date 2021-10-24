#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>
#include <HardwareSerial.h>

#include <stdio.h>

const int16_t MAX_SPEED = 400;
const unsigned long DEAD_RECKON_DELTA_MS = 100;

bool isMoving = false;
unsigned long stopMovingAt = 0;
Zumo32U4Motors motors;


void setup() {
    ledYellow(1);
    Wire.begin();
    Serial.begin(9600);
    ledYellow(0);
}

int16_t scaleSpeed(int16_t v) {
    return v * 3.125;
}

void setWheelsSpeeds(int16_t throttle, int16_t steer) {
    int16_t leftSpeed = scaleSpeed(throttle + steer);
    int16_t rightSpeed = scaleSpeed(throttle - steer);
    motors.setSpeeds(leftSpeed, rightSpeed);

    char out_buf[100];
    sprintf(out_buf, "throttle=%d; steer=%d; leftWheel=%d, rightWeel=%d\n", throttle, steer, leftSpeed, rightSpeed);
    Serial.print(out_buf);
}

char in_buf[2];

void loop() {
    ledYellow(isMoving);
    unsigned long tick = millis();
    if(Serial.peek() >= 0) {
        if(Serial.readBytes(in_buf, 2) == 2){
            setWheelsSpeeds(in_buf[0], in_buf[1]);
            isMoving = true;
            stopMovingAt = tick + DEAD_RECKON_DELTA_MS;
        }
    }

    else if(isMoving && stopMovingAt < tick) {
        isMoving = false;
        setWheelsSpeeds(0, 0);
    }
}
