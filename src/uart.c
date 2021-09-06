#include "uart.h"

unsigned char buffer[256] = {0};
unsigned char *a = &buffer[0];
char address = 0x01;
int fd;

void clean_buffer() {
    a = &buffer[0];
}

void write_uart(char code, char subcode) {
    *a++ = address;
    *a++ = code;
    *a++ = subcode;
    *a++ = 0;
    *a++ = 4;
    *a++ = 6;
    *a++ = 1;

    *a++ = calcula_CRC(buffer, 7);
    *a++ = calcula_CRC(buffer, 8);

    int byte_len = write(fd, buffer, 9);
    check(byte_len, "Erro ao escrever na UART.");

    sleep(1);
    clean_buffer();
}

void write_int_uart(int value) {
    *a++ = address;
    *a++ = 0x16;
    *a++ = 0xD1;
    *a++ = 0;
    *a++ = 4;
    *a++ = 6;
    *a++ = 1;
    *a++ = value & 0xFF;
    *a++ = (value >> 8) & 0xFF;
    *a++ = (value >> 16) & 0xFF;
    *a++ = (value >> 24) & 0xFF;

    *a++ = calcula_CRC(buffer, 11);
    *a++ = calcula_CRC(buffer, 12);

    check(write(fd, buffer, 13), "Erro ao enviar um int para a UART.");

    sleep(1);
    clean_buffer();
}

int read_int_uart() {
    char tmp;
    int ti;
    int byte_len;

    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);

    byte_len = read(fd, &ti, sizeof(int));

    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);
    check(byte_len, "Erro ao ler da UART.");

    return ti;
}

float read_float_uart() {
    char tmp;
    float ti;
    int byte_len;

    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);

    byte_len = read(fd, &ti, sizeof(float));

    byte_len = read(fd, &tmp, 1);
    byte_len = read(fd, &tmp, 1);
    check(byte_len, "Erro ao ler da UART.");

    return ti;
}

void init_uart() {
    fd = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
}

void close_uart() {
    close(fd);
}