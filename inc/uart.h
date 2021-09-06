#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "crc.h"
#include "error.h"

void clean_buffer();
void write_uart(char code, char subcode);
int read_int_uart();
float read_float_uart();
void init_uart();
void close_uart();
void write_int_uart(int value);