#include <cs50.h>
#include <stdio.h>

//Builds mario pyramid
int main(void)
{
    int x;
    //check that x is between 1 and 8
    do
    {
        x = get_int("Height: ");
    }
    while (x < 1 || x > 8);

    //generate rows
    for (int i = 0; i < x; i++)
    {
        //generate columns
        for (int j = x - 1; j > i; j--)
        {
            printf(" ");
        }
        //first tower
        for (int j = 0; j <= i; j++)
        {

            printf("#");
        }

        printf("  ");

        //second tower
        for (int j = 0; j <= i; j++)
        {

            printf("#");
        }

        printf("\n");
    }
}
