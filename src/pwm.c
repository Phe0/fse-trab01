#include "pwm.h"

void setup_pwm(int pin) {
    wiringPiSetup();
    pinMode(pin, OUTPUT);
    softPwmCreate(pin, 0, 100);
}

void set_pwm(int pin, int value) {
    if (pin == RESISTOR) {
        if (value > 0)
            softPwmWrite(VENTOINHA, 0);
        softPwmWrite(pin, value);
    } else if (pin == VENTOINHA) {
        if (value > 0)
            softPwmWrite(RESISTOR, 0);
        softPwmWrite(pin, value);
    }
}

void init_pwm() {
    setup_pwm(RESISTOR);
    setup_pwm(VENTOINHA);
}

void stop_pwm() {
    softPwmStop(RESISTOR);
    softPwmStop(VENTOINHA);
}