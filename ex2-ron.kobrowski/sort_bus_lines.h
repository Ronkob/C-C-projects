#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H
// and #endif //EX2_REPO_SORTBUSLINES_H
/**
 * a type that represents a bus line by a unique number, and distance,
 * duration properties.
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * sort the given array in bubble sort method, by distance.
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * sort the given array in quick sort by duration
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * an aux function for the quick sort, returns a pointer to the pivot after
 * swapping items to it's right and left.
 *
 * @return a pointer for the pivot element in the array, with all the
 * smaller elements to it's left, the bigger ones to the right.
 */
BusLine *partition (BusLine *start, BusLine *end);
// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
