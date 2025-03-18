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
    int32_t bitmap_height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m; // m indicates meter(s)
    uint32_t y_pixels_per_m; // m indicates meter(s)
    uint32_t colors_used;
    uint32_t important_colors;
}bmp_info_header;

typedef struct bmp_pixel{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}bmp_pixel;

// Restore structure padding
#pragma pack(pop)

#define BMP_SIG 0x4D42
#define BMP_BITS_PER_PIXEL 24

void _print_bmp_header(bmp_header *);
void _print_bmp_info_header(bmp_info_header *);


size_t read_bmp_file_into_struct(FILE *, void *, size_t);
size_t read_bmp_file_into_bmp_header(FILE *, bmp_header *);
size_t read_bmp_file_into_bmp_info_header(FILE *, bmp_info_header *);
int are_valid_bmp_headers(bmp_header *, bmp_info_header *);

void close_bmp_and_output_file(FILE *, FILE *);

int jump_to_data_offset_from_start(FILE *, bmp_header *);
