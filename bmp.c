#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "utility.h"

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

size_t write_bmp_headers_to_file(bmp_header * bmp_header, bmp_info_header * bmp_info_header, FILE * file){
    size_t total_bytes_written = 0;
    total_bytes_written += write_header_to_file(bmp_header, sizeof(*bmp_header), file);
    total_bytes_written += write_header_to_file(bmp_info_header, sizeof(*bmp_info_header), file);
    return total_bytes_written;
}

int are_valid_bmp_headers(bmp_header * bmp_header, bmp_info_header * bmp_info_header){
    return bmp_header->signature == BMP_SIG && bmp_info_header->bits_per_pixel == BMP_BITS_PER_PIXEL;
}

void close_bmp_and_output_file(FILE * bmp_file, FILE * output_file){
    fclose(bmp_file);
    fclose(output_file);
}
int jump_to_pixel_data_from_start(FILE * bmp_file, bmp_header * bmp_header){
    return fseek(bmp_file, bmp_header->data_offset, SEEK_SET);
}

uint32_t get_padding_bytes(uint32_t pixel_array_width){
    return ((4 - (pixel_array_width * BYTES_PER_PIXEL) % 4) % 4);
}


size_t write_file_into_pixel_array(FILE * file, bmp_pixel * pixel_array, int32_t pixel_array_height, uint32_t pixel_array_width){
    uint32_t padding_bytes = get_padding_bytes(pixel_array_width);
    size_t bytes_read = 0;
    size_t total_bytes_read = 0;
    
    for (int y = 0; y < pixel_array_height; y++) {
        bytes_read = fread(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, file);

        if (padding_bytes > 0) {
            fseek(file, padding_bytes, SEEK_CUR);
        }

        total_bytes_read += bytes_read;
    }

    return total_bytes_read;
}

size_t write_pixel_array_to_file(bmp_pixel * pixel_array, int32_t pixel_array_height, uint32_t pixel_array_width, FILE * file){
    uint8_t zero = 0;
    uint32_t padding_bytes = get_padding_bytes(pixel_array_width);
    size_t bytes_written = 0;
    size_t total_bytes_written = 0;

    for (int y = 0; y < pixel_array_height; y++) {
        bytes_written = fwrite(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, file);
        total_bytes_written += bytes_written;

        if (padding_bytes > 0) {
            total_bytes_written += fwrite(&zero, sizeof(uint8_t), padding_bytes, file);
        }
    }

    return total_bytes_written;
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


bmp_image * alloc_bmp_image(void){
    bmp_image * image = (bmp_image *) malloc(sizeof(bmp_image));
    if(image == NULL){
        return NULL;
    }

    image->header = (bmp_header *) malloc(sizeof(bmp_header));
    if(image->header == NULL){
        return NULL;
    }

    image->info_header = (bmp_info_header *) malloc(sizeof(bmp_info_header));
    if(image->info_header == NULL){
        return NULL;
    }

    image->pixel_array = (bmp_pixel_array *) malloc(sizeof(bmp_pixel_array));
    if(image->pixel_array == NULL){
        return NULL;
    }

    image->pixel_array->array = NULL;
    return image;
}

size_t initialize_bmp_image_headers(FILE * file, bmp_image * image){
    size_t bytes_read = 0;
    bytes_read += read_file_into_bmp_header(file, image->header);
    bytes_read += read_file_into_bmp_info_header(file, image->info_header);
    return bytes_read;
}



