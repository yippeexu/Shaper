
#include <stdint.h>

typedef struct tag_blockinfo blockinfo_t;
struct tag_blockinfo
{
	uint8_t used : 1;
} __attribute__((packed));

typedef struct tag_pageinfo pageinfo_t;
struct tag_pageinfo
{
	uint8_t used : 1;
	uint8_t dirty : 1;
	blockinfo_t blockinfo[16];
} __attribute__((packed));

void heap_init();
void* memalloc(size_t length);