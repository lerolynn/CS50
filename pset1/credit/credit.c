#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    // Prompt the user for card number
    long card;
    do
    {
        card = get_long("Number: ");
    }
    while (card < 0);

    long arr[16] = {0};
    int numLength = 0;

    // Place numbers into array
    for (int i = 0; i < 16; i++)
    {

        long x = pow(10, i + 1);
        arr[i] = (card % x) / pow(10, i);
        card -= arr[i];
        numLength++;

        if (card < pow(10, i + 1))
        {
            break;
        }
    }

    // Calculate checksum
    int prod[8] = {0};
    for (int i = 1; i < 16; i += 2)
    {
        prod[(i - 1) / 2] = arr[i] * 2;
    }

    // Multiply every 2nd digit and add sum of products' digits
    int checksum = 0;
    int tempDigit = 0;
    for (int i = 0; i < 8; i++)
    {
        if (prod[i] > 0 && prod[i] < 10)
        {
            checksum += prod[i];
        }
        else if (prod[i] >= 10)
        {
            checksum += prod[i] % 10;
            checksum += floor(prod[i] / 10);
        }
    }

    // Add remaining digits to checksum
    for (int i = 0; i < 16; i += 2)
    {
        checksum += arr[i];
    }

    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // AMEX - 15 digits, start with 34, 37
    if ((card >= pow(10, 14) && card < pow(10, 15)) &&
        (arr[14] == 3 && (arr[13] == 4 || arr[13] == 7)))
    {
        printf("AMEX\n");

    }


    // MASTERCARD - 16 digits, start with 51-55
    else if ((card >= pow(10, 15) && card < pow(10, 16)) &&
             (arr[15] == 5 && arr[14] >= 1 && arr[14] <= 5))
    {
        printf("MASTERCARD\n");
    }

    // VISA - 13, 16 digits , start with 4
    else if ((card >= pow(10, 12) && card < pow(10, 13) && arr[12] == 4) ||
             (card >= pow(10, 15) && card < pow(10, 16) && arr[15] == 4))
    {
        printf("VISA\n");
    }

    else
    {
        printf("INVALID\n");
    }
}