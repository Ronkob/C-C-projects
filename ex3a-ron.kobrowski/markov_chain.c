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

bool is_end_word(MarkovNode *word)
{
    if (word == NULL){
        return true;
    }
    int len = (int) strlen(word->data); // all words lengths are less than 100
    if ((word->data)[len - 1] == '.')
    {
        return true;
    }
    return false;
}

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

MarkovNode *get_first_random_node(MarkovChain *markov_chain)
{
    int size = markov_chain->database->size;
    int random = get_random_number(size);
    MarkovNode* word = get_linked_list_place(markov_chain->database,
                                            random)->data;
    while(is_end_word(word)){
        random = get_random_number(size);
        word = get_linked_list_place(markov_chain->database,
                                                 random)->data;
    }
    return word;
}

int count_all_next_words(MarkovNode* markovNode){
    if (markovNode == NULL){
        return -1;
    }
    int counter = 0;
    int max_size = markovNode->list_size;
    for (int i=0; i<max_size; i++){
        counter = counter+((markovNode->counter_list)[i]).frequency;
    }
    return counter;
}

MarkovNode* choose_the_kth_next_word(MarkovNode* markovNode, int k){
    int counter = 0;
    MarkovNode *head = markovNode->counter_list[0].markov_node;
    for (int i=0; (i<markovNode->list_size && counter<=k); i++){
        head = markovNode->counter_list[i].markov_node;
        counter+=markovNode->counter_list[i].frequency;
    }
    return head;
}

MarkovNode *get_next_random_node(MarkovNode *state_struct_ptr){
    int size = count_all_next_words(state_struct_ptr);
//    printf("size: %d. list size: %d\n", size, state_struct_ptr->list_size);
    int random = get_random_number(size);
    MarkovNode* next_word = choose_the_kth_next_word(state_struct_ptr, random);
    return next_word;
}


void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length){
    int flag = 0;
    if (first_node == NULL){
        first_node = get_first_random_node(markov_chain);
    }
    MarkovNode* current = first_node;
    fprintf(stdout," %s", current->data);
    for (int i=0; i<max_length && flag==0; i++){
        current = get_next_random_node(current);
        if (is_end_word(current)){
            flag =1;
        }
        fprintf(stdout, " %s", current->data);
    }
}

void free_markov_node(MarkovNode* markovNode){
    free(markovNode->counter_list);
    free(markovNode->data);
    free(markovNode);
}


void free_node(Node* head){
    free_markov_node(head->data);
    free(head);
}

void free_database(MarkovChain* markovChain){
    Node* head = markovChain->database->first;
    while(head != NULL){
        Node* next = head->next;
        free_node(head);
        head = next;
    }
    free(head);
    free(markovChain->database);
    markovChain->database = NULL;
}

void free_markov_chain(MarkovChain **ptr_chain){
    free_database(*ptr_chain);
    free(*ptr_chain);
    *ptr_chain = NULL;
}

int search_for_word(NextNodeCounter *list, int size, MarkovNode *next_word)
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

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node)
{
    int place = search_for_word(first_node->counter_list,
                                first_node->list_size, second_node);
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


Node *get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    // TODO: make sure it needs to return a node and not a MarkovNode
    Node *head = markov_chain->database->first;
    while (head != NULL)
    {
        if (strcmp((head->data)->data, data_ptr) == 0)
        {
            // return head;
            break;
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
MarkovNode *create_markov_node(char *data_ptr)
{
    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL)
    {
        return NULL;
    }
    new_markov_node->data = data_ptr;
    new_markov_node->counter_list = NULL;
    new_markov_node->list_size = 0;
    return new_markov_node;
}

Node *add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node *in_db = get_node_from_database(markov_chain, data_ptr);
    if (in_db != NULL)
    { // as if the node is known word
        return in_db;
    }
    // todo: check if its in the database/it's not null
    MarkovNode *new_markov_node = create_markov_node(data_ptr);
    if (new_markov_node == NULL)
    {
        return NULL;
    }

    add(markov_chain->database, new_markov_node);
    return markov_chain->database->last;
}

