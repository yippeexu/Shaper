
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <shapervfs.h>

typedef enum tag_fattype
{
    FAT_TYPE_FAT12,
    FAT_TYPE_FAT16,
    FAT_TYPE_FAT32,
    FAT_TYPE_EXFAT, // FAT64
} fattype_t;

void fat_init(blockdevice_t *block_device, fattype_t fat_type, fat32_bpb_t bpb);
uint8_t *fat_file_open(char *src, vfs_entry_t *entry);