#include <stdio.h>
#include <cs50.h>
#include <math.h>

float owed(void);
void change(int);

int main(void)
{
    change(owed());
}

float owed(void)
{
    printf("O hai! How much change is owed?\n");
    while (true){
        float n = get_float();
        if (n < 0){
            printf("How much change is owed?\n");
        }
        else
        {
            int i = round(n * 100);
            return i;
        }
    }
}

void change(int i)
{
    int coins = 0;
    coins += i / 25;
    coins += (i % 25) / 10;
    coins += ((i % 25) % 10) / 5;
    coins += (((i % 25) % 10) % 5) / 1;
    printf("%i\n", coins);
}