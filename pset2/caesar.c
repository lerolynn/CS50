#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    //check for command line argument count
    if (argc == 2)
    {
        int count = 0;
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isdigit(argv[1][i]) != 0)
            {
                count++;
            }
        }

        if (count == strlen(argv[1]))
        {
            char *input = get_string("plaintext: ");
            char *output = input;
            for (int i = 0; i < strlen(input); i++)
            {
                //for uppercase
                if (input[i] >= 65 && input[i] <= 90)
                {
                    output[i] = (input[i] - 65 + atoi(argv[1])) % 26 + 65;
                }
                //for lowercase
                else if (input[i] >= 97 && input[i] <= 122)
                {
                    output[i] = (input[i] - 97 + atoi(argv[1])) % 26 + 97;
                }
                //if user inputs number or symbols
                else
                {
                    output[i] = input[i];
                }
            }
            printf("ciphertext: %s\n", output);
        }
        //error if argument is numbers
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    //error if more than 1 or no arguments
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    return 0;
}
