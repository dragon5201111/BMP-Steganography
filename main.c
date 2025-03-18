#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "bmp.h"
#include "constants.h"
#include "utility.h"

#define DEBUG_BMP


int main(int argc, char ** argv){
    // argv[1] = bmp file
    // argv[2] = output file
    if(argc != ARG_MIN){
        print_usage();
        return -1;
    }

    FILE * bmp_file, * output_file;
    output_file = fopen(argv[2], WRITE_FILE);
    if(output_file == NULL){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    bmp_file = fopen(argv[1], READ_FILE);
    if(bmp_file == NULL){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    bmp_header bmp_h;
    bmp_info_header bmp_info_h;
    read_bmp_file_into_bmp_header(bmp_file, &bmp_h);
    read_bmp_file_into_bmp_info_header(bmp_file, &bmp_info_h);
    
    if(!is_valid_bits_per_pixel(bmp_info_h.bits_per_pixel)){
        fprintf(stderr, BMP_BITS_PER_PIXEL_ERR);
        return -1;
    }

    #ifdef DEBUG_BMP
        _print_bmp_header(&bmp_h);
        _print_bmp_info_header(&bmp_info_h);
    #endif

    jump_to_data_offset_from_start(bmp_file, &bmp_h);

    fclose(bmp_file);
    fclose(output_file);
    return 0;
}