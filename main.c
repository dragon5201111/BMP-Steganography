#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "bmp.h"
#include "constants.h"
#include "utility.h"

int main(int argc, char ** argv){
    // argv[1] = bmp file
    // argv[2] = output file
    if(argc != ARG_MIN){
        print_usage();
        return -1;
    }

    FILE * bmp_file = NULL;
    FILE * output_file = NULL;

    bmp_file = fopen(argv[1], READ_FILE_BINARY);
    if(is_null(bmp_file)){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    output_file = fopen(argv[2], WRITE_FILE_BINARY);
    if(is_null(output_file)){
        perror(FILE_OPEN_ERR);
        return -1;
    }


    bmp_header bmp_h = {0};
    bmp_info_header bmp_info_h = {0};
    read_bmp_file_into_bmp_header(bmp_file, &bmp_h);
    read_bmp_file_into_bmp_info_header(bmp_file, &bmp_info_h);

    if(!are_valid_bmp_headers(&bmp_h, &bmp_info_h)){
        printf(INVALID_BMP_FILE_ERR, argv[1]);
        close_bmp_and_output_file(bmp_file, output_file);
        return -1;
    }


    jump_to_data_offset_from_start(bmp_file, &bmp_h);

    int32_t pixel_array_height = abs(bmp_info_h.bitmap_height);
    uint32_t pixel_array_width = bmp_info_h.bitmap_width;
    bmp_pixel * pixel_array = alloc_pixel_array(pixel_array_height, pixel_array_width);
    
    write_bmp_file_into_pixel_array(bmp_file, pixel_array, pixel_array_height, pixel_array_width);  
    write_bmp_headers_to_file(&bmp_h, &bmp_info_h, output_file);
    write_pixel_array_to_file(pixel_array, pixel_array_height, pixel_array_width, output_file);
    
    dealloc_pixel_array(pixel_array);
    close_bmp_and_output_file(bmp_file, output_file);
    return 0;
}