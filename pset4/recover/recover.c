#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // Open file
    FILE *inptr = fopen(argv[1], "r");
    FILE *outptr = NULL;
    if (inptr == NULL)
    {
        return 2;
    }

    int imgCount = 0;
    unsigned char buffer[512];
    // Read from memory card, keep reading until end of file
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        // // Check for new jpeg header  - 0xff, 0xd8, 0xff, 0xe0 - 0xef
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (imgCount > 0)
            {
                fclose(outptr);
            }

            // Make a new jpeg file
            char filename[8];
            sprintf(filename, "%03i.jpg", imgCount);
            // Open new file to write
            outptr = fopen(filename, "w");
            imgCount++;
        }

        // Keep reading
        if (imgCount == 0 && !(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0))
        {
            continue;
        }

        fwrite(buffer, 1, 512, outptr);
    }

    // Close files
    fclose(outptr);
    fclose(inptr);
    return 0;
}
