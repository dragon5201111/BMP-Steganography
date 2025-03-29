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

typedef struct bmp_pixel_array{
    bmp_pixel * array;
    uint32_t padding_bytes;
}bmp_pixel_array;

typedef struct bmp_image{
    bmp_header * header;
    bmp_info_header * info_header;
    bmp_pixel_array * pixel_array;
}bmp_image;


// Restore structure padding
#pragma pack(pop)

#define BMP_SIG 0x4D42
#define BMP_BITS_PER_PIXEL 24

void _print_bmp_header(bmp_header *);
void _print_bmp_info_header(bmp_info_header *);

size_t read_file_into_struct(FILE *, void *, size_t);
size_t read_file_into_bmp_header(FILE *, bmp_header *);
size_t read_file_into_bmp_info_header(FILE *, bmp_info_header *);
size_t write_header_to_file(void *, size_t, FILE *);
size_t write_bmp_headers_to_file(bmp_header *, bmp_info_header *, FILE *);
int are_valid_bmp_headers(bmp_header *, bmp_info_header *);

void close_bmp_and_output_file(FILE *, FILE *);
int jump_to_pixel_data_from_start(FILE *, bmp_header *);

size_t write_file_into_pixel_array(FILE *, bmp_pixel *, int32_t, uint32_t);
size_t write_pixel_array_to_file(bmp_pixel *, int32_t, uint32_t, FILE *);

bmp_pixel * alloc_pixel_array(int32_t, uint32_t);
void dealloc_pixel_array(bmp_pixel *);

bmp_image * alloc_bmp_image(void);
size_t initialize_bmp_image_headers(FILE *, bmp_image *);
