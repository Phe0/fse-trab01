#include "file.h"

FILE* fp;

void create_file() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char filename[28];

    snprintf(filename, 28, "logs/%d-%d-%d_%d:%d:%d.csv", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fp = fopen(filename, "w");

    fprintf(fp, "data;hora;temperatura interna;temperatura externa;temperatura relativa;sinal de controle\n");
}

void write_data_to_file(float ti, float te, float tr, int control_signal) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(fp, "%d/%d/%d;%d:%d:%d;%f;%f;%f;%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, ti, te, tr, control_signal);
}

void close_file() {
    fclose(fp);
}