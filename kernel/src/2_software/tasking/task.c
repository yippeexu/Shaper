
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <paging.h>
#include <heap.h>
#include <task.h>
#include <x86.h>
#include <pit.h>
#include <pic.h>

static task_t *current_task;

static task_t *main_task; // System
static task_t *dummy_task; // Temporary task
static task_t *tasks[256];
static uint8_t tasks_length = 0;

extern void switch_registers(regs32_t *from, regs32_t *to); // Software context switching

void tasking_init()
{
    printstr("tasking: creating system task");
    dummy_task = memalloc(sizeof(task_t));
    main_task = memalloc(sizeof(task_t));
    main_task->task_context.cr3 = get_register32("cr3");
    main_task->task_context.eflags = get_register32("eflags");
    add_task(main_task);

    current_task = main_task;

    execute_after(yield, 100, true); // Preempt every 100 milliseconds
}

task_t *create_task(void (*main)(), uint32_t task_flags, pagedir_t *task_dir)
{
    task_t *new_task = memalloc(sizeof(task_t));

    new_task->task_context.cr3 = (uint32_t)task_dir;
    new_task->task_context.eflags = (uint32_t)task_flags;
    new_task->task_context.esp = (uint32_t)memalloc(0x1000) + 0x1000;
    new_task->task_context.eip = (uint32_t)main;
    add_task(new_task);
    return new_task;
}

void add_task(task_t *task)
{
    task->task_id = tasks_length;
    tasks[tasks_length++] = task;
}

void remove_task(task_t *task)
{
    memcpy(&tasks[task->task_id], &tasks[task->task_id + 1], (--tasks_length - task->task_id) * sizeof(task_t *));
    if (task == current_task) current_task = dummy_task;
    tasks[tasks_length] = 0;
}

void switch_task(task_t *next_task)
{
    if (current_task == next_task) return;
    task_t *last_task = current_task;
    current_task = next_task;
    switch_registers(&last_task->task_context, &current_task->task_context);
}
 
void exit()
{
    if (current_task == main_task) return;
    printstr("tasking: task ");
    printnum(current_task->task_id);
    printstr(" has exited.\n");
    remove_task(current_task);
    uint32_t next_taskid = current_task->task_id+1;
    switch_task(tasks[next_taskid % tasks_length]);
}

void yield()
{
    switch_task(tasks[(current_task->task_id+1) % tasks_length]);
}