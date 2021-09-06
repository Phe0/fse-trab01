#include <softPwm.h>
#include <wiringPi.h>

#define RESISTOR  4
#define VENTOINHA 5

void setup_pwm(int pin);
void set_pwm(int pin, int value);
void init_pwm();
void stop_pwm();