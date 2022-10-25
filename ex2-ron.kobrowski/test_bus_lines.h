#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

// write only between #define EX2_REPO_TESTBUSLINES_H
// and #endif //EX2_REPO_TESTBUSLINES_H
/**
 * tests if the given array of structs BusLine is ordered by distances,
 * ascending
 * @param start pointer to the first item in the array
 * @param end pointer to the item after the last item
 * @return 0 upon success.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);
/**
 * test if the given lines are sorted by duration
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);
/**
 * compares to see if the two arrays have the same items
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H
// and #endif //EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
