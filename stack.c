#include "helpers.h"

int main(int argc, char *argv[])
{
    // Prepare input files

    char infiles[MAXARR][STRSIZE];
    char * dir_path = "data";
    int count = 0;

    if (argc != 3)
    {
        printf("Usage: ./stack DIRECTORY TARGET\n");
        return 1;
    }
    //printf("%s", typeof(infiles));
    int file_count = getFiles(argv[1], infiles);

    if (file_count == -1)
    {
        printf("Directory does not exist\n");
        return 1;
    }
    else if (file_count == 0)
    {
        printf("Directory cannot be empty\n");
        return 2;
    }

    printf("file count: %i\n", file_count);
    //printf("0/0 images stacked");

    // Prepare output file

    FILE * output_file = fopen(argv[2], "wb+");
    if (output_file == NULL)
    {
        printf("Could not open output file\n");
        return 3;
    }

    int header_status = writeBitmapHeader(output_file, "OUTFILE.bmp");
    if (header_status == -1)
    {
        return 4;
    }
    else if (header_status == -2)
    {
        return 5;
    }
    else if (header_status == -3)
    {
        return 6;
    }


    // Copy in first file


    FILE *first_file = fopen(infiles[0], "rb");

    RGB (* output_array) [WIDTH] = calloc(HEIGHT, WIDTH * sizeof(RGB));
    RGB (* input_array) [WIDTH] = calloc(HEIGHT, WIDTH * sizeof(RGB));
    fseek(first_file, 54, SEEK_SET);
    readRGBArray(output_array, first_file);
    fclose(first_file);


    for (int i = 1; i < file_count; i++)
    {
        fseek(output_file, 54, SEEK_SET);
        FILE *input_file = fopen(infiles[i], "rb");
        if (input_file == NULL)
        {
            printf("Could not open data file %s\n", infiles[i]);
            free(output_array);
            return 7;
        }
        // Check if file is Bitmap

        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;
        fread(&bf, sizeof(BITMAPFILEHEADER), 1, input_file);
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, input_file);

        if (bf.bfType != 0x4d42 || bf.bOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)
        {
            continue;
        }

        // Stack image

        readRGBArray(input_array, input_file);
        fclose(input_file);


        for (int row = 0; row < HEIGHT; row++)
        {
            for (int column = 0; column < WIDTH; column++)
            {
                RGB pixel = input_array[row][column];
                if (pixel.Red + pixel.Green + pixel.Blue < 180)
                {
                    continue; // Dark pixel
                }
                if (pixel.Red > pixel.Green + pixel.Blue)
                {
                    continue; // Red-colored camera noise
                }
                if (pixel.Green > pixel.Red + pixel.Blue)
                {
                    continue; // Green-colored camera noise
                }
                if (pixel.Blue > pixel.Red + pixel.Green)
                {
                    continue; // Blue-colored camera noise
                }
                output_array[row][column] = input_array[row][column]; // Valid data
            }
        }
        bzero(input_array, HEIGHT*WIDTH*sizeof(RGB));
        count++;
        //displayStatus(count, file_count);
    }

    fseek(output_file, 54, SEEK_SET);
    writeRGBArray(output_array, output_file); // Write final array
    free(input_array);
    free(output_array);
    fclose(output_file);
    printf("All files successfully stacked\n");
}