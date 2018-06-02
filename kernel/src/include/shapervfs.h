
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>

#ifndef $_SHAPERVFS_H
#define $_SHAPERVFS_H

// Lastest version of FAT16 BPB
typedef struct tag_fat16bpb fat16_bpb_t;
struct tag_fat16bpb 
{
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors; // Used for Determing Root dir sector
    uint8_t fats_count;
    uint16_t rootdir_entries;
    uint16_t total_sectors;
    uint8_t media_desc;
    uint16_t sectors_per_fat; 
    uint16_t sectors_per_track;
    uint16_t heads_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint8_t drive_num;
    uint8_t flags;
    uint8_t extended_boot_signature;
    uint32_t volume_serial_number;
    uint8_t volume_label[11];
    uint8_t fs_type[8];
} __attribute__((packed));

typedef struct tag_fat32bpb fat32_bpb_t;
struct tag_fat32bpb
{
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors; // Used for Determing Root dir sector
    uint8_t fats_count;
    uint16_t rootdir_entries;
    uint16_t total_sectors;
    uint8_t media_desc;
    uint16_t sectors_per_fat; 
    uint16_t sectors_per_track;
    uint16_t heads_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t fats_count_32;
    uint16_t mirroring_flags;
    uint16_t version;
    uint32_t rootdir_cluster;
    uint16_t location_of_fs_info_sector;
    uint16_t location_of_backup_sectors;
    uint8_t boot_filename[12];
    uint8_t drive_num;
    uint8_t flags;
    uint8_t extended_boot_signature;
    uint32_t volume_serial_number;
    uint8_t volume_label[11];
    uint8_t fs_type[8];
} __attribute__((packed));

typedef struct tag_fat fat_t;
struct tag_fat
{
    uint8_t file_name[11];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t accessed_date;
    uint16_t high_cluster;
    uint16_t modification_time;
    uint16_t modification_date;
    uint16_t low_cluster;
    uint32_t file_size;
} __attribute__((packed));

typedef struct tag_ntfsbpb ntfs_bpb_t;
struct tag_ntfsbpb
{
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors; // Used for Determing Root dir sector
    uint8_t fats_count;
    uint16_t rootdir_entries;
    uint16_t total_sectors;
    uint8_t media_desc;
    uint16_t sectors_per_fat; 
    uint16_t sectors_per_track;
    uint16_t heads_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint8_t drive_num;
    uint8_t flags;
    uint8_t extended_boot_signature;
    uint8_t reserved0;
    uint64_t volume_sectors;
    uint64_t mft_first_cluster;
    uint64_t mft_mirror_first_cluster;
    uint32_t mft_record_size;
    uint64_t volume_serial_number;
    uint32_t checksum;
} __attribute__((packed));

typedef enum tag_vfs_entrytype
{
    VFS_ENTRYTYPE_NULL,
    VFS_ENTRYTYPE_FILE,
    VFS_ENTRYTYPE_DIR,
    VFS_ENTRYTYPE_VOL
} vfs_entrytype_t;

typedef enum tag_vfsattributes
{
    VFS_ATTRIB_SYSTEM = 1,
    VFS_ATTRIB_HIDDEN = 2,
} vfs_attributes_t;

typedef struct tag_vfsentry
{
    char name[64];
    char extension[4];
    size_t size;
    blockdevice_t *partition;
    //datetime_t creation_date_time;
    //datetime_t last_modify_date_time;
    vfs_entrytype_t entrytype;
    vfs_attributes_t attributes;
    uint32_t data_sector;
} vfs_entry_t;

vfs_entry_t vfs_volumes[256];
size_t volume_count;

uint8_t *font_data;
uint8_t *bmptest_data;
uint8_t *nesgame_data;

void vfs_init(blockdevice_t *block_device);
void vfs_add_volume(vfs_entry_t volume_entry);
void file_delete(char *dest_path);
void file_copy(char *src_path, char *dest_path);
void file_move(char *src_path, char *dest_path);
uint8_t *file_open(char *src_path, vfs_entry_t *entry);

#endif