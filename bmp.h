#pragma once

#include <stdint.h>
// Omit structure padding enforced by compilier
#pragma pack(push, 1)

typedef struct bmp_header{
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
}bmp_header;

typedef struct bmp_info_header{
    uint32_t size;
    uint32_t bitmap_width;
    uint32_t bitmap_height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m; // m indicates meter(s)
    uint32_t y_pixels_per_m; // m indicates meter(s)
    uint32_t colors_used;
    uint32_t important_colors;
}bmp_info_header;

// Restore structure padding
#pragma pack(pop)

void _print_bmp_header(bmp_header *);
void _print_bmp_info_header(bmp_info_header *);

int read_bmp_file_into_struct(FILE *, void *, size_t);
int read_bmp_file_into_bmp_header(FILE *, bmp_header *);
int read_bmp_file_into_bmp_info_header(FILE *, bmp_info_header *);
