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

size_t read_bmp_file_into_struct(FILE * bmp_file, void * s, size_t struct_size){
    return fread(s, 1, struct_size, bmp_file);
}

size_t read_bmp_file_into_bmp_header(FILE * bmp_file, bmp_header * bmp_header){
    return read_bmp_file_into_struct(bmp_file, bmp_header, sizeof(*bmp_header));
}

size_t read_bmp_file_into_bmp_info_header(FILE * bmp_file, bmp_info_header * bmp_info_header){
    return read_bmp_file_into_struct(bmp_file, bmp_info_header, sizeof(*bmp_info_header));
}

size_t write_bmp_headers_to_file(bmp_header * bmp_header, bmp_info_header * bmp_info_header, FILE * file){
    size_t bytes_written = 0;
    size_t total_bytes_written = 0;

    bytes_written = fwrite(bmp_header, sizeof(*bmp_header), 1, file);
    if (bytes_written <= 0){
        return 0;
    }

    total_bytes_written += bytes_written;

    bytes_written = fwrite(bmp_info_header, sizeof(*bmp_info_header), 1, file);
    if (bytes_written <= 0){
        return 0;
    }

    total_bytes_written += bytes_written;
    return total_bytes_written;
}

int are_valid_bmp_headers(bmp_header * bmp_header, bmp_info_header * bmp_info_header){
    return bmp_header->signature == BMP_SIG && bmp_info_header->bits_per_pixel == BMP_BITS_PER_PIXEL;
}

void close_bmp_and_output_file(FILE * bmp_file, FILE * output_file){
    fclose(bmp_file);
    fclose(output_file);
}

int jump_to_data_offset_from_start(FILE * bmp_file, bmp_header * bmp_header){
    return fseek(bmp_file, bmp_header->data_offset, SEEK_SET);
}

size_t write_bmp_file_into_pixel_array(FILE * bmp_file, bmp_pixel * pixel_array, int32_t pixel_array_height, uint32_t pixel_array_width){
    uint32_t padding_bytes = ((4 - (pixel_array_width * 3) % 4) % 4);
    size_t bytes_read = 0;
    size_t total_bytes_read = 0;

    for (int y = 0; y < pixel_array_height; y++) {
        bytes_read = fread(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, bmp_file);

        if (bytes_read <= 0) {
            return bytes_read;
        }

        if (padding_bytes > 0) {
            fseek(bmp_file, padding_bytes, SEEK_CUR);
        }

        total_bytes_read += bytes_read;
    }

    return total_bytes_read;
}

size_t write_pixel_array_to_file(bmp_pixel * pixel_array, int32_t pixel_array_height, uint32_t pixel_array_width, FILE * file){
    uint8_t zero = 0;
    uint32_t padding_bytes = ((4 - (pixel_array_width * 3) % 4) % 4);
    size_t bytes_written = 0;
    size_t total_bytes_written = 0;

    for (int y = 0; y < pixel_array_height; y++) {
        bytes_written = fwrite(&pixel_array[y * pixel_array_width], sizeof(bmp_pixel), pixel_array_width, file);

        if (bytes_written <= 0) {
            return bytes_written;
        }

        total_bytes_written += bytes_written;

        if (padding_bytes > 0) {
            bytes_written = fwrite(&zero, sizeof(uint8_t), padding_bytes, file);

            if (bytes_written <= 0) {
                return bytes_written;
            }

            total_bytes_written += bytes_written;
        }
    }

    return total_bytes_written;
}