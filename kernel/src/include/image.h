
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifndef $_IMAGE_H
#define $_IMAGE_H

typedef enum tag_imagetype
{
    IMAGETYPE_BMP,
    IMAGETYPE_PNG,
    IMAGETYPE_GIF,
    IMAGETYPE_JPEG
} imagetype_t;

typedef struct tag_image
{
    imagetype_t image_type;
    uint8_t image_bpp;
    FILE *image_file;
} image_t;

image_t create_image(imagetype_t image_type, uint8_t image_bpp, FILE *file_data);

#endif