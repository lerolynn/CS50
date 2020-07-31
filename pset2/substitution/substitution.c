#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Check that there is 1 argument given
    if (argc == 2)
    {
        // Check that key contains 26 characters
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            // Check if key contains characters that is not an alphabet
            for (int i = 0; i < 26; i++)
            {
                if (!isalpha(argv[1][i]))
                {
                    printf("Usage: ./substitution key\n");
                    return 1;
                }

                for (int j = 0; j < 26; j++)
                {
                    if (i != j)
                    {
                        if (argv[1][i] == argv[1][j])
                        {
                            printf("Key must not contain same characters.\n");
                            return 1;
                        }
                    }

                }
            }
        }
    }

    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    // Get input message from user
    string input = get_string("plaintext: ");
    string output = input;

    // Convert input to ciphertext output
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] >= 65 && input[i] <= 90)
        {
            output[i] = toupper(key[input[i] - 65]);
        }
        else if (input[i] >= 97 && input[i] <= 122)
        {
            output[i] = tolower(key[input[i] - 97]);
        }
    }

    printf("ciphertext: %s\n", output);
    return 0;
}