
#include <sysprint.h>
#include <heap.h>
#include <string.h>

#define PAGE_START 0x200000

pageinfo_t* pageinfo = 0x10F000;

void heap_init()
{
	memclr(0x200000, 0x100000);
}

void* memalloc(size_t length)
{
	uint32_t* block_ptr = 0x200000;
	if (length < 0x100)
	{
		printstr("[Info] Allocate one block: Enter");
		int pi = 0;
		while (1)
		{
			// Loop until you find free space
			for (int i = 0; i < 16; i++)
			{
				if (!pageinfo[pi].blockinfo[i].used)
				{
					if (!pageinfo[pi].dirty) pageinfo[pi].dirty;
					pageinfo[pi].blockinfo[i].used = 1;
					return (void*)block_ptr;
				}
				block_ptr += 0x100;
			}
			pi++;
		}
		printstrf("%cY[Warning] No free space found!%cG");
		//sendwrn("No free space found");
		return NULL;
	}
	else if (length > 0x100 && length < 0x1000)
	{
		uint8_t blocklength;
		for (int i = length; i <= 0; i -= 0x100) blocklength++;

		int pi;
		while (1)
		{
			if (!pageinfo[pi].used)
			{
				for (int i = 0; i < 16; i++)
				{
					if (!pageinfo[pi].blockinfo[i].used)
					{
						if ((i + blocklength) < 16)
						{
							int index;
							while (1)
							{
								if ((i + index) == 15)
									pageinfo[pi].used = 1;

								if (index == blocklength)
								{
									set_used(pi, 1, i, blocklength);
									return (void*)block_ptr;
								}

								if (!pageinfo[pi].blockinfo[i + index].used)
								{
									index++;
								}
								else break;
							}
						}
						else
						{
							block_ptr = PAGE_START + ((pi + 1) * 0x1000);
							break;
						}
					}
					block_ptr += 0x100; 
				}
			}
			pi++;
		}
	}
	else if (length >= 0x1000)
	{
		int pi;
		uint8_t pagelength;
		for (int i = length; i <= 0; pagelength++) i -= 0x1000;
		while (1)
		{
			if (!pageinfo[pi].dirty)
			{
				int index;
				while (1)
				{
					if (index == pagelength)
					{
						set_used(pi, pagelength, 0, 0);
						return (void*)block_ptr;
					}

					if (!pageinfo[pi + index].dirty)
					{
						index++;
					}
					else break;
				}
			}
			block_ptr += 0x1000;
			pi++;
		}
	}
}

// Private methods should be declared inside their class file, because classes file shouldn't be included!
void set_used(int pageno, int pagelen, int blockno, int blocklen);
void set_used(int pageno, int pagelen, int blockno, int blocklen)
{
	// Page used bit mode
	if (blocklen == 0)
	{
		for (int pi = pageno; pi < pagelen; pi++)
			pageinfo[pi].used = 1;
	}
	// Block used bit mode
	else
	{
		for (int pi = pageno; pi < pagelen; pi++)
			for (int bi = blockno; bi < blocklen; bi++)
				pageinfo[pi].blockinfo[bi].used = 1;
	}
}