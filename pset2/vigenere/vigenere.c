#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    char c_cipher;
    string plain;
    int j = 0;
    
    if (argc != 2)
    {
        printf("Missing command-line argument or has too much arguments\n");
        return 1;
    }
    
    string key = argv[1];
    for (int n = 0; n < strlen(key); n++)
    {
        if (!isalpha(key[n]))
        {
            printf("Command-line argument can only contain alphabetical characters\n");
            return 1;            
        }
        key[n] = tolower( (int) key[n]);
        
    }
    
    printf("plaintext: ");
    plain = get_string();
    printf("ciphertext: ");
    
    
    for (int i = 0; i < strlen(plain); i++)
    {
        int k = (int) key[j] - 97;
        char c = plain[i];
        if (isalnum(plain[i]))
        {   
            if (c >= 'a' && c <= 'z')
            {
                if ((c + k) > 122)
                {
                    c_cipher = 96 + ((c + k) % 122);
                }
                else
                {
                    c_cipher = c + k;
                }
            } 
            else if (c >= 'A' && c <= 'Z')
            {
                if ((c + k) > 90)
                {
                    c_cipher = 64 + ((c + k) % 90);
                }
                else
                {
                    c_cipher = c + k;
                }
            
            }
            
            if (j < strlen(key)-1)
            {
                j++;    
            } 
            else
            {
                j = 0;    
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
