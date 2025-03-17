#include <stdio.h>
#include <errno.h>
#include "bmp.h"
#include "constants.h"
#include "utility.h"

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

    bmp_header bmp_h;
    bmp_info_header bmp_info_h;
    read_bmp_file_into_bmp_header(bmp_file, &bmp_h);
    read_bmp_file_into_bmp_info_header(bmp_file, &bmp_info_h);
    _print_bmp_header(&bmp_h);
    _print_bmp_info_header(&bmp_info_h);
    fclose(bmp_file);
    return 0;
}