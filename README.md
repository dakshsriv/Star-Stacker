# The CS50 Star Stacker

## Introduction

Almost every amateur astronomer, including me, captures star trails at some point in time. Across a set of star photos captured consecutively, the stars appear to move across the image due to the rotation of the earth. These images can be stacked to produce star trails, which are gorgeous curving arcs of stars moving across the sky. Unfortunately, not many quality software exist on Linux to perform this task. Therefore, due to my passion of astronomy, I coded a command-line star stacker using C.

## Usage

Due to the large size, the full set of sample images can be downloaded from the following box.com link:
https://app.box.com/s/rwqck3fetwx1b0p4zjbk829iwb8ic04k

Once downloaded, enter the proper directory in the terminal and run the command, './stack image-data OUTFILE.bmp'

## File structure

data/
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Images
<br>
data.tar.gz

<br>
helpers.c
<br>
helpers.h
<br>
stack.c
<br>
stack
<br>
README.md
<br>
OUTFILE.bmp

### File structure uses:
*   data/    -- Contains the image data uncompressed
*   data.tar -- Is the compressed data
*   helpers.c -- Contains the helper functions
*   helpers.h -- Is the header file for helpers.c
*   stack.c -- Source code for the stacking process
*   stack -- Compiled program
*   README.md -- This file, explaining my project
*   OUTFILE.bmp -- The output file of the stacking

## Code procedure:
1. Check for proper CLI usage

2. use getFiles function to get a list of all the files in the input directory

3. Once a valid list is returned, the output file is opened
4. The bitmap header for the output file is written
5. Open, write, and close contents of the first image into the output file to do the background

6. Create the image array for the output file

7. For every remaining file:

        1. Open the next file

        2. validate file to see if it is a valid BITMAP 3.0

        3. Create image array for the input image

        4. If yes, for each pixel:
                1. If pixel is not dark or camera noise:
                2. Overwrite pixel into the output image array
8. Write the output array

9. Free all used memory

## Design changes along the way

* Switching from an array of file pointers to an array of filenames<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        - Too many file pointers are wasteful of resources and unconventional
* Switching the RGB array to use dynamic memory allocation instead of static memory allocation<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        - The stack wasn't large enough to hold that much data
* Splitting looking for files and validating them into two parts<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        - It is the convention that OS I/O should be done seperately from File I/O

## Possible improvements

* Gap filling option <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        - Dying batteries, bad images, etc. can produce gaps in the star trails. A gap filling option would be good to fill them
* Denoising option to remove noise from the final image
