
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>
#include <shapervfs.h>
#include <fat.h>

uint8_t *(*fs_file_open)(char *src_path, vfs_entry_t *entry);

uint32_t total_sectors;
uint32_t fat_size;
uint32_t rootdir_sectors;
uint32_t first_data_sector;
uint32_t first_fat_sector;
uint32_t data_sectors;
uint32_t total_clusters;

void vfs_init(blockdevice_t *block_device)
{
    if (block_device->dev_specifics[2] == 0) return;

    uint8_t *boot_sector = memalloc(512);
    block_device->read_sector(block_device, 0, 1, boot_sector);

    fat32_bpb_t *bpb = boot_sector+11;
    block_device->dev_specifics[3] = bpb;
    
    total_sectors = (bpb->total_sectors == 0 ? bpb->total_sectors_32 : bpb->total_sectors);
    fat_size = (bpb->fats_count == 0 ? bpb->fats_count_32 : bpb->fats_count);
    rootdir_sectors = ((bpb->rootdir_entries * 32) + (bpb->bytes_per_sector - 1)) / bpb->bytes_per_sector;
    first_data_sector = bpb->reserved_sectors + (bpb->sectors_per_fat * fat_size) + rootdir_sectors;
    first_fat_sector = bpb->reserved_sectors;
    data_sectors = bpb->total_sectors - (bpb->reserved_sectors + (bpb->fats_count * fat_size) + rootdir_sectors);
    total_clusters = data_sectors / bpb->sectors_per_cluster;

    if (total_sectors < 4085*bpb->sectors_per_cluster) {
        fat_init(block_device, 0, *bpb);
        
        fs_file_open = fat_file_open;
        printstr("shapervfs: volume ");
        printnum(volume_count);
        printstr(" = fs: fat12, size: ");
        printnum(block_device->dev_specifics);
        printchr('\n');
    } else if (total_sectors < 65525*bpb->sectors_per_cluster) {
        fat_init(block_device, 1, *bpb);
        
        fs_file_open = fat_file_open;
        printstr("shapervfs: volume ");
        printnum(volume_count);
        printstr(" = fs: fat16, size: ");
        printnum(block_device->dev_specifics);
        printchr('\n');
    } else if (total_sectors < 268435445*bpb->sectors_per_cluster) {
        fat_init(block_device, 2, *bpb);
        printstr("shapervfs: volume ");
        printnum(volume_count);
        printstr(" = unsupported fs: fat32, size: ");
        printnum(block_device->dev_specifics);
        printchr('\n');
    }
}

void vfs_add_volume(vfs_entry_t volume_entry)
{
    for (int i = 0; i < 256; i++)
    {
        if (vfs_volumes[i].entrytype != VFS_ENTRYTYPE_NULL) continue;
        vfs_volumes[i] = volume_entry;
    }
}

// Implementing other features in the future

uint8_t *file_open(char *src_path, vfs_entry_t *entry)
{
    return fs_file_open(src_path, entry);
}