#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Save a copy of the image
    RGBTRIPLE oldimage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldimage[i][j].rgbtRed = image[i][j].rgbtRed;
            oldimage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            oldimage[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = oldimage[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = oldimage[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = oldimage[i][width - 1 - j].rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Save a copy of the image
    RGBTRIPLE oldimage[height][width];
    // Save a copy of image in a larger array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldimage[i][j].rgbtRed = image[i][j].rgbtRed;
            oldimage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            oldimage[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    // Apply filter
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize temporary variables for pixels
            double red = 0;
            double green = 0;
            double blue = 0;
            int count = 0;

            for (int a = -1; a <= 1; a++)
            {

                for (int b = -1; b <= 1; b++)
                {
                    // Skip pixel if out of image range
                    if (i + a < 0 || i + a >= height || j + b < 0 || j + b >= width)
                    {
                        continue;
                    }

                    red += oldimage[i + a][j + b].rgbtRed;
                    green += oldimage[i + a][j + b].rgbtGreen;
                    blue += oldimage[i + a][j + b].rgbtBlue;
                    count++;
                }
            }

            image[i][j].rgbtRed = round(red / count);
            image[i][j].rgbtGreen = round(green / count);
            image[i][j].rgbtBlue = round(blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Save a copy of the image
    RGBTRIPLE oldimage[height][width];
    // Save a copy of image in a larger array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldimage[i][j].rgbtRed = image[i][j].rgbtRed;
            oldimage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            oldimage[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    // Initialize convolutional matrix
    int gx[3][3] = {{-1,  0,  1}, {-2, 0,  2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, { 0,  0, 0}, { 1, 2, 1}};


    // Apply filter
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize temporary variables to convolute over for gx
            double red_gx, green_gx, blue_gx;
            red_gx = green_gx = blue_gx = 0;

            // Initialize temporary variables to convolute over for gy
            double red_gy, green_gy, blue_gy;
            red_gy = green_gy = blue_gy = 0;

            for (int a = -1; a <= 1; a++)
            {
                for (int b = -1; b <= 1; b++)
                {
                    // Skip pixel if past edge of image
                    if (i + a < 0 || i + a >= height || j + b < 0 || j + b >= width)
                    {
                        continue;
                    }

                    red_gx += oldimage[i + a][j + b].rgbtRed * gx[a + 1][b + 1];
                    green_gx += oldimage[i + a][j + b].rgbtGreen * gx[a + 1][b + 1];
                    blue_gx += oldimage[i + a][j + b].rgbtBlue * gx[a + 1][b + 1];

                    red_gy += oldimage[i + a][j + b].rgbtRed * gy[a + 1][b + 1];
                    green_gy += oldimage[i + a][j + b].rgbtGreen * gy[a + 1][b + 1];
                    blue_gy += oldimage[i + a][j + b].rgbtBlue * gy[a + 1][b + 1];
                }
            }

            double red, green, blue;
            red = round(sqrt(pow(red_gx, 2) + pow(red_gy, 2)));
            green = round(sqrt(pow(green_gx, 2) + pow(green_gy, 2)));
            blue = round(sqrt(pow(blue_gx, 2) + pow(blue_gy, 2)));

            // Check for pixel values past byte range
            if (red > 255)
            {
                red = 255;
            }
            else if (red < 0)
            {
                red = 0;
            }
            if (green > 255)
            {
                green = 255;
            }
            else if (green < 0)
            {
                green = 0;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            else if (blue < 0)
            {
                blue = 0;
            }

            // Store pixel values into pixels
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;

    return;
}
