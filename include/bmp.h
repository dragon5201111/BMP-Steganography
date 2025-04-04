#pragma once

#include <stdint.h>


#define ARG_MIN 4

#define READ_FILE_BINARY "rb"
#define WRITE_FILE_BINARY "wb"

#define BMP_SIG 0x4D42
#define BMP_BITS_PER_PIXEL 24
#define BYTES_PER_PIXEL 3
#define BYTE_READ_SIZE 4096

#define ENCODE 'e'
#define DECODE 'd'
#define OPT_STR "ed"
#define USAGE_STR "Malformed program flag or arguments.\nCorrect Usage:\n\t./bmpSteg -e [bmp file] [file to encode]\n\t./bmpSteg -d [bmp file] [decoded file name] [encoded file size]\n"
#define FILE_OPEN_ERR "Error opening file named: %s.\n"
#define INVALID_BMP_FILE_ERR "Invalid file named: %s. Program requires a 24 bits per pixel bmp file.\n"
#define BMP_IMAGE_ALLOC_ERR "Unable to allocate memory for bmp image.\n"


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

typedef struct bmp_image {
    bmp_header header;
    bmp_info_header info_header;
    bmp_pixel * pixel_array;
    int32_t pixel_array_height;
    uint32_t pixel_array_width;
    uint32_t padding_bytes;
} bmp_image;

// Restore structure padding
#pragma pack(pop)

void _print_bmp_header(bmp_header *);
void _print_bmp_info_header(bmp_info_header *);

size_t read_file_into_struct(FILE *, void *, size_t);
size_t read_file_into_bmp_header(FILE *, bmp_header *);
size_t read_file_into_bmp_info_header(FILE *, bmp_info_header *);
size_t write_header_to_file(void *, size_t, FILE *);
size_t write_bmp_headers_to_file(bmp_header *, bmp_info_header *, FILE *);
int are_valid_bmp_headers(bmp_header *, bmp_info_header *);

int jump_to_pixel_data_from_start(FILE *, bmp_header *);

bmp_pixel * alloc_pixel_array(int32_t, uint32_t);
void dealloc_pixel_array(bmp_pixel *);

bmp_image * alloc_bmp_image();
void dealloc_bmp_image(bmp_image *);
size_t read_file_into_bmp_image(FILE *, bmp_image *);
size_t read_file_into_bmp_image_pixel_array(FILE *, bmp_image *);
size_t encode_file_into_bmp_image(FILE *, bmp_image *);
size_t write_bmp_image_headers_to_file(bmp_image *, FILE *);
size_t write_bmp_image_pixel_array_to_file(bmp_image *, FILE *);
size_t write_bmp_image_to_file(bmp_image *, FILE *);

int is_null(void * obj);
void display_usage(void);