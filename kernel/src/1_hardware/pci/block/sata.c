
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <encoding.h>
#include <ahci.h>
#include <sata.h>
#include <ata.h>

ahci_generic_t *ahci_mem;
ahci_port_t *sataport;
uint32_t port_num;

int sata_set_command(atacmd_t ahci_cmd, uint8_t *buffer);
int sata_set_command28(atacmd_t ahci_cmd, uint32_t lba, uint32_t count, uint8_t *buffer, bool write);
int sata_set_command48(atacmd_t ahci_cmd, uint32_t lba, uint32_t count, uint8_t *buffer, bool write);
bool sata_send_command(int cmd_slot);

void sata_rebase_port(ahci_port_t *port); 
int sata_get_freecmd(ahci_port_t *sata_port);
int sata_start_port(ahci_port_t *port);
int sata_stop_port(ahci_port_t *port);

void sata_init(ahci_generic_t *ahci_controller_mem, ahci_port_t *sata_port, int port_number)
{
    ahci_mem = ahci_controller_mem;
    sataport = sata_port;
    port_num = port_number;

    if ((sataport->port_signature >> 16) == 0xEB14 ||
        (sataport->port_signature >> 16) == 0x9669 ||
        (sataport->port_signature >> 16) == 0xC33C)
    {
        printerr("ahci: initializing non-sata port!", ERRCODE_NONE, true);
    }

    printstrf("%cgsataport%n");
    printnum(port_number);
    printstr(": initializing sata port\n");

    sata_rebase_port(sata_port);

    blockdevice_t *sata_block;
    memcpy(sata_block->signature, "BLKDEV", 6);
    memcpy(sata_block->dev_signature, "SATA", 4);
    sata_block->is_controller = true;
    sata_block->is_packet = false;

    uint32_t *sata_specifics = memalloc(8);
    sata_specifics[0] = port_number;
    sata_specifics[1] = ahci_mem;
    sata_block->dev_specifics = sata_specifics;

    uint16_t *sata_identify = memalloc(512);
    int cmd_slot = sata_set_command(ATA_CMD_IDENTIFY, sata_identify);
    sata_send_command(cmd_slot);
//
    change_endianness(&sata_identify[10], 20);
    change_endianness(&sata_identify[23], 8);
    change_endianness(&sata_identify[27], 40);
// //
    memcpy(sata_block->serial_no, &sata_identify[10], 20);
    memcpy(sata_block->firmware_rev, &sata_identify[23], 8);
    memcpy(sata_block->model_name, &sata_identify[27], 40);
// //
    sata_block->read_sector = sata_read;
    sata_block->write_sector = sata_write;

    //((bool (*)(void *, uint32_t, uint32_t, void *))
    //sata_block->read_sector)(sata_block, 0, 1, 0x8F00);

    //uint64_t *tmp = &sata_identify[100];
    ////sata_block.sectors_count = *tmp;
//

    //uint8_t mybuffer[512];
    //sata_read(sata_block, 0, 1, mybuffer);
    //int cmd_slot = sata_set_command(ATA_CMD_IDENTIFY, mybuffer);
    //sata_send_command(cmd_slot);
    // printhex(sata_identify[255], 0);
    // printhex(sata_identify[254], 0);
    // printchr('\n');

    add_blockdevice(sata_block);
}

void sata_rebase_port(ahci_port_t *port)
{
    sata_stop_port(port);

    uint32_t ahci_page = 0x300000;//memalloc_aligned((1024 + 256) * ports_count + (cmd_slots * sizeof(ahci_cmdtable_t)) * ports_count, 0x100000);
    
    uint8_t clb_address = ahci_page + (1024 * port_num);
    uint8_t fb_address = ahci_page + (1024 * ports_count) + (256 * port_num);
    port->cmd_list_base_address = clb_address;
    port->fis_base_address = fb_address;

    port->sata_error = 1;
    port->interrupt_status = 0;
    port->interrupt_enable = 0;

    memclr(clb_address, 1024);
    memclr(fb_address, 256);
    
    ahci_cmdheader_t *cmdheader = clb_address;
    for (int cmd_slot = 0; cmd_slot < cmd_slots; cmd_slot++)
    {
        cmdheader[cmd_slot].prdtl = 8;
        cmdheader[cmd_slot].atapi_cmd = 0;
        cmdheader[cmd_slot].cmd_table_base_address = ahci_page + (1024 + 256) * ports_count + (cmd_slot * sizeof(ahci_cmdtable_t)) + (port_num * (cmd_slots * sizeof(ahci_cmdtable_t)));
        memclr(cmdheader[cmd_slot].cmd_table_base_address, sizeof(ahci_cmdtable_t));
    }

    port->interrupt_status = 0;
    port->interrupt_enable = 0xFFFFFFFF;

    sata_start_port(port);
}

