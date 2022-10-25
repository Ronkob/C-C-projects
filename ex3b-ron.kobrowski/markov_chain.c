//
// Created by ron.kobrowski on 11/08/2022.
//

#include <stdlib.h>
#include "markov_chain.h"
#include "linked_list.h"
#include <string.h>
#include <assert.h>

int get_random_number(int max_number)
{
    assert(max_number);
    return rand() % max_number;
}

/**
 * return the *Node in the place place of the list
 * @param list
 * @param place
 * @return pointer to Node is the place place
 */
Node *get_linked_list_place(LinkedList *list, int place)
{
    if (list == NULL)
    {
        return NULL;
    }
    Node *head = list->first;
    for (int i = 0; i < place; i++)
    {
        head = head->next;
    }
    return head;
}

/**
 * @see doc in header file
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node(MarkovChain *markov_chain)
{
    int size = markov_chain->database->size;
    int rand_place = get_random_number(size);
    MarkovNode *p_markov_node = (get_linked_list_place(markov_chain->database,
                                                       rand_place))->data;
    while (markov_chain->is_last(p_markov_node->data))
    {
        rand_place = get_random_number(size);
        p_markov_node = get_linked_list_place(markov_chain->database,
                                              rand_place)->data;
    }
    return p_markov_node;
}

int count_all_next_words(MarkovNode *markovNode)
{
    if (markovNode == NULL)
    {
        return -1;
    }
    int counter = 0;
    int max_size = markovNode->list_size;
    for (int i = 0; i < max_size; i++)
    {
        counter = counter + ((markovNode->counter_list)[i]).frequency;
    }
    return counter;
}

MarkovNode *choose_the_kth_next_markov_node(MarkovNode *markov_node, int k)
{
    int counter = 0;
    MarkovNode *head = markov_node->counter_list[0].markov_node;
    for (int i = 0; (i < markov_node->list_size && counter <= k); i++)
    {
        head = markov_node->counter_list[i].markov_node;
        counter += markov_node->counter_list[i].frequency;
    }
    return head;
}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr)
{
    int size = count_all_next_words(state_struct_ptr);
    int random = get_random_number(size);
    MarkovNode *next_markov_node = choose_the_kth_next_markov_node(
            state_struct_ptr, random);
    return next_markov_node;
}


void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    int flag = 0;
    if (first_node == NULL)
    {
        first_node = get_first_random_node(markov_chain);
    }
    MarkovNode *current = first_node;
    markov_chain->print_func(current->data);
    for (int i = 0; i < max_length - 1 && flag == 0; i++)
    {
        current = get_next_random_node(current);
        if (current == NULL)
        {
            return;
        }
        if (markov_chain->is_last(current->data))
        {
            flag = 1;
        }
        markov_chain->print_func(current->data);
    }
}

void free_markov_node(MarkovChain *markovChain, MarkovNode *markovNode)
{
    free(markovNode->counter_list);
    markovChain->free_data(markovNode->data);
    free(markovNode);
}


void free_node(MarkovChain *markovChain, Node *head)
{
    free_markov_node(markovChain, head->data);
    free(head);
}

void free_database(MarkovChain *markovChain)
{
    Node *head = markovChain->database->first;
    while (head != NULL)
    {
        Node *next = head->next;
        free_node(markovChain, head);
        head = next;
    }
    free(head);
    free(markovChain->database);
    markovChain->database = NULL;
}

void free_markov_chain(MarkovChain **ptr_chain)
{
    if ((*ptr_chain)->database != NULL)
    {
        free_database(*ptr_chain);
    }
    free(*ptr_chain);
    *ptr_chain = NULL;
}

int corr_markov_node_place(NextNodeCounter *list, int size, MarkovNode
*next_word)
{
    for (int i = 0; i < size; i++)
    {
        if ((list[i].markov_node) == (next_word))
        {
            return i;
        }
    }
    return -1;
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markovChain)
{
    if (markovChain->is_last(first_node->data))
    {
        return true;
    }
    int place = corr_markov_node_place(first_node->counter_list,
                                       first_node->list_size,
                                       second_node);
    if (place == -1)
    { // word not in list before
        (first_node->list_size)++;
        first_node->counter_list = realloc((first_node->counter_list), (
                (first_node->list_size) * sizeof(NextNodeCounter)));
        if ((first_node->counter_list) == NULL)
        {
            return false;
        }
        (first_node->counter_list)[(first_node->list_size) - 1].markov_node
                = second_node;
        (first_node->counter_list)[(first_node->list_size) - 1].frequency = 1;
    } else
    {
        ((first_node->counter_list)[place].frequency)++;
    }
    return true;
}


Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
    Node *head = markov_chain->database->first;
    while (head != NULL)
    {

        if (markov_chain->comp_func((head->data)->data, data_ptr) == 0)
        {
            return head;
        }
        head = head->next;
    }
    return head;
}

/**
 * ALLOCATES MEMORY!
 * FREED IN:
 *  1. if already exists than freed in add_node_to_cnt_list
 *  2. else, freed with the whole markov_chain
 * @param data_ptr
 * @return
 */
MarkovNode *create_markov_node(MarkovChain *markovChain, void *data_ptr)
{
    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL)
    {
        return NULL;
    }
    void *new_data = markovChain->copy_func(data_ptr);
    if (new_data == NULL)
    {
        return NULL;
    }
    new_markov_node->data = new_data;
    new_markov_node->counter_list = NULL;
    new_markov_node->list_size = 0;
    return new_markov_node;
}

Node *add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
    if (data_ptr == NULL || markov_chain == NULL)
    {
        return NULL;
    }
    Node *in_db = get_node_from_database(markov_chain, data_ptr);
    if (in_db != NULL)
    { // as if the node is known word
        return in_db;
    } else
    {
        MarkovNode *new_markov_node = create_markov_node(markov_chain,
                                                         data_ptr);
        if (new_markov_node == NULL)
        {
            return NULL;
        }

        add(markov_chain->database, new_markov_node);
        return markov_chain->database->last;

    }
}

