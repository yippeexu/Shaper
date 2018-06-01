
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct tag_acpi_sdt_header acpi_sdtheader_t;
typedef struct tag_dsdt dsdt_t;
typedef struct tag_generic_address_structure gas_t;
typedef struct tag_rsdt rsdt_t;
typedef struct tag_rsdp rsdp_t;
typedef struct tag_rsdp_ext rsdp_ext_t;
typedef struct tag_fadt fadt_t;
typedef struct tag_madt madt_t;

struct tag_acpi_sdt_header
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_tableid[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct tag_generic_address_structure
{
    uint8_t address_space;
    uint8_t bitwidth;
    uint8_t bitoffset;
    uint8_t access_size;
    uint64_t address;
} __attribute__((packed));

struct tag_rsdt
{
    acpi_sdtheader_t header;
    acpi_sdtheader_t *other_sdt[256];
} __attribute__((packed));

struct tag_rsdp
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    rsdt_t *rsdt_base_address;
} __attribute__((packed));

struct tag_rsdp_ext
{
    rsdp_t rsdp;
    uint32_t length;
    uint64_t xsdt_base_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct tag_fadt
{
    acpi_sdtheader_t header;
    uint32_t firmware_control;
    dsdt_t *dsdt_base_address;

    uint8_t reserved;

    uint8_t preffered_pm_profile;
    uint16_t sci_interrupt;
    uint32_t smi_commandport;
    uint8_t acpi_enable;
    uint8_t acpi_disable;
    uint8_t s4bios_req;
    uint8_t pstate_control;
    uint32_t pm1a_eventblock;
    uint32_t pm1b_eventblock;
    uint32_t pm1a_controlblock;
    uint32_t pm1b_controlblock;
    uint32_t pm2_controlblock;
    uint32_t pm_timerblock;
    uint32_t gpe0_block;
    uint32_t gpe1_block;
    uint8_t pm1_eventlength;
    uint8_t pm1_controllength;
    uint8_t pm2_controllength;
    uint8_t pm_timerlength;
    uint8_t gpe0_length;
    uint8_t gpe1_length;
    uint8_t gpe1_base;
    uint8_t cstate_control;
    uint16_t worst_c2_latency;
    uint16_t worst_c3_latency;
    uint16_t flush_size;
    uint16_t flush_stride;
    uint8_t duty_offset;
    uint8_t duty_width;
    uint8_t day_alarm;
    uint8_t month_alarm;
    uint8_t century;

    uint16_t boot_architecture_flags;
    
    uint8_t reserved2;
    uint32_t flags;

    gas_t reset_reg;

    uint8_t reset_value;
    uint8_t reserved3[3];

    uint64_t x_firmware_control;
    uint64_t x_dsdt_base_address;

    gas_t x_pm1a_eventblock;
    gas_t x_pm1b_eventblock;
    gas_t x_pm1a_controlblock;
    gas_t x_pm1b_controlblock;
    gas_t x_pm2_controlblock;
    gas_t x_pm_timerblock;
    gas_t x_gpe0_block;
    gas_t x_gpe1_block;
} __attribute__((packed));

struct tag_madt
{
    acpi_sdtheader_t header;
    uint32_t apic_base_address;
    uint32_t flags;
} __attribute__((packed));

struct tag_dsdt
{
    acpi_sdtheader_t header;
    uint8_t aml_file[1];
} __attribute__((packed));

rsdp_t *rsdp;

void acpi_init();
void acpi_switch();
void acpi_shutdown();
void acpi_reboot();
uint8_t acpi_get_century_reg();
void acpi_setbrightness(int brightness);
acpi_sdtheader_t *acpi_find_sdt(rsdt_t *rsdt, char *signature);