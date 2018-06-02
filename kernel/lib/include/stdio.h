
#ifndef $_CSTDIO
#define $_CSTDIO

#define EOF (-1)

typedef struct {
    char *fpos;
    void *base;
    unsigned short handle;
    short flags;
    short unget;
    unsigned long alloc;
    unsigned short buffincrement;
} FILE;

FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *stream);
char *fgets(char *s, int n, FILE *stream);
short fgetc(FILE *stream);
int fgetchar(void);

#endif