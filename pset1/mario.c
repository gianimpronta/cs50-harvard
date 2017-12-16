#include <stdio.h>
#include <cs50.h>

int get_height(void);
void make_pyramid(int height);
void spaces(int n);
void blocks(int n);

int main(void)
{
    
    int height = get_height();
    make_pyramid(height);
}

int get_height(void)
{
    while (true)
    {
        printf("Height: ");
        int i = get_int();
        if (i >= 0 && i <= 23)
        {
            return i;
        } 
    }
}

void make_pyramid(int height)
{
    int bl = 1;
    for (int i=0; i < height; i++)
    {
        int sp = height - bl;
        spaces(sp);
        blocks(bl);
        spaces(2);
        blocks(bl);
        printf("\n");
        bl ++;
    }
}

void spaces(int n)
{
    for (int i=0; i < n; i++)
        {
            printf(" ");
        }
}

void blocks(int n)
{
    for (int k=0; k < n; k++)
        {
            printf("#");
        }
}