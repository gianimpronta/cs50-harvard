/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"
#define MAX 65536

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n <= 0)
    {
        return false;
    }
    int start = 0;
    int middle;
    int end = n-1;
    while (start <= end)
    {   
        middle = (start + end)/2;
        if (value == values[middle])
        {
            return true;
        }
        if (value > values[middle])
        {
            start = middle + 1;
        }
        else
        {
            end = middle - 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int count[MAX+1];
    int output[n];
    // Create an auxiliar array with all 0's
    for (int i = 0 ; i < MAX+1; i++)
    {
        count[i] = 0;
    }
    // Count how many values[j] are there
    for (int j = 0; j < n; j++)
    {
        count[values[j]] += 1;
    }
    
    for (int k = 1; k < MAX+1; k++)
    {
        count[k] += count[k-1];
    }

    for (int l = n-1; l >= 0; l--)
    {
        output[count[values[l]]] = values[l];
        count[values[l]] -= 1;
    }
    
    for (int a = 1; a <= n; a++)
    {
        values[a-1] = output[a];
    }
    
    return;
}
