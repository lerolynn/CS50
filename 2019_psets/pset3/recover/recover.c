#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //true

int main(int argc, char *argv[])
{
    //check that there is only 1 command line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //remember filenames
    char *infile = argv[1];

    //open input file
    FILE *inptr = fopen(infile, "r");
    FILE *outptr = NULL;

    //check that input file exists
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //declare buffer containing 512 bytes
    unsigned char buffer[512];
    int imgCount = 0;
    bool notFileEnd = true;

    //loop through infile, reading file at the same time until end of file reached
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        //if jpeg header is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if already writing to outfile
            if (imgCount > 0)
            {
                //close outfile
                fclose(outptr);
            }
            //open new outfile

            char filename[8];
            sprintf(filename, "%03i.jpg", imgCount);
            outptr = fopen(filename, "w");
            //add to image count
            imgCount++;
        }

        //if first jpeg header is not found yet and block is not a jpeg header
        if (!(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) && imgCount == 0)
        {
            //continue reading next block (don't write to outfile)
            continue;
        }
        //write block into outfile
        fwrite(buffer, 1, 512, outptr);
    }
    fclose(outptr);
    //close infile
    fclose(inptr);
    return 0;
}
