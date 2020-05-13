#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    //check only 1 command line argument input
    if (argc == 2)
    {
        //argv[1] is the hashed password
        //assume each password is 5 or less characters
        //first 2 characters of hash is salt
        char letters[53] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                            '\0'
                           };
        const char *hashed = argv[1];
        char salt[3] = {hashed[0], hashed[1], '\0'};
        //initialize pass to end of string
        char pass[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};
        //loop through letters for each character in password
        for (int i = 0; i < sizeof(letters); i++)
        {
            for (int j = 0; j < sizeof(letters); j++)
            {
                for (int k = 0; k < sizeof(letters); k++)
                {
                    for (int l = 0; l < sizeof(letters); l++)
                    {
                        for (int m = 0; m < sizeof(letters); m++)
                        {
                            pass[0] = letters[i];
                            pass[1] = letters[j];
                            pass[2] = letters[k];
                            pass[3] = letters[l];
                            pass[4] = letters[m];

                            //check if crypt = hash
                            if (strcmp(crypt(pass, salt), argv[1]) == 0)
                            {
                                printf("%s\n", pass);
                                break;
                            }
                        }
                    }
                }
            }
        }
        return 0;
    }
    else
    {
        printf("Usage ./crack hash\n");
        return 1;
    }
}
