
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>

#include <acpi.h>
#include <io.h>

rsdp_t *acpi_find_rsdp();
uint8_t acpi_checksum(uint8_t *sd_ptr, uint8_t sd_length);
acpi_sdtheader_t *acpi_find_sdt(rsdt_t *rsdt, char *signature);

char rsdp_signature[] =
{
    'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '
};

fadt_t *fadt;
dsdt_t *dsdt;

void acpi_init()
{
    printstrf("%cR%n");
    printstr("acpi: looking for a vaild rsdp\n");
    rsdp = acpi_find_rsdp();
    fadt = (fadt_t *)acpi_find_sdt(rsdp->rsdt_base_address, "FACP");
    dsdt = fadt->dsdt_base_address;
}

void acpi_switch()
{
    printstrf("%cR%n");
    outb(fadt->smi_commandport, fadt->acpi_enable);
    while (inw(fadt->pm1a_controlblock) & 1 == 0) ;
}

bool irq_handled;

void acpi_handler()
{
    irq_handled = true;
    int scan_code = inb(0x60);
    if (scan_code & 0x80) return;
}

void acpi_shutdown()
{
    printstrf("%cR%n");
    fadt_t *fadt = (fadt_t *)acpi_find_sdt(rsdp->rsdt_base_address, "FACP");
    dsdt_t *dsdt = fadt->dsdt_base_address;
    
    uint8_t *s5_addr = dsdt->aml_file;
    int dsdt_length = dsdt->header.length;

    while (0 < dsdt_length--)
    {
        if (memcmp(s5_addr, "_S5_", 4))
            break;
        s5_addr++;
    }

    if (dsdt_length > 0)
    {
        if ((*(s5_addr-1) == 0x08) || ((*(s5_addr-2) == 0x08 && *(s5_addr-1) == '\\') && *(s5_addr+4) == 0x12))
        {
            uint16_t slp_typa, slp_typb;
            uint16_t slp_en = 1 << 13;

            s5_addr += 5;
            s5_addr += ((*s5_addr & 0xC0) >> 6) + 2;
            
            if (*s5_addr == 0x0A)
                s5_addr++;

            slp_typa = *(s5_addr) << 10;
            s5_addr++;

            if (*s5_addr == 0x0A)
                s5_addr++;
            slp_typb = *(s5_addr) << 10;

            outw(fadt->pm1a_controlblock, slp_typa | slp_en);
            if (fadt->pm1b_controlblock != 0)
                outw(fadt->pm1b_controlblock, slp_typb | slp_en);
        } else {
            printstr("acpi: \\_S5 parse error!\n");
        }
    } else {
        printstr("acpi: \\_S5 parse error!\n");
    }
}

void acpi_reboot()
{
    printstrf("%cR%n");
    printstr("acpi: rebooting this laptop\n");
    fadt_t *fadt = (fadt_t *)acpi_find_sdt(rsdp->rsdt_base_address, "FACP");
    if (fadt->header.revision >= 2 && fadt->flags & 1 << 10)
    {
        outb(fadt->reset_reg.address, fadt->reset_value);
    } else {
        printstr("acpi: acpi reboot is not supported!\n");
        printstr("ps2: rebooting!\n");
        __asm("cli");
        
        uint8_t temp;
        do {
            temp = inb(0x64);
            if ((temp & 1 << 0) != 0)
                inb(0x60);
        } while (temp & 1 << 1);

        outb(0x64, 0xFE);
        for (;;) __asm("hlt");
    }
    printstr("acpi: failed to reboot!\n");
}

uint8_t acpi_get_century_reg()
{
    printstrf("%cR%n");
    fadt_t *fadt = (fadt_t *)acpi_find_sdt(rsdp->rsdt_base_address, "FACP");
    return fadt->century;
}

rsdp_t *acpi_find_rsdp()
{
    uint32_t ebda_ba = get_ebda_base_address();
    rsdp_t *rsdp_ptr = (rsdp_t *)ebda_ba;
    rsdp_t *rsdp = rsdp_ptr;
    while (!memcmp(rsdp->signature, rsdp_signature, 8))
    {
        if ((uint32_t)rsdp_ptr >= ebda_ba+0x400 && (uint32_t)rsdp_ptr < 0xE0000) rsdp_ptr = (rsdp_t *)0xE0000; 
        if ((uint32_t)rsdp_ptr >= 0x100000) {
            printstr("acpi: cannot find rsdp!\n");
            return 0;
        }
        rsdp = rsdp_ptr += 16;
    }

    if (acpi_checksum((uint8_t *)rsdp, 20))
    {
        printstr("acpi: invaild rsdp checksum!\n");
        return 0;
    }

    return rsdp;
}

acpi_sdtheader_t *acpi_find_sdt(rsdt_t *rsdt, char *signature)
{
    if (rsdt)
    {
        acpi_sdtheader_t *sdt;
        for (int i = 0; i < 8192; i++)
        {
            sdt = rsdt->other_sdt[i];
            if (memcmp(sdt->signature, signature, 4))
            {
                return rsdt->other_sdt[i];
            }
        }
        printstr("acpi: cannot find the sdt you're looking for!\n");
    }
    return 0;
}

uint8_t acpi_checksum(uint8_t *sd_ptr, uint8_t sd_length)
{
    uint8_t checksum = 0;
    for (int i = 0; i < sd_length; i++) checksum += *sd_ptr++;
    return checksum;
}