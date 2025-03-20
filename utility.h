#pragma once
#define USAGE_STR "Usage: ./main [BMP FILE] [OUTPUT FILE]\n"
#define FILE_OPEN_ERR "Error opening file"
#define INVALID_BMP_FILE_ERR "Invalid file %s. Program requires a 24 bits per pixel bmp file.\n"
#define READ_PIXEL_ARR_ERR "Unable to read bmp file into pixel array"

void print_usage(void);
int is_null(void * obj);