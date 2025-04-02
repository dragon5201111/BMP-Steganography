#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "bmp.h"
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
      
    bmp_image * bmp_image = alloc_bmp_image();
    if(is_null(bmp_image)){
        perror(BMP_IMAGE_ALLOC_ERR);
        return -1;
    }

    // TODO:
    // Add validation for verifying bmp_image is of correct type
    // Add validation for verifying correct amount of bytes were read into bmp_image
    // Add validation for verifying correct amount of bytes were written to output_file
    read_file_into_bmp_image(bmp_file, bmp_image);
    write_bmp_image_to_file(bmp_image, output_file);

    dealloc_bmp_image(bmp_image);
    close_bmp_and_output_file(bmp_file, output_file);
    return 0;
}