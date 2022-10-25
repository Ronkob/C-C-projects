#include "markov_chain.h"
#include "linked_list.h"
#include <stdio.h>

void print_chain(MarkovChain *chain)
{
    if(!chain || !chain->database || !chain->database->first || !chain->database->first->data)
    {
        return;
    }
    Node *p_node = chain->database->first;
    FILE* fp = fopen("unique.txt", "w");
    for(int i = 0; i < chain->database->size; i++)
    {
        fprintf(fp, "%s:\n", p_node->data->data);
        for(int j = 0; j < p_node->data->list_size; j++)
        {
            fprintf(fp,"  - %s %d\n", p_node->data->counter_list[j]
            .markov_node->data, p_node->data->counter_list[j].frequency);
        }
        p_node = p_node->next;
    }
}
