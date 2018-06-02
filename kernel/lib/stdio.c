
#include <sysprint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <shapervfs.h>
#include <ps2_keyboard.h>
#include <heap.h>

FILE *fopen(const char *filename, const char *mode)
{
    FILE *f = memalloc(sizeof(FILE));
    f->flags |= (strchr(mode, 'r') != 0) << 0;
    f->flags |= (strchr(mode, 'w') != 0) << 1;
    vfs_entry_t *file_entry = memalloc(sizeof(vfs_entry_t));
    f->base = file_open(filename, file_entry);
    f->buffincrement = file_entry->size;
    f->fpos = f->base;
    if (!f->base)
    {
        sprintstr(0, "stdio: File not found\n");
        // if ((f->flags & 1 << 1) || strchr(mode, 'a'))
        // {        
        //     f->base = file_create(filename);
        //     if (!f->base)
        //         return NULL;
        //     else
        //     {
        
        //     }
        // }
        return NULL;
    }
    
    f->alloc = f->buffincrement;
    sprintstr(0, "stdio: Found ");
    sprintstr(0, file_entry->name);
    sprintstr(0, ", base = ");
    sprinthex(0, f->base, 0);
    sprintchr(0, '\n');
    sprintstr(0, "size = ");
    sprintnum(0, file_entry->size);
    sprintchr(0, '\n');
    return f;
}

int fclose(FILE *stream)
{
    memclr(stream, sizeof(stream));
    return 0;
}

char *fgets(char *s, int n, FILE *stream)
{
    for (int i = 0; i < n;)
    {
        if (i == stream->alloc)
        {
            sprintstr(0, "stdio: EOF\n");
            stream->flags |= 1 << 5;
            return NULL;
        }
        else if (stream->fpos[i] == '\n')
        {
            sprintstr(0, "stdio: EOL\n");
            return s;
        }
        s[i] = *stream->fpos++;
        s[++i] = 0;
    }
    return s;
}

short fgetc(FILE *stream)
{
    if (((uint32_t)stream->fpos-(uint32_t)stream->base) == stream->alloc)
    {
        sprintstr(0, "stdio: EOF\n");
        stream->flags |= 1 << 5;
        return EOF;
    }
    return *stream->fpos++;
}

int fgetchar(void)
{
    return keyboard_get_char();
}