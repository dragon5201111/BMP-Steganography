#pragma once
#define USAGE_STR "Usage: ./main [BMP FILE] [OUTPUT FILE]\n"
#define BMP_BITS_PER_PIXEL_ERR "Invalid bits per pixel for bmp file. Must be 24-bits\n"
#define FILE_OPEN_ERR "Error opening file"
#define PIXEL_ARRAY_ALLOC_ERR "Error allocating for pixel array"

void print_usage(void);