// Copies a BMP file

#include <stdio.h>
#include <stdlib.h> //used for atoi

#include "bmp.h"

//accepts 3 cmd line argument. 1. int n>0, <=100
//2. name of BMP to be resized
//3. name of resized version to be written
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    //size to resize
    int n = atoi(argv[1]);
    float f = atof(argv[1]);

    //check that n is positive integer less than 100
    if (n <= 0 || n > 100 || f != n)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames - point to input and output file locations
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int in_width = bi.biWidth;
    int in_height = bi.biHeight;

    int out_width = in_width * n;
    int out_height = in_height * n;

    //determine padding for scanlines
    int in_padding = (4 - (in_width * sizeof(RGBTRIPLE)) % 4) % 4;
    int out_padding = (4 - (out_width * sizeof(RGBTRIPLE)) % 4) % 4;;

    bi.biWidth = out_width;
    bi.biHeight = out_height;
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + out_padding) * abs(bi.biHeight);
    //total size of pixel, padding, header
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    //for each scanline
    for (int i = 0, in_biHeight = abs(in_height); i < in_biHeight; i++)
    {

        for (int h = 0; h < n; h++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < in_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //for each pixel in row, write n times
                for (int w = 0; w < n; w++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            //add padding to output
            for (int k = 0; k < out_padding; k++)
            {
                //write padding
                fputc(0x00, outptr);
            }
            //go back to start of input scanline
            if (h < n - 1)
            {
                fseek(inptr, -in_width * sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }
        //ignore input padding: move inptr right by size of in_padding
        fseek(inptr, in_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
