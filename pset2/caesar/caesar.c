#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    char c_cipher;
    string plain;
    
    if (argc != 2)
    {
        printf("Missing command-line argument or has too much arguments\n");
        return 1;
    }
    
    int key = (int) strtol(argv[1], NULL, 10);
    
    if (key > 25)
    {
        key = (key % 26);    
    }
    
    printf("plaintext: ");
    plain = get_string();
    printf("ciphertext: ");

    
    for (int i = 0; i < strlen(plain); i++)
    {
        char c = plain[i];
        if (isalpha(plain[i]))
        {   
            if (c >= 'a' && c <= 'z')
            {
                if ((c + key) > 122)
                {
                    c_cipher = 96 + ((c + key) % 122);
                }
                else
                {
                    c_cipher = c + key;
                }
            } 
            else if (c >= 'A' && c <= 'Z')
            {
                if ((c + key) > 90)
                {
                    c_cipher = 64 + ((c + key) % 90);
                }
                else
                {
                    c_cipher = c + key;
                }
            }
        }
        else
        {
            c_cipher = plain[i];
        }
        printf("%c", c_cipher);

    }
    printf("\n");
    return 0;
}
