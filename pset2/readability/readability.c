#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");

    // Count number of letters
    int letters = 0;
    int words = 0;
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }

        if (isspace(text[i]) || i == strlen(text) - 1)
        {
            words++;
        }

        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    float L, S;

    L = (float) letters / words * 100;
    S = (float) sentences / words * 100;

    int grade;
    grade = round(0.0588 * L - 0.296 * S - 15.8);

    if (grade > 16)
    {
        printf("Grade 16+\n");
    }

    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }

    else
    {
        printf("Grade %i\n", grade);
    }

    return 0;
}