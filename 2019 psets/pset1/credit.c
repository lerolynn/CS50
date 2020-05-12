#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    long long number;
    //get input
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0 || number > 9999999999999999);

    //multiply every other digit by 2, starting with second last digit
    //add digits of products together
    //add digits of products together
    //add sum to sum of digits not multiplied by 2
    //if total's digit = 0, card is legit
    long long arr[16];

    //luhns algorithm
    for (int i = 0; i < 16; i++)
    {
        long long x = pow(10, i + 1);
        arr[i] = number % x;
    }

    for (int i = 15; i >= 0; i--)
    {
        long long x = pow(10, i);
        if (i != 0)
        {
            arr[i] = arr[i] - arr[i - 1];
        }
        arr[i] = arr[i] / x;
    }

    int check = 0;
    int tmpDigit = 0;
    for (int i = 1; i < 16; i += 2)
    {
        tmpDigit = arr[i] * 2;
        if (tmpDigit >= 10)
        {
            tmpDigit -= 10;
            check += 1;
        }
        check += tmpDigit;
    }

    for (int i = 0; i < 16; i += 2)
    {
        check += arr[i];
    }

    //printf("%i\n", check);
    //check company of card
    if (check % 10 == 0 && number >= 1000000000000)
    {
        if (arr[15] == 4 ||
            (arr[15] == 0 && arr[14] == 4) ||
            (arr[15] == 0 && arr[14] == 0 && arr[13] == 4) ||
            (arr[15] == 0 && arr[14] == 0 && arr[13] == 0 && arr[12] == 4))
        {
            printf("VISA\n");
        }
        else if (arr[15] == 0 && arr[14] == 3 &&
                 (arr[13] == 4 || arr[13] == 7))
        {
            printf("AMEX\n");
        }
        else if (arr[15] == 5 && (arr[14] >= 1 && arr[14] <= 5))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}
