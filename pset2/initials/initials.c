#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    if (s != NULL)
    {
        int first;
        int i = 0;
        // Traverse the string
        while (s[i] != '\0')
        {   
            // Look for a whitespace
            if ( s[i] != ' ' && s[i] != '\0')
            {
                first = i;
                char letter = toupper(s[first]);
                printf("%c", letter);
                while (s[i] != ' ' && s[i] != '\0')
                {
                    i++;
                }
            } 
            if ( s[i] == '\0')
            {
                break;
            }
            i++;
        }
        printf("\n");
    }
}