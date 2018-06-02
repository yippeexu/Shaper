
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <paging.h>

typedef struct tag_task task_t;
struct tag_task
{
    regs32_t task_context;
    char *task_args;
    uint8_t task_id;
};

void tasking_init();
task_t *create_task(void (*main)(), uint32_t task_flags, pagedir_t *task_pagedir);
void yield();
void exit();