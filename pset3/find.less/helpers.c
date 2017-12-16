/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

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
    // Using Insertion Sort
    int tmp;
    for (int unsorted = 1; unsorted < n; unsorted++)
    {
        for (int k = unsorted; k > 0 && values[k] < values[k-1]; k--)
        {
            tmp = values[k];
            values[k] = values[k-1];
            values[k-1] = tmp;
        }    
    }
    return;
}
