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
    /*
        ./ -e [bmp image] [encoded image dest.] [file to encode]
        ./ -d [bmp image] [file name] [file size]
    */

    if(argc < ARG_MIN){
        display_usage();
        exit(EXIT_FAILURE);
    }

    int opt;
    int encode = 0;
    int decode = 0;

    while((opt = getopt(argc, argv, "ed")) != -1){
        switch (opt)
        {
        case 'e':
            encode = 1;
            break;
        
        case 'd':
            decode = 1;
            break;
        default:
            display_usage();
            exit(EXIT_FAILURE);
        }
    }
    
    if(!encode || !decode){
        display_usage();
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
