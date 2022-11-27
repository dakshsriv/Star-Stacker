#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include "helpers.h"

int getFiles(char * dir_path, char files[MAXARR][STRSIZE]) {
    DIR * dir = opendir(dir_path);
    if (ENOENT == errno)
    {
        return -1;
    }
    struct dirent * ent;
    int i = 0;
    while((ent = readdir(dir)) != NULL)
    {
        char fname[50];

        strcpy(fname, dir_path);
        strcat(fname, "/");
        strcat(fname, ent->d_name);

        if (ent->d_name[0] == '.')
        {
            continue;
        }

        /*
        FILE * z = fopen(fname, "rb");
        if (z == NULL)
        {
            printf("%d", errno);
        }
        BITMAPFILEHEADER bf;
        int ni = fread(&bf, sizeof(BITMAPFILEHEADER), 1, z);
        if (ni < 1) {
            if (feof(z)) {
                printf("End of file reached\n");
                // do something
            } else {
                printf("Error occurred: %d\n", errno);
                // exit
            }
        }
        // Read infile's BITMAPINFOHEADER
        BITMAPINFOHEADER bi;
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, z);
        if (bf.bfType != 0x4d42 || bf.bOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)     // This contains code from https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader, https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader and filter.c
        {
            fclose(z);
            return -2;
        }
        fclose(z);
        */


        strncpy(files[i], fname, strlen(fname)>STRSIZE?STRSIZE:strlen(fname));
        i++;
    }
    closedir(dir);
    return i;
}

/*
void displayStatus (int number, int total) // How many files and total files
{
    char number_string[50];
    char total_string[50];
    sprintf(number_string, "%d", number);
    sprintf(total_string, "%d", number);
    for(int i = 0; i < (16+strlen(number_string)+strlen(total_string)); i++)
    {
        printf("\b");
    }
    printf("%d/%d images stacked", number, total);
}
*/

int writeBitmapHeader(FILE * file, char * filename) // The layout of the header came from https://lmcnulty.me/words/bmp-output
{
    uint32_t file_size = 54 + 3*WIDTH*HEIGHT;

    BITMAPFILEHEADER fileheader = {.bfType = 0x4d42, .bfSize = file_size, .bfReserved1 = 0, .bfReserved2 = 0, .bOffBits = 54};

    BITMAPINFOHEADER infoheader = {
        .biSize = 40, .biWidth = WIDTH,
        .biHeight = HEIGHT,
        .biPlanes = 1, .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = 0,
        .biXPelsPerMeter = (uint16_t) 800000,
        .biYPelsPerMeter = (uint16_t) 800000,
        .biClrUsed = 0, .biClrImportant = 0};

    size_t x = fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, file);
    if (x != 1)
    {
        printf("Failed to write file header\n");
        return -1;
    }
    size_t y = fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, file);
    if (y != 1)
    {
        printf("Failed to write info header\n");
        return -2;
    }

    // Verification

    fseek(file, 0, SEEK_SET);
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    if (bf.bfType != 0x4d42 || bf.bOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)     // This contains code from filter.c
    {
        printf("Header write error\n");
        return -3;
    }
    return 0;
}

void readRGBArray(RGB outArray[HEIGHT][WIDTH], FILE * file)
{
    for (int i = 0; i < HEIGHT; i++)
    {

        RGB buffer[WIDTH];

        fread(buffer, sizeof(RGB), WIDTH, file);
        for (int j = 0; j < WIDTH; j++)
        {
            outArray[i][j] = buffer[j];
        }
    }
}

void writeRGBArray(RGB inArray[HEIGHT][WIDTH], FILE * file)
{
    for (int i = 0; i < HEIGHT; i++)
    {

        RGB row[WIDTH];
        for (int j = 0; j < WIDTH; j++)
        {
            row[j] = inArray[i][j];
        }

        fwrite(row, sizeof(RGB), WIDTH, file);
    }
}