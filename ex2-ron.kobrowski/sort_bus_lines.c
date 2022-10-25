#include "sort_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>


/**
 * an aux function that swaps Bus Line structs.
 * @param ap
 * @param bp
 */
void bus_line_swap(BusLine *ap, BusLine *bp){
    BusLine temp = *ap;
    *ap = *bp;
    *bp = temp;
}

/**
 * bubble sort an array of structs BusLine based on property distance = see
 * header file
 * @param start
 * @param end
 */
void bubble_sort(BusLine *start, BusLine *end){
    int array_size = (int)(end-start);
    for (int i=0; i<array_size-1; i++){
        for (int j=0; j<array_size-i-1; j++){
            if((*(start+j)).distance>(*(start+j+1)).distance){
                bus_line_swap((start+j), (start+j+1));
            }
        }
    }

}
/**
 * aux function for quick sort - see header file
 * @param start
 * @param end element after the last one
 * @return a pointer for the pivot element in the array, with all the
 * smaller elements to it's left, the bigger ones to the right.
 */
BusLine *partition (BusLine *start, BusLine *end){
    int pivot = (*(end-1)).duration;
    BusLine *i = start-1;
    BusLine *j;

    for (j=start; j<end-1; j++){
        if(j->duration <= pivot){
            i++;
            bus_line_swap(i, j);
        }
    }
    bus_line_swap(i+1, (end-1));
    return i+1;
}


/**
 * quick sort an array of structs BusLine based on property duration
 * @param start first element of array
 * @param end alement after the last
 */
void quick_sort(BusLine *start, BusLine *end){
    if ((&(start)->duration) < (&(end-1)->duration)){
        BusLine *pivot = partition(start, end);
        quick_sort(start, pivot);
        quick_sort(pivot+1, end);
    }
}




