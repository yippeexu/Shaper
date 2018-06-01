
#include <stdint.h>
#include <string.h>

#include <x86.h>

uint32_t get_register32(char *register_name)
{
    uint32_t value;
    if (!strcmp(register_name, "eax"))
        __asm__ ("movl %%eax, %0" : "=r"(value));
    else if (!strcmp(register_name, "ebx"))
        __asm__ ("movl %%ebx, %0" : "=r"(value));
    else if (!strcmp(register_name, "ecx"))
        __asm__ ("movl %%ecx, %0" : "=r"(value));
    else if (!strcmp(register_name, "edx"))
        __asm__ ("movl %%edx, %0" : "=r"(value));
    else if (!strcmp(register_name, "esi"))
        __asm__ ("movl %%esi, %0" : "=r"(value));
    else if (!strcmp(register_name, "edi"))
        __asm__ ("movl %%edi, %0" : "=r"(value));
    else if (!strcmp(register_name, "ebp"))
        __asm__ ("movl %%ebp, %0" : "=r"(value));
    else if (!strcmp(register_name, "esp"))
        __asm__ ("movl %%esp, %0" : "=r"(value));
    else if (!strcmp(register_name, "cr0"))
        __asm__ ("movl %%cr0, %%eax; movl %%eax, %0" : "=r"(value));
    else if (!strcmp(register_name, "cr2"))
        __asm__ ("movl %%cr2, %%eax; movl %%eax, %0" : "=r"(value));
    else if (!strcmp(register_name, "cr3"))
        __asm__ ("movl %%cr3, %%eax; movl %%eax, %0" : "=r"(value));
    else if (!strcmp(register_name, "cr4"))
        __asm__ ("movl %%cr4, %%eax; movl %%eax, %0" : "=r"(value));
    else if (!strcmp(register_name, "eflags"))
        __asm__ ("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;" : "=r"(value));
    return value;
}

void set_register32(char *register_name, uint32_t value)
{
    if (!strcmp(register_name, "eax"))
        __asm__ ("movl %0, %%eax" : : "r"(value));
    else if (!strcmp(register_name, "ebx"))
        __asm__ ("movl %0, %%ebx" : : "r"(value));
    else if (!strcmp(register_name, "ecx"))
        __asm__ ("movl %0, %%ecx" : : "r"(value));
    else if (!strcmp(register_name, "edx"))
        __asm__ ("movl %0, %%edx" : : "r"(value));
    else if (!strcmp(register_name, "esi"))
        __asm__ ("movl %0, %%esi" : : "r"(value));
    else if (!strcmp(register_name, "edi"))
        __asm__ ("movl %0, %%edi" : : "r"(value));
    else if (!strcmp(register_name, "ebp"))
        __asm__ ("movl %0, %%ebp" : : "r"(value));
    else if (!strcmp(register_name, "esp"))
        __asm__ ("movl %0, %%esp" : : "r"(value));
    else if (!strcmp(register_name, "cr0"))
        __asm__ ("movl %0, %%eax; movl %%eax, %%cr0" : : "r"(value));
    else if (!strcmp(register_name, "cr2"))
        __asm__ ("movl %0, %%eax; movl %%eax, %%cr2" : : "r"(value));
    else if (!strcmp(register_name, "cr3"))
        __asm__ ("movl %0, %%eax; movl %%eax, %%cr3" : : "r"(value));
    else if (!strcmp(register_name, "cr4"))
        __asm__ ("movl %0, %%eax; movl %%eax, %%cr4" : : "r"(value));
    else if (!strcmp(register_name, "eflags"))
        __asm__ ("push %0; popfl;" : : "r"(value));
}