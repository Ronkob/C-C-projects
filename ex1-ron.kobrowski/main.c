#include "cipher.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define BUFFER_SIZE 1026
#define USE_CASE_RUN_SIZE 5
#define USE_CASE_TEST_SIZE 2
#define DECIMAL 10


// your code goes here

/**
 *  defining return type for auxiliary functions, as of EXIT_SUCCESS/FAILURE
 *  where a T return value means the function did it's action successfully
 */
typedef enum Boolean
{
    F = 0,
    T = 1
} Boolean;

typedef enum UseCase
{
    RUN = 1,
    TEST = 2
} UseCase;

/**
 *  test the input arguments at the accepted lengths, and defining the
 *  use_case accordingly
 * @param argc
 * @return 1 for indicating run use_case, 2 for indicating a test use_case,
 * F upon a not valid input
 */
int test_use_case(int argc)
{
    if (argc == USE_CASE_TEST_SIZE)
    {
        return TEST;
    }
    if (argc == USE_CASE_RUN_SIZE)
    {
        return RUN;
    } else
    {
        return F;
    }

}

/**
 * test the file handler pointer
 * @param fp
 * @return T upon success
 */
Boolean test_valid_file_handler(FILE *fp)
{
    if (fp == NULL)
    {
        return F;
    }
    return T;
}

/**
 * test if a path to a file is readable
 * @param in_path
 * @return T upon success
 */
Boolean test_readable_path(char *in_path)
{
    FILE *in = fopen(in_path, "r");
    if (test_valid_file_handler(in) == F)
    {
        return F;
    } else
    {
        fclose(in);
        return T;
    }
}

/**
 * test if a path to a file is writeable
 * @param out_path
 * @return T upon success
 */
Boolean test_writeable_path(char *out_path)
{
    FILE *out = fopen(out_path, "w+");
    if (test_valid_file_handler(out) == F)
    {
        return F;
    } else
    {
        fclose(out);
        return T;
    }
}

/**
 * test if the given shift argument is valid
 * the function tests whether there is anything in the string besides an
 * integer
 * @param string_k given string for the shift argument
 * @return T upon success
 */
Boolean test_shift(char *string_k)
{
    char *processed_k = "";
    strtol(string_k, &processed_k, DECIMAL);
    if (strcmp(processed_k, "") != 0)
    {
        return F;
    }
    return T;
}

/**
 * test valitidy of the given arguments in the RUN case
 * @param argv
 * @return T upon success
 */
Boolean check_case_run(char *argv[])
{
    char *command = argv[1];
    char *string_k = argv[2];
    char *in_path = argv[3];
    char *out_path = argv[4];

    if (strcmp(command, "encode") != 0 && strcmp(command, "decode") != 0)
    {
        fprintf(stderr, "The given command is invalid.\n");
        return F;
    }
    if (test_shift(string_k) != T)
    {
        fprintf(stderr, "The given shift value is invalid.\n");
        return F;
    }
    if (test_readable_path(in_path) != T ||
        test_writeable_path(out_path) != T)
    {
        fprintf(stderr, "The given file is invalid.\n");
        return F;
    }
    return T;
}

/**
 * based on the use case, checks if the input is valid, by using aux function
 * check_case_run(char *argv[]) for the RUN case
 * @param argv
 * @param use_case
 * @return T upon success
 */
Boolean test_input_args(char *argv[], int use_case)
{
    if (!(use_case == RUN || use_case == TEST))
    {
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
        return F;
    }
    if (use_case == RUN)
    {
        return check_case_run(argv);
    } else
    {
        if (strcmp(argv[1], "test") != 0)
        {
            fprintf(stderr, "Usage: cipher test\n");
            return F;
        }
        return T;
    }
    assert(0);
}

/**
 * mother function, to check the validity of the input, using test_use_case,
 * and test_input_args as aux functions
 * @param argc
 * @param argv
 * @return T upon success
 */
int test_input(int argc, char *argv[])
{
    int use_case = test_use_case(argc);
    if (test_input_args(argv, use_case) == T)
    {
        return use_case;
    } else
    {
        return F;
    }
}

/**
 * encode the stuff in in_path to the out_path using a k shift, and the
 * function encode from
 * @param k
 * @param in_path
 * @param out_path
 * @return
 */
Boolean use_encode(long int k, const char *in_path, const char *out_path)
{
    FILE *in_file_p = fopen(in_path, "r");
    FILE *out_file_p = fopen(out_path, "w+");
    if (test_valid_file_handler(in_file_p) == F || test_valid_file_handler
                                                           (out_file_p) == F)
    {
        if (test_valid_file_handler(in_file_p) == T)
        {
            fclose(in_file_p);
        } else if (test_valid_file_handler(out_file_p) == F)
        {
            fclose(out_file_p);
        }
        fprintf(stderr, "The given file is invalid.\n");
        return F;
    }
    char buffer[BUFFER_SIZE] = {0};
    while (fgets(buffer, BUFFER_SIZE, in_file_p) != NULL)
    {
        encode(buffer, (int) k);
        fprintf(out_file_p, "%s", buffer);
    }
    fclose(in_file_p);
    fclose(out_file_p);
    return T;
}

/**
 * uses the encode function with a backwards shift
 * @param k
 * @param in_path
 * @param out_path
 * @return T upon success
 */
Boolean use_decode(long int k, const char *in_path, const char *out_path)
{
    return use_encode(-k, in_path, out_path);
}


/** runs in RUN mode
 * reading the given arguments, and calling aux functions as use_encode
 *
 * @param argv
 * @return T upon success
 */
int run_command(char *argv[])
{
    const char *command = argv[1];
    const long int k = strtol(argv[2], NULL, DECIMAL);
    const char *in_path = argv[3];
    const char *out_path = argv[4];

    if (strcmp(command, "encode") == 0)
    {
        // run encode
        return use_encode(k, in_path, out_path);
    } else if (strcmp(command, "decode") == 0)
    {
        // run decode
        return use_decode(k, in_path, out_path);
    }
    fprintf(stderr, "Something went wrong:(");
    assert(0);
}

/**
 * runs the given tests
 * @return the number of tests that failed - 0 upon success
 */
int run_all_tests()
{
    // tests for encode
    int first = test_encode_non_cyclic_lower_case_positive_k();
    int second = test_encode_cyclic_lower_case_special_char_positive_k();
    int third = test_encode_non_cyclic_lower_case_special_char_negative_k();
    int forth = test_encode_cyclic_lower_case_negative_k();
    int fifth = test_encode_cyclic_upper_case_positive_k();
    // tests for decode
    int sixth = test_decode_non_cyclic_lower_case_positive_k();
    int seventh = test_decode_cyclic_lower_case_special_char_positive_k();
    int eighth = test_decode_non_cyclic_lower_case_special_char_negative_k();
    int ninth = test_decode_cyclic_lower_case_negative_k();
    int tenth = test_decode_cyclic_upper_case_positive_k();
    int sum = first + second + third + forth + fifth + sixth + seventh +
              eighth + ninth + tenth;
    return sum;
}

int main(int argc, char *argv[])
{
    int use_case = test_input(argc, argv);
    if (use_case == F)
    {
        return EXIT_FAILURE;
    }
    if (use_case == RUN)
    {
        // normal run
        int ret = run_command(argv);
        if (ret == T)
        {
            return EXIT_SUCCESS;
        }
    } else // then use_case is TEST
    {
        // run tests
        int ret = run_all_tests();
        if (ret != 0)
        {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
}
