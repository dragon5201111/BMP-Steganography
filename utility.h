#pragma once
#define USAGE_STR "Usage: ./main [BMP FILE] [OUTPUT FILE]\n"
#define FILE_OPEN_ERR "Error opening file"
#define INVALID_BMP_FILE_ERR "Invalid file. Not a 24 bits per pixel bmp file.\n"

void print_usage(void);
int is_null(void * obj);