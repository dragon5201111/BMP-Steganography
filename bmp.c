#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "constants.h"

void _print_bmp_header(bmp_header * bmp_header){
    printf("BMP Header Information:\n");
    // Signature is stored in little endian format
    printf("Signature: %c%c\n", (bmp_header->signature & 0xFF), (bmp_header->signature >> 8));
    printf("File Size: %u bytes\n", bmp_header->file_size);
    printf("Reserved: %u\n", bmp_header->reserved);
    printf("Data Offset: %u bytes\n", bmp_header->data_offset);
    putchar('\n');
}

void _print_bmp_info_header(bmp_info_header * bmp_info_header){
    printf("BMP Info Header Information:\n");
    printf("Info Header Size: %u bytes\n", bmp_info_header->size);
    printf("Bitmap Width in Pixels: %u\n", bmp_info_header->bitmap_width);
    printf("Bitmap Height in Pixels: %u\n", bmp_info_header->bitmap_height);
    printf("Number of Planes: %u\n", bmp_info_header->planes);
    printf("Bits Per Pixel: %u\n", bmp_info_header->bits_per_pixel);
    printf("Compression: %u\n", bmp_info_header->compression);
    printf("Image Size: %u bytes\n", bmp_info_header->image_size);
    printf("X Pixels Per Meter: %u\n", bmp_info_header->x_pixels_per_m);
    printf("Y Pixels Per Meter: %u\n", bmp_info_header->y_pixels_per_m);
    printf("Colors Used: %u\n", bmp_info_header->colors_used);
    printf("Important Colors: %u\n", bmp_info_header->important_colors);
    putchar('\n');
}

int is_valid_bits_per_pixel(int bits_per_pixel){
    return bits_per_pixel == VALID_BITS_PER_PIXEL;
}

int read_bmp_file_into_struct(FILE * bmp_file, void * s, size_t struct_size){
    return fread(s, 1, struct_size, bmp_file);
}

int read_bmp_file_into_bmp_header(FILE * bmp_file, bmp_header * bmp_header){
    return read_bmp_file_into_struct(bmp_file, bmp_header, sizeof(*bmp_header));
}

int read_bmp_file_into_bmp_info_header(FILE * bmp_file, bmp_info_header * bmp_info_header){
    return read_bmp_file_into_struct(bmp_file, bmp_info_header, sizeof(*bmp_info_header));
}

int jump_to_data_offset(FILE * bmp_file, bmp_header * bmp_header){
    return fseek(bmp_file, bmp_header->data_offset, SEEK_SET);
}

int get_pixel_array_width_padding(bmp_info_header * bmp_info_header){
    return (4 - (bmp_info_header->bitmap_width * 3) % 4) % 4;
}

int get_pixel_array_width(bmp_info_header * bmp_info_header){
    return bmp_info_header->bitmap_width * PIXEL_NUM_BYTES + get_pixel_array_width_padding(bmp_info_header);
}

int get_pixel_array_height(bmp_info_header * bmp_info_header){
    return bmp_info_header->bitmap_height;
}
