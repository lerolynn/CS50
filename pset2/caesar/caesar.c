#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    int k;
    if (argc == 2)
    {
        // Loop through cmd line input and check if any character is not a digit
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (!isdigit(argv[1][i]))
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        k = atoi(argv[1]);
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Get input message from user
    string input = get_string("plaintext: ");
    string output = input;

    // Convert to cipher
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] >= 65 && input[i] <= 90)
        {
            output[i] = (input[i] - 65 + k) % 26 + 65;
        }
        else if (input[i] >= 97 && input[i] <= 122)
        {
            output[i] = (input[i] - 97 + k) % 26 + 97;
        }

    }
    printf("ciphertext: %s\n", output);
    return 0;
}