#pragma once

#define ARG_MIN 3
#define READ_FILE_BINARY "rb"
#define WRITE_FILE_BINARY "wb"
#define BYTES_PER_PIXEL 3

#define USAGE_STR "Usage: ./main [BMP FILE] [OUTPUT FILE]\n"
#define FILE_OPEN_ERR "Error opening file"
#define INVALID_BMP_FILE_ERR "Invalid file %s. Program requires a 24 bits per pixel bmp file.\n"
#define BMP_IMAGE_ALLOC_ERR "Unable to allocate memory for bmp image.\n"

void print_usage(void);
int is_null(void * obj);