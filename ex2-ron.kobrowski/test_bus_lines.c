#include "test_bus_lines.h"
#include <stdio.h>
//TODO add implementation here

/**
 * see doc in header file
 * @param start
 * @param end
 * @return
 */
int is_sorted_by_distance(BusLine *start, BusLine *end){
    long size = end-start;
    int flag =0;
    for (int i =0; i<size-1 && flag == 0; i++){
        if (start[i+1].distance<start[i].distance){
            flag = 1;
        }
    }

    if (flag == 0)
    {
        fprintf(stdout, "TEST 1 PASSED: testing the array is sorted by "
                        "distance\n");
    }
    else{
        fprintf(stdout, "TEST 1 FAILED: testing the array is sorted by "
                        "distance\n");
    }
    return flag;
}

/**
 * see doc in header file
 * @param start
 * @param end
 * @return
 */
int is_sorted_by_duration (BusLine *start, BusLine *end){
    long size = end-start;
    int flag =0;
    for (int i =0; i<size-1 && flag == 0; i++){
        if (start[i+1].duration<start[i].duration){
            flag = 1;
        }
    }
    if (flag == 0)
    {
        fprintf(stdout, "TEST 3 PASSED: testing the array is sorted by "
                        "duration\n");
    }
    else{
        fprintf(stdout, "TEST 3 FAILED: testing the array is sorted by "
                        "duration \n");
    }
    return flag;
}

/**
 * see doc in header file
 * @param start_sorted
 * @param end_sorted
 * @param start_original
 * @param end_original
 * @return
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original){
    int flag = 0;
    int origin_size = (int) (end_original-start_original);
    int sorted_size = (int) (end_sorted-start_sorted);
    if (origin_size != sorted_size){
        flag = 1;
    }
    int num_of_identical_line_numbers = 0;
    // assuming there are no repeating line numbers in original
    for (int i =0; i<origin_size; i++){
        flag = 0 ;
        for (int j =0; j<origin_size && flag == 0; j++){
            if(start_original[i].line_number == start_sorted[j].line_number){
                num_of_identical_line_numbers++;
                flag = 1;
            }
        }
    }
    if (num_of_identical_line_numbers == origin_size){
        return 0;
    }
    return 1;
}