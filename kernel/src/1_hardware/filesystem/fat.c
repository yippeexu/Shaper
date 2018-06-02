
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <blockdevice.h>
#include <shapervfs.h>
#include <fat.h>

vfs_entry_t current_volume;

void fat_select_volume(int volume_number);
int fat_get_rootdir_entries(vfs_entry_t entries[]);

void fat_init(blockdevice_t *block_device, fattype_t fat_type, fat32_bpb_t bpb)
{
    if (fat_type == FAT_TYPE_FAT12 || FAT_TYPE_FAT16) 
    {
        vfs_entry_t volume_entry;
        volume_entry.entrytype = VFS_ENTRYTYPE_VOL;
        for (int i = 0; i < lengthof(int2str(volume_count)); i++)
        volume_entry.name[i] = int2str(volume_count)[i];
        volume_entry.partition = block_device;
        volume_entry.size = bpb.total_sectors * 512 / 1024;
        vfs_add_volume(volume_entry);
    }
}

int fat_get_rootdir_entries(vfs_entry_t entries[])
{
    fat32_bpb_t *bpb = current_volume.partition->dev_specifics[3];
    uint32_t fat_size = (bpb->fats_count == 0 ? bpb->fats_count_32 : bpb->fats_count);
    uint32_t rootdir_sectors = ((bpb->rootdir_entries * 32) + (bpb->bytes_per_sector - 1)) / bpb->bytes_per_sector;
    uint32_t first_data_sector = bpb->reserved_sectors + (bpb->sectors_per_fat * fat_size) + rootdir_sectors;
    uint32_t first_rootdir_sector = first_data_sector - rootdir_sectors;

    uint8_t *rootdir = memalloc(bpb->rootdir_entries * 32);
    current_volume.partition->read_sector(current_volume.partition, first_rootdir_sector, rootdir_sectors, rootdir);

    int length = 0;
    int entryindex = 0;
    int entriesindex = 0;
    vfs_entry_t cur_entry;
    while (true)
    {
        if (rootdir[entryindex] == 0x00) break;

        if (rootdir[entryindex] == 0xE5)
        {
            entryindex += 0x20;
            continue;
        }
        
        for (length = 0; rootdir[entryindex+length] != 0x20; length++)
        {
            if (length >= 8) break;
            cur_entry.name[length] = rootdir[entryindex+length];
            cur_entry.name[length+1] = 0;
        }

        if (rootdir[entryindex+11] == 0x0F)
        {
            entryindex += 0x40;
            continue;
        }
        else
        {
            cur_entry.extension[0] = rootdir[entryindex+8];
            cur_entry.extension[1] = rootdir[entryindex+9];
            cur_entry.extension[2] = rootdir[entryindex+10];
            cur_entry.extension[3] = 0;
        }

        if (rootdir[entryindex+11] & 0x02)
            cur_entry.attributes |= VFS_ATTRIB_HIDDEN;
        
        if (rootdir[entryindex+11] & 0x04)
            cur_entry.attributes |= VFS_ATTRIB_SYSTEM;

        if (rootdir[entryindex+11] & 0x10)
            cur_entry.entrytype = VFS_ENTRYTYPE_DIR;
        else
            cur_entry.entrytype = VFS_ENTRYTYPE_FILE;

        //datetime_t creation_dt;
        //datetime_t modification_dt;
        //creation_dt.year = rootdir[entryindex+16] & 0x7F;
        //modification_dt.year = rootdir[entryindex+24] & 0x7F;
        //creation_dt.month = rootdir[entryindex+16] >> 7;
        //modification_dt.month = rootdir[entryindex+24] >> 7;
        //creation_dt.month |= (rootdir[entryindex+17] & 0x07) << 1;
        //modification_dt.month |= (rootdir[entryindex+25] & 0x07) << 1;
        //creation_dt.monthday = (rootdir[entryindex+17] >> 3) & 0x1F;
        //modification_dt.monthday = (rootdir[entryindex+25] >> 3) & 0x1F;
        //creation_dt.hour = rootdir[entryindex+14] & 0x1F; 
        //modification_dt.hour = rootdir[entryindex+22] & 0x1F; 
        //creation_dt.minutes = (rootdir[entryindex+14] >> 5) & 0x07;
        //modification_dt.minutes = (rootdir[entryindex+22] >> 5) & 0x07;
        //creation_dt.minutes |= (rootdir[entryindex+15] & 0x07) << 3;
        //modification_dt.minutes |= (rootdir[entryindex+23] & 0x07) << 3;
        //creation_dt.seconds = ((rootdir[entryindex+15] >> 3) & 0x1F) * 2;
        //modification_dt.seconds = ((rootdir[entryindex+23] >> 3) & 0x1F) * 2;

        //cur_entry.last_modify_date_time = modification_dt;
        //cur_entry.creation_date_time = creation_dt;

        cur_entry.size = (rootdir[entryindex+31] << 24 | rootdir[entryindex+30] << 16 | rootdir[entryindex+29] << 8 | rootdir[entryindex+28]);
        
        cur_entry.data_sector = ((((rootdir[entryindex+21] << 24) | rootdir[entryindex+20] << 16) | (rootdir[entryindex+27] << 8 | rootdir[entryindex+26]) - 2) * bpb->sectors_per_cluster) + first_data_sector;

        entries[entriesindex] = cur_entry;

        entryindex += 0x20;
        entriesindex++;
    }

    return entriesindex;
}

uint8_t *fat_file_open(char *src, vfs_entry_t *entry)
{
    bool has_extension = true;
    int i = 0;
    int l = 0;
    //for (i = 0; i < lengthof(src); i++)
    //    if (src[i] == ':') break;

    // Find the extension seperator starting from the end to avoid file name with dots
    for (l = lengthof(src)-1; l >= 0; l--)
    {
        if (src[l] == '.') break;
        else has_extension = false;
    }

    //char *volnum_str = memalloc(i);
    char *filename_str = memalloc(64);
    char *extension_str = memalloc(4);
    //for (int e = 0; e < i; e++)
    //{
    //    volnum_str[e] = src[e];
    //}

    //volnum_str[i] = 0;

    //memcpy(filename_str, src+2+i, l-2-i);
    //filename_str[l-2-i] = 0;
    
    memcpy(filename_str, src, l);
    filename_str[l] = 0;

    memcpy(extension_str, src+l+1, lengthof(src+l+1));
    extension_str[lengthof(src+l+1)] = 0;

    //fat_select_volume(volnum);
    fat_select_volume(0);

    vfs_entry_t files[256];
    int len = fat_get_rootdir_entries(files);

    for (int k = 0; k < len; k++)
    {
        if (!strcmp(filename_str, files[k].name) &&
            !strcmp(extension_str, files[k].extension))
        {
            uint8_t *data = memalloc((ceil(files[k].size / 512) + 1) * 512);
            current_volume.partition->read_sector(current_volume.partition, files[k].data_sector, files[k].size / 512 + 1, data);
            memcpy(entry, &files[k], sizeof(entry));
            entry->size = files[k].size;
            return data;
        }
    }
    printstrf("%cGfat: %n");
    printstr(filename_str);
    printchr('.');
    printstr(extension_str);
    printstr(" file not found!");

    entry = NULL;
    return NULL;
}

void fat_select_volume(int volume_number)
{
    printstr("fat: selecting volume ");
    printnum(volume_number);
    printchr('\n');
    current_volume = vfs_volumes[volume_number];
}