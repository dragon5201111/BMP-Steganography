#include <stdio.h>
#include <errno.h>
#include "bmp.h"
#include "constants.h"
#include "utility.h"

#define DEBUG_BMP

int main(int argc, char ** argv){
    // argv[1] = bmp file
    if(argc != ARG_MIN){
        print_usage();
    }

    FILE * bmp_file = fopen(argv[1], READ_FILE);
    if(bmp_file == NULL){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    /*
        TODO:
        0.) Add check for if is a bmp file
        1.) Implement error checking for reading and jumping to offset
        2.) Read data into pixel array
    */
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

    jump_to_data_offset(bmp_file, &bmp_h);
    
    int pixel_array_width = get_pixel_array_width(&bmp_info_h);
    int pixel_array_height = get_pixel_array_height(&bmp_info_h);
    bmp_pixel ** pixel_array;
   
    fclose(bmp_file);
    return 0;
}