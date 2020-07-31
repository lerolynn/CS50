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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = (int) round((0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue));
            int green = (int) round((0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue));
            int blue = (int) round((0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue));

            // Check corner cases for red
            if (red > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else if (red < 0)
            {
                image[i][j].rgbtRed = 0;
            }
            else
            {
                image[i][j].rgbtRed = (BYTE) red;
            }

            // Check corner cases for green
            if (green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else if (green < 0)
            {
                image[i][j].rgbtGreen = 0;
            }
            else
            {
                image[i][j].rgbtGreen = (BYTE) green;
            }

            // Check corner cases for blue
            if (blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else if (blue < 0)
            {
                image[i][j].rgbtBlue = 0;
            }
            else
            {
                image[i][j].rgbtBlue = (BYTE) blue;
            }
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
