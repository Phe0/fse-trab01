#include <stdio.h>
#include <ncurses.h>

#define ON_OFF 0
#define PID    1

#define TERMINAL       0
#define POTENCIOMETRO  1

#define CHAVE 1

void init_menu();
void close_menu();
WINDOW * get_menu(int size);
void set_menu_title(WINDOW * menuwin, char * title);
int get_option(WINDOW * menuwin, char * choices[], int size);
int menu(char* title, char* options[], int size);
int choose_strategy_source();
int open_menu();
void show_temp(float te, float ti, float tr);
int choose_control_strategy();
int choose_reference_temp_source();
float get_float_menu(char * msg);
void clear_window(WINDOW* win);