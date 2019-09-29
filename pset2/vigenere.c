#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int shift(char c);

int main(int argc, string argv[])
{
    //check if argument count is 1
    if (argc == 2)
    {
        int count = 0;
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            //check if every input is alpha
            if (isalpha(argv[1][i]) != 0)
            {
                count++;
            }
        }
        //check if every element in argument is alphabet
        if (count == strlen(argv[1]))
        {
            //int key = shift(argv[1][0]);
            //printf("%i\n", key);
            string input = get_string("plaintext: ");
            string output = input;
            //keyword counter
            int keycount = 0;
            for (int i = 0; i < strlen(input); i++)
            {
                //check that every input is alpha
                if (isalpha(input[i]) != 0)
                {
                    //check for keyword looparounds
                    if (keycount == strlen(argv[1]))
                    {
                        keycount = 0;
                    }
                    //check for loop arounds
                    if (input[i] >= 65 && input[i] <= 90)
                    {
                        output[i] = (input[i] + shift(argv[1][keycount]) - 65) % 26 + 65;
                    }
                    else if (input[i] >= 97 && input[i] <= 122)
                    {
                        output[i] = (input[i] + shift(argv[1][keycount]) - 97) % 26 + 97;
                    }
                    keycount++;
                }
                else
                {
                    output[i] = input[i];
                }
            }
            printf("ciphertext: %s\n", output);
            return 0;
        }
        else
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
}

int shift(char c)
{
    //Convert character to positional integer value
    int x = 0;
    if (c >= 65 && c <= 90)
    {
        x = c - 65;
    }
    else if (c >= 97 && c <= 122)
    {
        x = c - 97;
    }
    return x;
}
