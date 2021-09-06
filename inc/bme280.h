#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int bme280Init(int iChannel, int iAddr);
int bme280ReadValues(int *T, int *P, int *H);
void init_i2c();
float get_external_temperature();