int sata_stop_port(ahci_port_t *port)
{
    port->command_status &= ~(1 << 0);

    //while(port->command_status & (1 << 14)) __asm("HLT");
    while(port->command_status & (1 << 15)) ;//__asm("HLT");

    port->command_status &= ~(1 << 4);
}

int sata_start_port(ahci_port_t *port)
{
    while (port->command_status & (1 << 15));

    port->command_status |= (1 << 4);
    port->command_status |= (1 << 0);
}

int sata_get_freecmd(ahci_port_t *sata_port)
{
    uint8_t slot_status = (sata_port->cmd_issue | sata_port->sata_active);

    for (int cmd_freeslot = 0; cmd_freeslot < cmd_slots; cmd_freeslot++)
        if (!((slot_status >> cmd_freeslot) & 1))
            return cmd_freeslot;

    return -1;
}

int sata_set_command(atacmd_t ahci_cmd, uint8_t *buffer)
{
    sataport->interrupt_status = -1;

    int cmd_slot;
    if (cmd_slot = sata_get_freecmd(sataport) == -1)
        return cmd_slot;

    ahci_cmdheader_t *cmd_header = sataport->cmd_list_base_address;
    cmd_header += cmd_slot;

    cmd_header->cmdfis_length = sizeof(ahci_fisreg_h2d_t)/4;
    cmd_header->write_cmd = 0;
    cmd_header->prdtl = 1;

    cmd_header->cmd_table_base_address = 0x00400000 + (cmd_slot * sizeof(ahci_cmdtable_t)) + (port_num * (cmd_slot * sizeof(ahci_cmdtable_t)));

    ahci_cmdtable_t *cmd_table = cmd_header->cmd_table_base_address;
    memclr(cmd_table, sizeof(ahci_cmdtable_t) + sizeof(ahci_prdt_t));
    
    cmd_table->prdt[0].buffer_base_address = buffer;
    cmd_table->prdt[0].buffer_base_address_upper = 0;//(buffer >> 32);
    cmd_table->prdt[0].buffer_byte_count = 511;
    cmd_table->prdt[0].interrupt_on_completion = 1;

    ahci_fisreg_h2d_t *command_fis;
    command_fis = &cmd_table->command_fis;

    command_fis->fis_type = AHCI_FISTYPE_REG_H2D;

    command_fis->is_command = true;
    command_fis->command = ahci_cmd;

    command_fis->device = 0; // Master
    
        return cmd_slot;
}

int sata_set_command28(atacmd_t ahci_cmd, uint32_t lba, uint32_t count, uint8_t *buffer, bool write)
{
    sataport->interrupt_status = -1;

    int cmd_slot;
    if (cmd_slot = sata_get_freecmd(sataport) == -1)
        return cmd_slot;

    ahci_cmdheader_t *cmd_header = sataport->cmd_list_base_address;
    cmd_header += cmd_slot;

    cmd_header->cmdfis_length = sizeof(ahci_fisreg_h2d_t)/4;
    cmd_header->write_cmd = write;
    cmd_header->prdtl = ((count - 1) >> 5) + 1;

    cmd_header->cmd_table_base_address = 0x00400000 + (cmd_slot * sizeof(ahci_cmdtable_t)) + (port_num * (cmd_slot * sizeof(ahci_cmdtable_t)));

    ahci_cmdtable_t *cmd_table = cmd_header->cmd_table_base_address;
    memclr(cmd_table, sizeof(ahci_cmdtable_t) + (cmd_header->prdtl-1) * sizeof(ahci_prdt_t));

    for (int i = 0; i < cmd_header->prdtl - 1; i++)
    {
        cmd_table->prdt[i].buffer_base_address = buffer;
        cmd_table->prdt[i].buffer_base_address_upper = 0;//(buffer >> 32);
        cmd_table->prdt[i].buffer_byte_count = 16383;
        cmd_table->prdt[i].interrupt_on_completion = 1;
        buffer += 8192;
        count -= 32;
    }
    
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_base_address = buffer;
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_base_address_upper = 0;//(buffer >> 32);
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_byte_count = (512 * count) - 1;
    cmd_table->prdt[cmd_header->prdtl - 1].interrupt_on_completion = 1;

    ahci_fisreg_h2d_t *command_fis;
    command_fis = &cmd_table->command_fis;

    command_fis->fis_type = AHCI_FISTYPE_REG_H2D;

    command_fis->is_command = true;
    command_fis->command = ahci_cmd;

    command_fis->lba0 = (lba >> 0x00) & 0xFF;
    command_fis->lba1 = (lba >> 0x08) & 0xFF;
    command_fis->lba2 = (lba >> 0x10) & 0xFF;
    
    command_fis->count_low  = (count >> 0x00) & 0xFF;

    command_fis->device = 0xE0 | ((lba >> 0x18) & 0x0F); // LBA 24 mode
    
    return cmd_slot;
}

