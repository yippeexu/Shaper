
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <ahci.h>

blockdevice_t *part_host;
uint32_t part0_lba, part1_lba, part2_lba, part3_lba;
uint32_t part0_count, part1_count, part2_count, part3_count;

int part0_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part1_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part2_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part3_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part0_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part1_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part2_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);
int part3_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer);

void mbr_getparts(blockdevice_t *block)
{
    part_host = block;
    if (!part_host->is_controller) return;
    if (part_host->is_packet) return;

    uint8_t mbr[512];
    part_host->read_sector(part_host, 0, 1, mbr);

    if (*((uint16_t *)&mbr[510]) != 0xAA55) {
        printwrn("mbr: raw hard disk found!");
        return;
    } 

    blockdevice_t *part_block[4];
    for (int i = 0; i < 4; i++)
    {
        if (*((uint32_t *)&mbr[458+i*16])) 
        {
            part_block[i] = memalloc(sizeof(blockdevice_t));
            memcpy(part_block[i]->signature, "BLKDEV", 6);
            memcpy(part_block[i]->dev_signature, "PART", 4);
            
            part_block[i]->is_controller = 0;
            part_block[i]->is_packet = 0;
            
            part_block[i]->dev_specifics = memalloc(16);
            part_block[i]->dev_specifics[0] = *((uint32_t *)&mbr[454+i*16]);
            part_block[i]->dev_specifics[1] = *((uint32_t *)&mbr[458+i*16]);
            part_block[i]->dev_specifics[2] = part_host;
        }
    }

    part_block[0]->read_sector = part0_read;
    part_block[0]->write_sector = part0_write;

    part_block[1]->read_sector = part1_read;
    part_block[1]->write_sector = part1_write;

    part_block[2]->read_sector = part2_read;
    part_block[2]->write_sector = part2_write;

    part_block[3]->read_sector = part3_read;
    part_block[3]->write_sector = part3_write;

    for (int i = 0; i < 4; i++)
        if (*((uint32_t *)&mbr[458+i*16])) 
            add_blockdevice(part_block[i]);
}

int part0_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part0_lba = block->dev_specifics[0];
    part0_count = block->dev_specifics[1];
    
    return part_host->read_sector(part_host, part0_lba + lba, count, buffer);
}   

int part1_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part1_lba = block->dev_specifics[0];
    part1_count = block->dev_specifics[1];
    
    return part_host->read_sector(part_host, part1_lba + lba, count, buffer);
}

int part2_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part2_lba = block->dev_specifics[0];
    part2_count = block->dev_specifics[1];
    
    return part_host->read_sector(part_host, part2_lba + lba, count, buffer);
}

int part3_read(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part3_lba = block->dev_specifics[0];
    part3_count = block->dev_specifics[1];
    
    return part_host->read_sector(part_host, part3_lba + lba, count, buffer);
}

int part0_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part0_lba = block->dev_specifics[0];
    part0_count = block->dev_specifics[1];
    
    return part_host->write_sector(part_host, part0_lba + lba, count, buffer);
}

int part1_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part1_lba = block->dev_specifics[0];
    part1_count = block->dev_specifics[1];
    
    return part_host->write_sector(part_host, part1_lba + lba, count, buffer);
}

int part2_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part2_lba = block->dev_specifics[0];
    part2_count = block->dev_specifics[1];
    
    return part_host->write_sector(part_host, part2_lba + lba, count, buffer);
}

int part3_write(blockdevice_t *block, uint32_t lba, uint32_t count, uint8_t *buffer)
{
    part_host = block->dev_specifics[2];
    part3_lba = block->dev_specifics[0];
    part3_count = block->dev_specifics[1];
    
    return part_host->write_sector(part_host, part3_lba + lba, count, buffer);
}