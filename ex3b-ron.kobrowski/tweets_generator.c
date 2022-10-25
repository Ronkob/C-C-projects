//
// Created by ron.kobrowski on 11/08/2022.
//
#include "linked_list.h"
#include "markov_chain.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define NDEBUG

#define MIN_WORDS_TO_READ (-1)
#define SPECIFIC_WORDS_USE_CASE 5
#define DECIMAL 10
#define MAX_LINE_BUFFER 1002
#define MAX_TWEET_LENGTH 20
#define DELIMITER " \n"
#define LINE_MAX_SIZE 1000
#define NUMBER_OF_ARGS 5

#define USAGE_MESSAGE "Usage: The program require the following parameters\n1) Seed number.\n2) Number of lines to generate.\n3) Absolute path to file.\n4) Optional - Number of lines to read from file. If no value is given read the entire file\n"
#define INVALID_FILE_MESSAGE "Error: file is not valid\n"

typedef enum UseCase
{
    INVALID = 0,
    VALID = 1
} UseCase;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    fprintf(stdout, "%s", error_msg);
    if (database != NULL)
    {
        if (*database != NULL)
        {
            free_markov_chain(database);
        }
    }
    return EXIT_FAILURE;
}

/************************/
/*    aux functions     */
/************************/
/**
 * a string-implementation of the general function
 * @param string
 * @return
 */
static void *string_copier(void *string)
{
    char *new_word = malloc(strlen(string) + 1);
    if (new_word == NULL)
    {
        handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
        return false;
    }
    strcpy(new_word, string);
    return new_word;
}

/**
 * a string-implementation of the general function
 */
static int string_comp(void *string1, void *string2)
{
//    char *str1 = (char *) string1;
//    char *str2 = (char *) string2;
    return strcmp(string1, string2);
}

/**
 * a string-implementation of the general function
 */
static void free_string(void *string)
{
    char *str = (char *) string;
    free(str);
    string = NULL;
}

/**
 * a string-implementation of the general function
 */
static bool is_end_word(void *word)
{
    if (!word)
    {
        return true;
    }
    char *string = word;
    int len = (int) strlen(string);
    if (string_comp((string + len - 1), ".") == 0)
    {
        return true;
    }
    return false;
}

/**
 * a string-implementation of the general function
 */
static void print_string(void *string)
{
    char *str = (char *) string;
    if (is_end_word(string))
    {
        fprintf(stdout, "%s", str);
    } else
    {
        fprintf(stdout, "%s ", str);
    }
}


/**
 * test if a path to a file is readable
 * @param in_path
 * @return T upon success
 */
static bool test_readable_path(const char *in_path)
{
    FILE *in = fopen(in_path, "r");
    if (in == NULL)
    {
        return false;
    } else
    {
        fclose(in);
        return true;
    }
}

/**
 * test if the given arguments are fine, or need to return usage error
 * @param argc
 * @param argv
 * @return
 */
static int test_use_case(const int argc, char *const argv[])
{
    if (argc < NUMBER_OF_ARGS - 1 || argc > NUMBER_OF_ARGS)
    {
        handle_error(USAGE_MESSAGE, NULL);
        return INVALID;
    } else
    {
        bool file_validity = test_readable_path(argv[3]);
        if (!file_validity)
        {
            handle_error(INVALID_FILE_MESSAGE, NULL);
            return INVALID;
        }
        return VALID;
    }


}

/**
 * aux function, self explanatory
 * @param argv
 * @param argc
 * @return
 */
static int get_num_words_to_read(char *const argv[], int argc)
{
    int num_words_to_read = MIN_WORDS_TO_READ;
    if (argc == SPECIFIC_WORDS_USE_CASE)
    {
        num_words_to_read = (int) strtol(argv[SPECIFIC_WORDS_USE_CASE - 1],
                                         NULL, DECIMAL);
    }
    return num_words_to_read;
}


/**
 * separating the line to single words, strips it using aux function and then sends it to add_to_database @see markov_chain.c
 * @param line
 * @param markov_chain
 * @param readed_words
 * @param words_to_read
 * @return VALID upon success
 */
