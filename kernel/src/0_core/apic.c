
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>

#include <cpuid.h>
#include <apic.h>
#include <acpi.h>
#include <pic.h>

bool apic_is_supported();

madt_t *madt;

void apic_init()
{
    printstrf("%cC%n");
    printstr("apic: checking for apic support!\n");

    if (!apic_is_supported()) {
        printstr("apic: apic not supported, using \"8259\" pic!\n");
        return;
    } 
    printstr("apic: apic is supported!\n");

    madt = (madt_t *)acpi_find_sdt(rsdp->rsdt_base_address, "APIC");

    //printhex(madt->apic_base_address, 0);

    _pic_remap(0x00, 0xFF, 0x08, 0xFF);
    uint32_t *local_apic_registers = (uint32_t *)madt->apic_base_address;
    uint32_t low, high;
    _get_msr(0x1B, &low, &high);
    _set_msr(0x1B, (low & 0xFFFFF000) | 0x800, 0);
    local_apic_registers[0x3C] |= 0x1FF;
    printstr("(APIC) APIC is now enabled, Recieving interrupts!");
    local_apic_registers[0x20] |= 0xFF;

    local_apic_registers[0xC8] &= 0xFFFEFF00;
    local_apic_registers[0xC8] |= 0x20;
    local_apic_registers[0xF8] = 0x3;
    local_apic_registers[0xE0] = 0xFFFFFFFF;

    local_apic_registers[0xC8] |= 0x10000;
    
    local_apic_registers[0xC8] |= 0x20020;
    local_apic_registers[0xF8] = 0x3;
    local_apic_registers[0xE0] = 0xFFFFFFFF - 10000;

    local_apic_registers[0xC8] &= ~(0x10000);
//
    printnum(local_apic_registers[0xC8] >> 12 & 1);
    for (;;) ;
}

void apic_sendEOI()
{
    ((uint32_t *)madt->apic_base_address)[0x2C] = 0;
}

bool apic_is_supported()
{
    uint32_t features = 0;
    _cpuid_get_features(&features, 0);
    return features >> 9 & 1;
}