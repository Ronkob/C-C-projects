#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


#define BUFFER_SIZE 60
#define ARGC_REQ_SIZE 2
#define DECIMAL 10

#define MAX_LINE_NUMBER 999
#define MIN_LINE_NUMBER 1
#define MAX_DISTANCE 1000
#define MIN_DISTANCE 0
#define MAX_DURATION 100
#define MIN_DURATION 10


/**
 * a program that gets info on bus lines, and is able to sort them in
 * different methods.
 */

typedef enum UseCase
{
    INVALID = 0,
    BUBBLE = 1,
    QUICK = 2,
    TEST = 3
} UseCase;

/**
 * test if the buffer is a valid input for number of lines
 * @param buffer
 * @return
 */
bool test_valid_num_of_lines_input(char buffer[])
{
    char *processed_input = "";
    long n = strtol(buffer, &processed_input, DECIMAL);
    if (strcmp(processed_input, "\n") != 0 || n < 1)
    {
        return false;
    }
    return true;
}

/**
 * a function that asks the user for a valid number of bus lines to be input
 * in the future.
 * uses the aux function test_valid_num_of_lines_input to check the validity
 * of the input.
 * @return an int bigger than 0 that is the number of bus lines
 */
int get_num_of_lines()
{
    fprintf(stdout, "Enter number of lines. Then enter\n");
    char buffer[BUFFER_SIZE] = {0};
    fgets(buffer, BUFFER_SIZE, stdin);
    while (!test_valid_num_of_lines_input(buffer))
    {
        fprintf(stdout, "ERROR: number of lines should be a positive "
                        "integer\n");
        fprintf(stdout, "Enter number of lines. Then enter\n");
        fgets(buffer, BUFFER_SIZE, stdin);
    }
    long n = strtol(buffer, NULL, DECIMAL);
    return (int) n;
}

/**
 * test if buffer is a valid input for a line info
 * @param buffer
 * @return true upon success
 */
bool test_valid_line_info(char buffer[])
{
    int line_number, distance, duration;
    sscanf(buffer, "%d,%d,%d", &line_number, &distance, &duration);
    if (line_number < MIN_LINE_NUMBER || line_number > MAX_LINE_NUMBER)
    {
        fprintf(stdout, "ERROR: Line number should be an integer between 1 "
                        "and 999 (includes)\n");
        return false;
    }
    if (distance < MIN_DISTANCE || distance > MAX_DISTANCE)
    {
        fprintf(stdout, "ERROR: Distance should be an integer between 0 "
                        "and 1000 (includes)\n");
        return false;
    }
    if (duration < MIN_DURATION || duration > MAX_DURATION)
    {
        fprintf(stdout, "ERROR: Duration should be an integer between 10 "
                        "and 100 (includes)\n");
        return false;
    }
    return true;
}

/**
 * assuming the buffer is from correct input structure, build a busline
 * struct from the input.
 * @param start
 * @param i
 * @param buffer
 * @return
 */
BusLine build_bus_line(char buffer[])
{
    int line_number, distance, duration;
    sscanf(buffer, "%d,%d,%d", &line_number, &distance, &duration);
    BusLine line = {line_number, distance, duration};
    return line;
}

/**
 * transferring ownership of bus-lines array
 *
 * FUNCTION IS ALLOCATING MEM BUT NOT FREEING!
 *
 * @param n number of bus lines, was given by the user. an int bigger than 0
 * @return a pointer to a dynamic array of structs buslines.
 *
 *
 */
BusLine *get_bus_lines(int n)
{
    // a for loop collecting all the data from the user
    BusLine *start = malloc(sizeof *start * n); // dynamic array of n
    // Bus-line structs
    for (int i = 0; i < n; i++)
    {
        fprintf(stdout, "Enter line info. Then enter\n");
        char buffer[BUFFER_SIZE] = {0};
        fgets(buffer, BUFFER_SIZE, stdin);
        while (!test_valid_line_info(buffer))
        {
            fprintf(stdout, "Enter line info. Then enter\n");
            fgets(buffer, BUFFER_SIZE, stdin);
        }
        BusLine line = build_bus_line(buffer);
        start[i] = line;
    }
    return start;

}

/**
 *
 * prints bus lines from an array of bus line structs, according to the format.
 * @param lines
 * @param n
 */
void print_bus_lines(BusLine *lines, int n)
{
    BusLine *lp = lines;
    for (int i = 0; i < n; i++)
    {
        fprintf(stdout, "%d,%d,%d\n", lp[i].line_number, lp[i].distance,
                lp[i].duration);
    }
}

/**
 * runs the tests for the bubble sort, and returns the number of tests that
 * failed
 * @param start_sorted
 * @param end_sorted
 * @param start_original
 * @param end_original
 * @return
 */
