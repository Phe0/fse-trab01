#include "main.h"

pthread_t thread_1, thread_2, thread_3;

float ti, te, tr;
float histerese;
int control_signal = 0;
int ref_temp_source;

int control_strategy;
int strategy_source;
int has_stopped = 0;

double kp, ki, kd;

void update_temperatures(int strategy) {
    te = get_external_temperature();

    write_uart(0x23, 0xC1);
    ti = read_float_uart();

    if (strategy == POTENCIOMETRO) {
        write_uart(0x23, 0xC2);
        tr = read_float_uart();
        printw("1: %f", tr);
        if (control_strategy == PID) {
            pid_atualiza_referencia(tr);
        }
    }
    if (tr < te) {
        tr = te;
        printw("2: %f", tr);
    }
}

void on_off() {
    float top = tr + histerese/2.0;
    float bottom = tr - histerese/2.0;
    if (ti >= top) {
        set_pwm(VENTOINHA, 100);
        control_signal = -100;
    }
    else if (ti <= bottom) {
        set_pwm(RESISTOR, 100);
        control_signal = 100;
    }
}

void pid() {
    double pid_value = pid_controle(ti);
    if (pid_value > 0.0) {
        set_pwm(RESISTOR, pid_value);
    } else if(pid_value < 0.0 && pid_value > -40.0) {
        set_pwm(RESISTOR, 0);
        set_pwm(VENTOINHA, 0);
    } else {
        set_pwm(VENTOINHA, pid_value * -1);
    }
    control_signal = pid_value;
}

void *update_display(void *args) {
    while(1) {
        update_temperatures(ref_temp_source);
        write_lcd(ti, te, tr);

        if (control_strategy == ON_OFF) {
            on_off();
        } else if (control_strategy == PID) {
            pid();
        }
        write_int_uart(control_signal);

        show_temp(ti, te, tr);
        sleep(1);
    }
}

void *update_csv(void *args) {
    while(1) {
        write_data_to_file(ti, te, tr, control_signal);
        sleep(2);
    }
}

void handle_interuption(int value) {
    printw("entrou aqui");
    pthread_cancel(thread_1);
    pthread_cancel(thread_2);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    close_menu();
    close_file();
    close_uart();
    stop_pwm();
    exit(1);
}

int main() {

    signal(SIGINT, handle_interuption);
    signal(SIGTERM, handle_interuption);
    signal(SIGQUIT, handle_interuption);
    signal(SIGHUP, handle_interuption);
    signal(SIGTSTP, handle_interuption);

    init_uart();
    init_i2c();
    lcd_init();

    setup_pwm(RESISTOR);
    setup_pwm(VENTOINHA);

    init_menu();

    strategy_source = choose_strategy_source();
    if (strategy_source == TERMINAL)
        control_strategy = choose_control_strategy();

    ref_temp_source = choose_reference_temp_source();

    if (ref_temp_source == TERMINAL) {
        tr = get_float_menu("Qual valor deseja inserir para a temperatura de referência?");
    }
    if(control_strategy == ON_OFF) {
        histerese = get_float_menu("Qual valor deseja inserir para a histerese?");
    } else if (control_strategy == PID) {
        kp = get_float_menu("Qual valor deseja inserir para o kp?");
        ki = get_float_menu("Qual valor deseja inserir para o ki?");
        kd = get_float_menu("Qual valor deseja inserir para o kd?");
        pid_configura_constantes(kp, ki, kd);
        pid_atualiza_referencia(tr);
    }

    create_file();

    pthread_create(&thread_1, NULL, &update_display, NULL);
    pthread_create(&thread_2, NULL, &update_csv, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    close_menu();

    return 0;
}
