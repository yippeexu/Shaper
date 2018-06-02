
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <floppy.h>
#include <io.h>

// RQM = 7
// DIO = 6
// 

uint8_t* flp_buffer;
bool irq_recieved = false;

void flp_init()
{
    printstr("(FLP) Floppy Init");
    flp_sendcmd(FLP_CMD_VERSION, 0);
    //--flp_buffer;
    printnum(flp_buffer[0]);
}

void flp_sendcmd(flpcmd_t floppy_cmd, uint8_t parameter_byte)
{
    uint8_t msr = inb(IO_FLP_MAIN_STATUS);
    if ((msr & 0xC0) == 0x80) return;
        //flp_reset();
    outb(IO_FLP_DATA_FIFO, floppy_cmd);

    do
    {
        msr = inb(IO_FLP_MAIN_STATUS);
    } while (!(msr & 0x80));
    while (inb(IO_FLP_MAIN_STATUS) & 0x40) ;
    outb(IO_FLP_DATA_FIFO, parameter_byte);

    msr = inb(IO_FLP_MAIN_STATUS);
    if (!(msr & 0x20))
    {
        goto result_phase;
    }

    msr = 0;
    while (true)
    {
        while(!irq_recieved);
        irq_recieved = false;
        while ((inb(IO_FLP_MAIN_STATUS) & 0xA0) == 0xA0)
        {
            if (floppy_cmd == FLP_CMD_READ_SECTOR) {
                *flp_buffer = inb(IO_FLP_DATA_FIFO);
                flp_buffer++;
            } else if (floppy_cmd == FLP_CMD_WRITE_SECTOR) {
                outb(IO_FLP_DATA_FIFO, *flp_buffer);
                flp_buffer++;
            }
        }
    }
    return;

    result_phase:
    do
    {
        msr = inb(IO_FLP_MAIN_STATUS);
    } while (!(msr & 0x80));
    while (!(inb(IO_FLP_MAIN_STATUS) & 0x40));
    while ((inb(IO_FLP_MAIN_STATUS) & 0x50) != 0x50)
    {
        *flp_buffer = inb(IO_FLP_DATA_FIFO);
        flp_buffer++;
    }
}

void flp_handler()
{
    irq_recieved = true;
}