int get_sum_of_tests_bubble(BusLine *start_sorted, BusLine *end_sorted, BusLine
*start_original, BusLine *end_original)
{
    int first = is_sorted_by_distance(start_sorted, end_sorted);
    int second = is_equal(start_sorted, end_sorted, start_original,
                          end_original);
    if (second == 0)
    {
        fprintf(stdout, "TEST 2 PASSED: testing the array have the same items "
                        "after sorting\n");
    } else
    {
        fprintf(stdout, "TEST 2 FAILED: testing the array have the same items "
                                                "after sorting\n");
    }
    int sum = first + second;
    return sum;
}

/**
 * same as the bubble, but with quick
 * @param start_sorted
 * @param end_sorted
 * @param start_original
 * @param end_original
 * @return
 */
int get_sum_of_tests_quick(BusLine *start_sorted, BusLine *end_sorted, BusLine
*start_original, BusLine *end_original)
{
    int third = is_sorted_by_duration(start_sorted, end_sorted);
    int forth = is_equal(start_sorted, end_sorted, start_original,
                         end_original);
    if (forth == 0)
    {
        fprintf(stdout, "TEST 4 PASSED: testing the array have the same "
                        "items after sorting");
    } else
    {
        fprintf(stdout, "TEST 4 FAILED: testing the array have the same "
                        "items after sorting");
    }
    int sum = third + forth;
    return sum;
}

/**
 * method for running the tests
 * @param start
 * @param end
 * @param size
 * @param start_bubble_sorted
 * @param start_quick_sorted
 * @return the number of failed tests, out of 4
 */
int get_sum_of_tests(BusLine *start, BusLine *end, int size,
                     BusLine *start_bubble_sorted,
                     BusLine *start_quick_sorted)
{
    bubble_sort(start_bubble_sorted, start_bubble_sorted + size);
    int bubble_sum_of_tests = get_sum_of_tests_bubble(start_bubble_sorted,
                                                      start_bubble_sorted +
                                                      size,
                                                      start,
                                                      end);

    quick_sort(start_quick_sorted, start_quick_sorted + size);
    int quick_sum_of_tests = get_sum_of_tests_quick(start_quick_sorted,
                                                    start_quick_sorted + size,
                                                    start, end);

    int sum_of_tests = bubble_sum_of_tests + quick_sum_of_tests;
    return sum_of_tests;
}

/**
 * medthod for running the tests.
 * allocating memory and freeing it.
 * @param start
 * @param end
 * @return
 */
bool test_runner(BusLine *start, BusLine *end)
{
    int size = (int) (end - start);
    BusLine *start_bubble_sorted = malloc(sizeof *start_bubble_sorted * size);
    memcpy(start_bubble_sorted, start, sizeof(BusLine) * size);

    BusLine *start_quick_sorted = malloc(sizeof *start_quick_sorted * size);
    memcpy(start_quick_sorted, start, sizeof(BusLine) * size);

    int sum_of_tests = get_sum_of_tests(start, end, size, start_bubble_sorted,
                                        start_quick_sorted);

    free(start_bubble_sorted);
    start_bubble_sorted = NULL;
    free(start_quick_sorted);
    start_quick_sorted = NULL;

    if (sum_of_tests == 0)
    {
        free(start);
        return true;
    }
    free(start);
    return false;
}

/**
 * FREEING get_bus_lines MEM ALLOCATION
 * @param use_case a macro
 * @param start pointer to the address of the first item in the bus lines
 * array.
 * @param end pointer to the address of the item AFTER the last in the bus
 * lines
 * array.
 * @return
 */
bool run_command(int use_case)
{
    int num_of_lines = get_num_of_lines();
    BusLine *start = get_bus_lines(num_of_lines);
    BusLine *end = &(start[num_of_lines]); // not a real element of start
    switch (use_case)
    {
        case INVALID:
            assert(0);
            return false;
        case BUBBLE:
            bubble_sort(start, end);
            print_bus_lines(start, num_of_lines);
            free(start);
            return true;
        case QUICK:
            quick_sort(start, end);
            print_bus_lines(start, num_of_lines);
            free(start);
            return true;
        case TEST:
            return test_runner(start, end);
        default:
            return false;
    }
}


/**
 * checks for a valid call of the executable file with one argument: {test,
 * bubble, test}
 *
 * @param argc
 * @param argv
 * @return defined macro for the given use_case of the program
 */
int check_use_case(int argc, char *argv[])
{
    if (argc != ARGC_REQ_SIZE)
    {
        return INVALID;
    }
    if (strcmp(argv[1], "bubble") == 0)
    {
        return BUBBLE;
    }
    if (strcmp(argv[1], "quick") == 0)
    {
        return QUICK;
    }
    if (strcmp(argv[1], "test") == 0)
    {
        return TEST;
    }
    return INVALID;
}


/**
 * main function.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    int use_case = check_use_case(argc, argv);
    if (use_case == INVALID)
    {
        fprintf(stdout, "USAGE: one of {bubble, quick, test}");
        return EXIT_FAILURE;
    }
    bool ret = run_command(use_case);
    if (ret)
    {
        return EXIT_SUCCESS;

    }
    return EXIT_FAILURE;

}