static int insert_line_to_database(char *line, MarkovChain *markov_chain, int *
readed_words, int words_to_read)
{
    char *word = strtok(line, DELIMITER);
    Node *prev_node = NULL;
    while ((((*readed_words) < words_to_read) || (words_to_read == -1)) &&
           word != NULL)
    {
        Node *word_node = add_to_database(markov_chain, word);
        if (word_node == NULL)
        {
            handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
            return INVALID;
        }
        if (prev_node != NULL)
        {
            if (!(markov_chain->is_last(prev_node->data->data)))
            {
                bool ret = add_node_to_counter_list(prev_node->data,
                                                    word_node->data,
                                                    markov_chain);
                if (ret == false)
                {
                    return INVALID;
                }
            }
        }
        (*readed_words)++;
        word = strtok(NULL, DELIMITER); // delim by " " or "\n"
        prev_node = word_node;
    }
    return VALID;
}

/**
 * fills the unique words database (markovchain) using the given file pointer
 * @param fp
 * @param words_to_read
 * @param markov_chain
 * @return
 */
static int fill_database(FILE *fp, const int words_to_read, MarkovChain
*markov_chain)
{
    char line[LINE_MAX_SIZE] = {0};
    int num_readed_words = 0;
    while ((fgets(line, MAX_LINE_BUFFER, fp) != NULL) &&
           ((num_readed_words < words_to_read) || (words_to_read == -1)))
    {
        int ret = insert_line_to_database(line, markov_chain,
                                          &num_readed_words,
                                          words_to_read);
        if (ret == INVALID)
        {
            return INVALID;
        }
    }
    return VALID;
}

/**
 * main function for generating a number of new tweets
 * @param unique_words
 * @param num_of_tweets
 * @return
 */
static bool tweets_generator(MarkovChain *unique_words, int num_of_tweets)
{
    for (int i = 0; i < num_of_tweets; i++)
    {
        MarkovNode *first_word = get_first_random_node(unique_words);
        if (first_word == NULL)
        {
            return false;
        }
        fprintf(stdout, "Tweet %d: ", i + 1);
        generate_random_sequence(unique_words, first_word, MAX_TWEET_LENGTH);
        fprintf(stdout, "\n");
    }
    return true;

}

/**
 * sets up a given allocation to a database struct to initialized values
 * @param unique_words
 * @return true upon success
 */
static bool init_database(MarkovChain *unique_words)
{
    (unique_words)->database = malloc(sizeof(LinkedList));
    if ((unique_words)->database == NULL)
    {
        handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
        return false;
    }
    (unique_words)->database->size = 0;
    (unique_words)->database->first = NULL;
    (unique_words)->database->last = NULL;
    return true;
}

/**
 * sets up a given allocation to a database struct to initialized values
 * @param unique_words
 * @return true upon success
 */
static bool init_unique_words(MarkovChain *unique_words)
{
    unique_words->copy_func = string_copier;
    unique_words->print_func = print_string;
    unique_words->free_data = free_string;
    unique_words->is_last = is_end_word;
    unique_words->comp_func = string_comp;
    bool ret = init_database(unique_words);
    return ret;

}

/**
 * main command hub for running the program after main
 * @param argv
 * @param argc
 * @return
 */
static bool run_command(char *const argv[], const int argc)
{
    int seed = (int) strtol(argv[1], NULL, DECIMAL);
    int num_of_tweets = (int) strtol(argv[2], NULL, DECIMAL);
    const char *in_path = argv[3];
    FILE *fp = fopen(in_path, "r");
    int num_words_to_read = get_num_words_to_read(argv, argc);
    srand(seed);

    MarkovChain *unique_words = malloc(sizeof(MarkovChain));
    if (unique_words == NULL)
    {
        handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
        return false;
    }
    bool unique_words_initialize = init_unique_words(unique_words);
    if (!unique_words_initialize)
    {
        return false;
    }

    int ret = fill_database(fp, num_words_to_read, unique_words);
    if (ret == INVALID)
    {
        return false;
    }
    bool bull_ret = tweets_generator(unique_words, num_of_tweets);
    free_markov_chain(&unique_words);
    return bull_ret;
}

int main(int argc, char *argv[])
{
    int use_case = test_use_case(argc, argv);
    if (use_case == INVALID)
    {
        return EXIT_FAILURE;
    }
    bool ret = run_command(argv, argc);
    if (!ret)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}



