
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <image.h>

image_t create_image(imagetype_t image_type, uint8_t image_bpp, FILE *file_stream)
{
    image_t image;
    image.image_type = image_type;
    image.image_bpp = image_bpp;
    image.image_file = file_stream;
    return image;
}