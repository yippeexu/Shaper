
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void serial_init(int port_num);
char serial_read();
void serial_write(char value);
void serial_select(int port_num);