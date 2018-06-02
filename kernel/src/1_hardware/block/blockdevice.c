
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <blockdevice.h>

void add_blockdevice(blockdevice_t *block_device)
{
    for (int i = 0; ; i++)
    {
        if (memcmp(blockdevices[i]->signature, "BLKDEV", 6)) continue; 
        blockdevices[i] = block_device;
        break;
    }
}