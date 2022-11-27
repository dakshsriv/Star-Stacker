#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <math.h>

#define STRSIZE 50
#define MAXARR  500

#define HEIGHT 2272 // Predetermined 2272
#define WIDTH 4032  // Predetermined 4032


typedef struct
{
    uint8_t  Blue;
    uint8_t  Green;
    uint8_t  Red;
} __attribute__((__packed__))
RGB; // From filter

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER
{
 uint16_t bfType;  //specifies the file type "BM" 0x424d
 uint32_t bfSize;  //specifies the size in bytes of the bitmap file
 uint16_t bfReserved1;  //reserved; must be 0
 uint16_t bfReserved2;  //reserved; must be 0
 uint32_t bOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop) // From stackoverflow

typedef struct tagBITMAPINFOHEADER {
  uint32_t biSize;
  uint32_t  biWidth;
  uint32_t  biHeight;
  uint16_t  biPlanes;
  uint16_t  biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint16_t  biXPelsPerMeter;
  uint16_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

int getFiles(char * dir_path, char files[MAXARR][STRSIZE]);
int writeBitmapHeader(FILE * file, char * filename);
void readRGBArray(RGB outArray[HEIGHT][WIDTH], FILE * file);
void writeRGBArray(RGB inArray[HEIGHT][WIDTH], FILE * file);
//void displayStatus (int number, int total);