int sata_set_command48(atacmd_t ahci_cmd, uint32_t lba, uint32_t count, uint8_t *buffer, bool write)
{
    sataport->interrupt_status = -1;

    int cmd_slot;
    if (cmd_slot = sata_get_freecmd(sataport) == -1)
        return cmd_slot;

    ahci_cmdheader_t *cmd_header = sataport->cmd_list_base_address;
    cmd_header += cmd_slot;

    cmd_header->cmdfis_length = sizeof(ahci_fisreg_h2d_t)/4;
    cmd_header->write_cmd = 0;
    cmd_header->prdtl = ((count - 1) >> 5) + 1;

    cmd_header->cmd_table_base_address = 0x00400000 + (cmd_slot * sizeof(ahci_cmdtable_t)) + (port_num * (cmd_slot * sizeof(ahci_cmdtable_t)));

    ahci_cmdtable_t *cmd_table = cmd_header->cmd_table_base_address;
    memclr(cmd_table, sizeof(ahci_cmdtable_t) + (cmd_header->prdtl-1) * sizeof(ahci_prdt_t));

    for (int i = 0; i < cmd_header->prdtl - 1; i++)
    {
        cmd_table->prdt[i].buffer_base_address = buffer;
        cmd_table->prdt[i].buffer_base_address_upper = 0;//(buffer >> 32);
        cmd_table->prdt[i].buffer_byte_count = 16383;
        cmd_table->prdt[i].interrupt_on_completion = 1;
        buffer += 8192;
        count -= 32;
    }
    
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_base_address = buffer;
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_base_address_upper = 0;//(buffer >> 32);
    cmd_table->prdt[cmd_header->prdtl - 1].buffer_byte_count = (512 * count) - 1;
    cmd_table->prdt[cmd_header->prdtl - 1].interrupt_on_completion = 1;

    uint8_t *commandfis_bytes = &cmd_table->command_fis;
    ahci_fisreg_h2d_t *command_fis = &cmd_table->command_fis;

    command_fis->fis_type = AHCI_FISTYPE_REG_H2D;

    command_fis->is_command = true;
    command_fis->command = ahci_cmd;

    command_fis->lba0 = (lba >> 0x00) & 0xFF;
    command_fis->lba1 = (lba >> 0x08) & 0xFF;
    command_fis->lba2 = (lba >> 0x10) & 0xFF;

    command_fis->lba3 = (lba >> 0x18) & 0xFF;
    command_fis->lba4 = 0x00;//(lba >> 0x20) & 0xFF;
    command_fis->lba5 = 0x00;//(lba >> 0x28) & 0xFF;
    
    command_fis->count_low  = (count >> 0x00) & 0xFF;
    command_fis->count_high = (count >> 0x08) & 0xFF;

    command_fis->device = 0x40; // LBA 48 mode

    return cmd_slot;
}

bool sata_send_command(int cmd_slot)
{
    while (sataport->task_file_data & ((1 << 7) | (1 << 3)));

    sataport->cmd_issue = (1 << cmd_slot);

    while (1)
    {
        if ((sataport->cmd_issue & (1 << cmd_slot)) == 0) break;
        if (sataport->interrupt_status & (1 << 30))
        {    
            printstr("sataport");
            printnum(port_num);
            printstr(": error occured while sending command!\n");
            return false;
        }
    }

    return true;
}

bool sata_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    // if (count > block->sectors_count) {
    //    return false;
    // } else if (lba > block->sectors_count) {
    //    return false;
    // }

    printstr("sataport");
    printnum(port_num);
    printstr(": reading ");
    printnum(count);
    printstr(" sectors starting from lba ");
    printhex(lba, 0);
    printchr('\n');

    uint32_t *ahci_specifics = block->dev_specifics;
    port_num = ahci_specifics[0]; 
    ahci_mem = ahci_specifics[1];
    sataport = ahci_specifics[1] + 0x100 + (0x80 * port_num);

    int cmd_slot = sata_set_command48(ATA_CMD_READ_DMA_EXT, lba, count, buffer, false);
    if (cmd_slot == -1) return false;

    return sata_send_command(cmd_slot);
}

bool sata_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    //if (count > block.sectors_count) {
    //    return false;
    //} else if (lba > block.sectors_count) {
    //    return false;
    //}
    
    printstr("sataport");
    printnum(port_num);
    printstr(": reading ");
    printnum(count);
    printstr(" sectors starting from lba ");
    printhex(lba, 0);
    printchr('\n');

    uint32_t *ahci_specifics = block->dev_specifics;
    port_num = ahci_specifics[0];
    ahci_mem = ahci_specifics[1];
    sataport = ahci_specifics[1] + 0x100 + (0x80 * port_num);

    int cmd_slot = sata_set_command48(ATA_CMD_WRITE_DMA_EXT, lba, count, buffer, true);
    if (cmd_slot == -1) return false;

    bool status = sata_send_command(cmd_slot);

    return status;
}