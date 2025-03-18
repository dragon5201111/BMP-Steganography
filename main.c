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

    FILE * bmp_file;
    FILE * output_file;
    
    output_file = fopen(argv[2], WRITE_FILE);
    if(is_null(output_file)){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    bmp_file = fopen(argv[1], READ_FILE);
    if(is_null(bmp_file)){
        perror(FILE_OPEN_ERR);
        return -1;
    }

    bmp_header bmp_h;
    bmp_info_header bmp_info_h;
    read_bmp_file_into_bmp_header(bmp_file, &bmp_h);
    read_bmp_file_into_bmp_info_header(bmp_file, &bmp_info_h);

    if(!are_valid_bmp_headers(&bmp_h, &bmp_info_h)){
        printf(INVALID_BMP_FILE_ERR);
        close_bmp_and_output_file(bmp_file, output_file);
        return -1;
    }

    jump_to_data_offset_from_start(bmp_file, &bmp_h);

    close_bmp_and_output_file(bmp_file, output_file);
    return 0;
}