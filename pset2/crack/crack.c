#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <crypt.h>
#define _XOPEN_SOURCE

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Missing command-line argument\n");
        return 1;
    } else if (argc > 2) 
    {
        printf("Too much command-line arguments\n");
        return 1;
    }
    
    string hash = argv[1];
    string result;
    string chars = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char * salt;
    char * pass;
    
    salt = malloc(2);
    pass = malloc(4);
    salt[0] = hash[0];
    salt[1] = hash[1];
    // printf("%s\n", salt);

    for ( int c1 = 12; c1 < 64; c1++)
    {
        for ( int c2 = 12; c2 < 64; c2++)
        {
            for ( int c3 = 12; c3 < 64; c3++)
            {
                for ( int c4 = 12; c4 < 64; c4++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (i == 0){
                            pass[0] = chars[c1];
                        } else if (i == 1){
                            pass[0] = chars[c1];
                            pass[1] = chars[c2];
                        } else if (i == 2){
                            pass[0] = chars[c1];
                            pass[1] = chars[c2];
                            pass[2] = chars[c3];
                        } else if (i == 3){
                            pass[0] = chars[c1];
                            pass[1] = chars[c2];
                            pass[2] = chars[c3];
                            pass[3] = chars[c4];
                        }
                        result = crypt(pass, salt);
                        if (strcmp(hash, result) == 0)
                        {
                            printf("%s\n", pass);
                            return 0;
                        }
                        memset(&pass[0], 0, sizeof(pass));
                    }
                }
            }    
        }
    }
    printf("Not Cracked!");
    return 0;
}