#include <stdio.h>
#include <cs50.h>
#include <string.h>

int first_check(long long ccn);
void sec_check(long long ccn);
int sec_to_last(long long ccn);
int remaining(long long ccn);


int main(void)
{
    printf("Number:");
    long long number = get_long_long();
    if (first_check(number) % 10 != 0){
        printf("INVALID\n");
        return 0;
    }
    sec_check(number);
    return 0;
}

int first_check(long long ccn)
{
    int sum = 0;
    sum += sec_to_last(ccn);
    sum += remaining(ccn);
    // printf("sum: %i\n", sum);
    return sum;
}

int sec_to_last(long long ccn)
{
    char temp_ccn[16];
    sprintf(temp_ccn,"%lld",ccn);
    int sum = 0;

    for (int i=0; i< strlen(temp_ccn); i++)
    {
        int x = (ccn / 10) % 10;
        x *= 2;
        x = (x >= 10) ? ((x / 10) + (x % 10)) : x;
        sum += x;
        ccn /= 100;
    }
    
    return sum;
}

int remaining(long long ccn)
{
    char temp_ccn[16];
    sprintf(temp_ccn,"%lld",ccn);
    int sum = 0;
    
    for (int i=0; i< strlen(temp_ccn); i++)
    {
        int x = ccn % 10;
        sum += x;
        ccn /= 100;
    }

    return sum;
}

void sec_check(long long ccn)
{
    char temp_ccn[16];
    sprintf(temp_ccn,"%lld",ccn);
    switch(strlen(temp_ccn))
    {
        case 13:
            printf("VISA\n");
        case 15:
            ccn /= 10000000000000;
            if (ccn == 34 || ccn == 37){
                printf("AMEX\n");
                break;
            }
        case 16:
            ccn /= 100000000000000;
            if (ccn == 51 || ccn == 52 || ccn == 53 || ccn == 54 || ccn == 55)
            {
                printf("MASTERCARD\n");
                break;
            } else if (ccn / 10 == 4)
            {
                printf("VISA\n");
                break;
            }
        default:
            printf("INVALID\n");
            break;
    }
}