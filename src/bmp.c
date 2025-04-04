#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

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

size_t read_file_into_struct(FILE * file, void * s, size_t struct_size){
    return fread(s, 1, struct_size, file);
}

size_t read_file_into_bmp_header(FILE * file, bmp_header * bmp_header){
    return read_file_into_struct(file, bmp_header, sizeof(*bmp_header));
}

size_t read_file_into_bmp_info_header(FILE * file, bmp_info_header * bmp_info_header){
    return read_file_into_struct(file, bmp_info_header, sizeof(*bmp_info_header));
}

size_t write_header_to_file(void *header, size_t header_size, FILE *file) {
    return fwrite(header, header_size, 1, file);
}

int are_valid_bmp_headers(bmp_header * bmp_header, bmp_info_header * bmp_info_header){
    return bmp_header->signature == BMP_SIG && bmp_info_header->bits_per_pixel == BMP_BITS_PER_PIXEL;
}

int jump_to_pixel_data_from_start(FILE * bmp_file, bmp_header * bmp_header){
    return fseek(bmp_file, bmp_header->data_offset, SEEK_SET);
}

uint32_t get_padding_bytes(uint32_t pixel_array_width){
    return ((4 - (pixel_array_width * BYTES_PER_PIXEL) % 4) % 4);
}



bmp_pixel * alloc_pixel_array(int32_t pixel_array_height, uint32_t pixel_array_width){
    return (bmp_pixel *) malloc(sizeof(bmp_pixel) * pixel_array_height * pixel_array_width);
}

void dealloc_pixel_array(bmp_pixel * pixel_array){
    if(is_null(pixel_array)){
        return;
    }

    free(pixel_array);
}

bmp_image * alloc_bmp_image(){
    bmp_image * image = (bmp_image *) malloc(sizeof(bmp_image));
    if(is_null(image)){
        return NULL;
    }

    image->pixel_array = NULL;
    return image;
}


void dealloc_bmp_image(bmp_image * image){
    if(is_null(image)){
        return;
    }
    
    dealloc_pixel_array(image->pixel_array);
    free(image);
}

void initialize_bmp_image_pixel_array(bmp_image * image){
    int32_t pixel_array_height = abs(image->info_header.bitmap_height);
    uint32_t pixel_array_width = image->info_header.bitmap_width;

    image->pixel_array_height = pixel_array_height;
    image->pixel_array_width = pixel_array_width;
    image->padding_bytes = get_padding_bytes(pixel_array_width);
    image->pixel_array = alloc_pixel_array(pixel_array_height, pixel_array_width);
}

size_t read_file_into_bmp_image(FILE * file, bmp_image * image){
    size_t total_bytes_read = 0;

    total_bytes_read += read_file_into_bmp_header(file, &image->header);
    total_bytes_read += read_file_into_bmp_info_header(file, &image->info_header);

    jump_to_pixel_data_from_start(file, &image->header);
    initialize_bmp_image_pixel_array(image);

    total_bytes_read += read_file_into_bmp_image_pixel_array(file, image);

    return total_bytes_read;
}

size_t read_file_into_bmp_image_pixel_array(FILE *file, bmp_image *image) {
    size_t total_bytes_read = 0;
    uint32_t padding_bytes = image->padding_bytes;
    int32_t pixel_array_height = image->pixel_array_height;
    uint32_t pixel_array_width = image->pixel_array_width;
    bmp_pixel *pixel_array = image->pixel_array;

    for (int y = 0; y < pixel_array_height; y++) {
        size_t bytes_read = fread(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, file);
        total_bytes_read += bytes_read;

        if (padding_bytes > 0) {
            fseek(file, padding_bytes, SEEK_CUR);
        }
    }

    return total_bytes_read;
}

size_t write_bmp_image_pixel_array_to_file(bmp_image * image, FILE * file){
    uint8_t zero = 0;
    uint32_t padding_bytes = image->padding_bytes;
    int32_t pixel_array_height = image->pixel_array_height;
    uint32_t pixel_array_width = image->pixel_array_width;
    bmp_pixel *pixel_array = image->pixel_array;

    size_t total_bytes_written = 0;

    for (int y = 0; y < pixel_array_height; y++) {
        size_t bytes_written = 0;
        bytes_written = fwrite(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, file);
        total_bytes_written += bytes_written;

        if (padding_bytes > 0) {
            total_bytes_written += fwrite(&zero, sizeof(uint8_t), padding_bytes, file);
        }
    }

    return total_bytes_written;
}

size_t write_bmp_image_headers_to_file(bmp_image * image, FILE * file){
    size_t total_bytes_written = 0;
    total_bytes_written += write_header_to_file(&image->header, sizeof(image->header), file);
    total_bytes_written += write_header_to_file(&image->info_header, sizeof(image->info_header), file);
    return total_bytes_written;
}


size_t write_bmp_image_to_file(bmp_image * image, FILE * file){
    size_t total_bytes_written = 0;
    total_bytes_written += write_bmp_image_headers_to_file(image, file);
    total_bytes_written += write_bmp_image_pixel_array_to_file(image, file);
    return total_bytes_written;
}

size_t encode_file_into_bmp_image(FILE *file, bmp_image * image) {
    bmp_pixel * pixel_array = image->pixel_array;
    size_t bytes_encoded = 0;
    int pixel_index = 0;
    int counter = 0;  // 1: blue, 2: green, 0: red
    
    uint8_t byte_buffer[BYTE_READ_SIZE];
    size_t bytes_read = 0;
    uint8_t bit;
    bmp_pixel * current_pixel;

    while ((bytes_read = fread(byte_buffer, sizeof(uint8_t), BYTE_READ_SIZE, file)) > 0) {
        bytes_encoded += bytes_read;

        for (size_t byte_index = 0; byte_index < bytes_read; byte_index++) {
            uint8_t byte = byte_buffer[byte_index];

            for (int i = 7; i >= 0; i--) {
                bit = (byte >> i) & 1;
                current_pixel = &pixel_array[pixel_index];
                counter = (counter + 1) % 3;

                if (counter == 1) {
                    current_pixel->blue |= bit;
                } else if (counter == 2) {
                    current_pixel->green |= bit;
                } else {
                    current_pixel->red |= bit; 
                    pixel_index++;  
                }
            }
        }
    }

    return bytes_encoded;
}

int is_null(void * obj){
    return obj == NULL;
}

void display_usage(void){
    printf(USAGE_STR);
}