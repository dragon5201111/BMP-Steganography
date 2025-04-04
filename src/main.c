#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"

// TODO:
// Add validation for verifying bmp_image is of correct type
// Add validation for verifying correct amount of bytes were read into bmp_image
// Add validation for verifying correct amount of bytes were written to output_file
// Add validation to ensure input file is sufficiently sized to be encoded
// Add validation for arguments

int main(int argc, char ** argv){
    if(argc < ARG_MIN){
        display_usage();
        exit(EXIT_FAILURE);
    }

    int opt;
    int encode = 0;
    int decode = 0;

    while((opt = getopt(argc, argv, OPT_STR)) != -1){
        switch (opt)
        {
        case ENCODE:
            encode = 1;
            break;
        
        case DECODE:
            decode = 1;
            break;
        default:
            display_usage();
            exit(EXIT_FAILURE);
        }
    }
    
    if(!encode && !decode){
        display_usage();
        exit(EXIT_FAILURE);
    }

    FILE * bmp_file = fopen(argv[2], READ_FILE_BINARY);
    if(is_null(bmp_file)){
        printf(FILE_OPEN_ERR, argv[2]);
        exit(EXIT_FAILURE);
    }

    bmp_image * image = alloc_bmp_image();
    if(is_null(image)){
        printf(BMP_IMAGE_ALLOC_ERR);
        exit(EXIT_FAILURE);
    }


    fclose(bmp_file);
    dealloc_bmp_image(image);
    return EXIT_SUCCESS;
}
