
#include <stdbool.h>
#include <stdint.h>

#include <pci.h>

#ifndef $_AHCI_H
#define $_AHCI_H

uint8_t ports_count;
bool supports_externalsata;
bool em_supported;
bool ccc_supported;
uint8_t cmd_slots;
bool partial_state_capable;
bool slumber_state_capable;
bool pio_multiple_drq_block;
bool fb_switching_supported;
bool supports_portmultiplier;
bool supports_ahcionly;
uint8_t interface_speed_support;
bool supports_cmdlist_override;
bool supports_activity_led;
bool supports_aggressive_link_power_management;
bool supports_staggered_spinup;
bool supports_mechanical_presence_switch;
bool supports_snotification_register;
bool supports_native_command_queuing;
bool supports_64bit_addressing; 

typedef struct tag_ahcigeneric
{
    uint32_t capabilities;
    uint32_t global_control;
    uint32_t interrupt_status;
    uint32_t ports_implemented;
    uint32_t ahci_version;
    uint32_t ccc_control;
    uint32_t ccc_ports;
    uint32_t em_location;
    uint32_t em_control;
    uint32_t extended_capabilities;
    uint32_t bios_handoff_control;
} ahci_generic_t;

typedef struct tag_ahciport
{
    uint32_t cmd_list_base_address;
    uint32_t cmd_list_base_address_upper;
    uint32_t fis_base_address;
    uint32_t fis_base_address_upper;
    uint32_t interrupt_status;
    uint32_t interrupt_enable;
    uint32_t command_status;
    uint32_t reserved0;
    uint32_t task_file_data;
    uint32_t port_signature;
    uint32_t sata_status;
    uint32_t sata_control;
    uint32_t sata_error;
    uint32_t sata_active;
    uint32_t cmd_issue;
    uint32_t sata_notification;
    uint32_t fb_switching_control;
    uint32_t device_sleep;
    uint32_t reserved1[10];
    uint32_t vendor_specific[4];
} ahci_port_t;

typedef struct tag_ahcicmdheader
{
    uint8_t cmdfis_length : 5;
    uint8_t atapi_cmd : 1;
    uint8_t write_cmd : 1;
    uint8_t perfetchable : 1;
    uint8_t reset_cmd : 1;
    uint8_t bist : 1;
    uint8_t clear_busy : 1;
    uint8_t reserved : 1;
    uint8_t port_multiplier_port : 4;
    uint16_t prdtl;

    uint32_t prdbc;

    uint32_t cmd_table_base_address;

    uint32_t cmd_table_base_address_upper;

    uint32_t dw4_reserved[4];

} ahci_cmdheader_t;

typedef struct tag_ahciprdt
{
    uint32_t buffer_base_address;

    uint32_t buffer_base_address_upper;

    uint32_t dw2_reserved;

    uint32_t buffer_byte_count : 22;
    uint16_t dw3_reserved : 9;
    uint8_t interrupt_on_completion : 1;
} ahci_prdt_t;

typedef struct tag_ahcicmdtable
{
    uint8_t command_fis[64];
    uint8_t atapi_packet[16];
    uint8_t reserved[48];
    ahci_prdt_t prdt[256];
} ahci_cmdtable_t;

typedef enum tag_ahcidrivetype
{
    AHCI_DRIVE_NULL,
    AHCI_DRIVE_NOTHING,
    AHCI_DRIVE_SATA,
    AHCI_DRIVE_SATAPI,
    AHCI_DRIVE_PORT_MULTIPLIER,
    AHCI_DRIVE_SEMB
} ahci_drivetype_t;

typedef struct tag_ahci_fisreg_h2d ahci_fisreg_h2d_t;
struct tag_ahci_fisreg_h2d
{
    uint8_t fis_type;

    uint8_t port_multiplier_port : 4;
    uint8_t dw0_reserved : 3;
    uint8_t is_command : 1;

    uint8_t command;
    uint8_t feature_low;
    
    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t feature_high;

    uint8_t count_low;
    uint8_t count_high;
    uint8_t icc;
    uint8_t control;

    uint8_t dw4_reserved[4];

} __attribute__((packed));

typedef enum tag_ahcifistype
{
    AHCI_FISTYPE_REG_H2D = 0x27,
    AHCI_FISTYPE_REG_D2H = 0x34,
    AHCI_FISTYPE_DMA_ACT = 0x39,
    AHCI_FISTYPE_DMA_SETUP = 0x41,
    AHCI_FISTYPE_DATA = 0x46,
    AHCI_FISTYPE_BIST = 0x58,
    AHCI_FISTYPE_PIO_SETUP = 0x5F,
    AHCI_FISTYPE_DEV_BITS = 0xA1
} ahci_fistype_t;

void ahci_init(pcidevice_t *ahci_device);

#endif