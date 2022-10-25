//
// Created by ron.kobrowski on 11/08/2022.
//
#include "linked_list.h"
#include "markov_chain.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_WORDS_TO_READ (-1)
#define SPECIFIC_WORDS_USE_CASE 5
#define DECIMAL 10
#define MAX_LINE_BUFFER 1002
#define MAX_TWEET_LENGTH 20
#define DELIMITER " \n"
#define LINE_MAX_SIZE 1000
#define NUMBER_OF_ARGS 5

typedef enum UseCase
{
    INVALID = 0,
    VALID = 1
} UseCase;

/**
 * test if a path to a file is readable
 * @param in_path
 * @return T upon success
 */
bool test_readable_path(const char *in_path)
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
int test_use_case(const int argc, const char *argv[])
{
    if (argc < NUMBER_OF_ARGS-1 || argc > NUMBER_OF_ARGS)
    {
        return INVALID;
    } else
    {
        bool file_validity = test_readable_path(argv[3]);
        if (!file_validity)
        {
            fprintf(stdout, "ERROR: file is not valid\n");
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
int get_num_words_to_read(const char *argv[], int argc)
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
 * return a copy of the string.
 * not stripped, bc don't need to - the strtok does it for us.
 * and strcpy does it well to
 * @param word_to_strip
 * @return
 */
char *strip_word(char *word_to_strip)
{
    int len = (int) strlen(word_to_strip);
    char *new_word = calloc(2, len);
    strcpy(new_word, word_to_strip);
    return new_word;
}

/**
 * separating the line to single words, strips it using aux function and then sends it to add_to_database @see markov_chain.c
 * @param line
 * @param markov_chain
 * @param readed_words
 * @param words_to_read
 * @return VALID upon success
 */
int insert_line_to_database(char *line, MarkovChain *markov_chain, int *
readed_words, int words_to_read)
{
    char *word = strtok(line, DELIMITER);
    Node *prev_node = NULL;
    while ((((*readed_words) < words_to_read) || (words_to_read == -1)) &&
           word != NULL)
    {
        word = strip_word(word);
        Node *word_node = add_to_database(markov_chain, word);
        if (prev_node != NULL)
        {
            if (!is_end_word(prev_node->data))
            {
                bool ret = add_node_to_counter_list(prev_node->data,
                                                    word_node->data);
                if (ret == false)
                {
                    return INVALID;
                }
            }
        }
        if (word_node == NULL)
        {
            fprintf(stderr, "ERROR: memory allocation failed\n");
            return INVALID;
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
int fill_database(FILE *fp, const int words_to_read, MarkovChain
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
bool tweets_generator(MarkovChain *unique_words, int num_of_tweets)
{
    for (int i = 0; i < num_of_tweets; i++)
    {
        MarkovNode *first_word = get_first_random_node(unique_words);
        if (first_word == NULL)
        {
            return false;
        }
        fprintf(stdout, "Tweet %d:", i + 1);
        generate_random_sequence(unique_words, first_word, MAX_TWEET_LENGTH);
        fprintf(stdout, "\n");
    }
    return true;

}

/**
 * aux function not being used in submission
 * @param words
 */
void print_unique_words(MarkovChain *words)
{
    FILE *fp = fopen("word_list.txt", "w");
    Node *head = words->database->first;
    for (int i = 0; i < words->database->size; i++)
    {
        fprintf(fp, "$%s$\n", head->data->data);
        head = head->next;
    }
}

/**
 * main command hub for running the program after main
 * @param argv
 * @param argc
 * @return
 */
bool run_command(const char *argv[], const int argc)
{
    int seed = (int) strtol(argv[1], NULL, DECIMAL);
    int num_of_tweets = (int) strtol(argv[2], NULL, DECIMAL);
    const char *in_path = argv[3];
    FILE *fp = fopen(in_path, "r");
    int num_words_to_read = get_num_words_to_read(argv, argc);
    srand(seed);

    MarkovChain *unique_words = malloc(sizeof(MarkovChain));
    unique_words->database = malloc(sizeof(LinkedList));
    int ret = fill_database(fp, num_words_to_read, unique_words);
    if (ret == INVALID)
    {
        return false;
    }

    bool bull_ret = tweets_generator(unique_words, num_of_tweets);

    free_markov_chain(&unique_words);
    return bull_ret;

}

int main(const int argc, const char *argv[])
{
    int use_case = test_use_case(argc, argv);
    if (use_case == INVALID)
    {
        fprintf(stdout, "Usage: The program require the following parameters\n"
                        "1) Seed number.\n"
                        "2) Number of lines to generate.\n"
                        "3) Absolute path to file.\n"
                        "4) Optional - Number of lines to read from file. If"
                        "no value is given read the entire file\n");
        return EXIT_FAILURE;
    }
    bool ret = run_command(argv, argc);
    if (!ret)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}