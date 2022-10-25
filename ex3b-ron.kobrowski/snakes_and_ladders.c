#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"
#define NDEBUG

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define DECIMAL 10
#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define USAGE_MESSAGE "USAGE: 1: seed value \n       2: number of sentences to generate\n"

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
                // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;


/**
 * compares the numbers of the cells
 * @param cell1
 * @param cell2
 * @return
 */
static int cell_comp(void *cell1, void *cell2)
{
    Cell *cell_1 = cell1;
    Cell *cell_2 = cell2;
    int ret = cell_1->number - cell_2->number;
    return ret;
}

static void free_cell(void *cell)
{
    Cell* cell_pointer = cell;
    free(cell_pointer);
    cell = NULL;
}

static void print_cell(void *cell)
{
    Cell *cell_type = cell;
    if (cell_type->ladder_to != EMPTY)
    {
        fprintf(stdout, "[%d]-ladder to %d -> ", cell_type->number,
                cell_type->ladder_to);
    } else if (cell_type->snake_to != EMPTY)
    {
        fprintf(stdout, "[%d]-snake to %d -> ", cell_type->number,
                cell_type->snake_to);
    } else
    {
        if (cell_type->number == BOARD_SIZE)
        {
            fprintf(stdout, "[%d]", cell_type->number);
        } else
        {
            fprintf(stdout, "[%d] -> ", cell_type->number);
        }
    }
}


static bool is_last_cell(void *cell)
{
    Cell *cell_type = cell;
    if (cell_type->number >= BOARD_SIZE)
    {
        return true;
    }
    return false;
}


/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}

static void *cell_copier(void *cell)
{
    Cell *old_cell = cell;
    Cell *new_cell = malloc(sizeof(Cell));
    if (new_cell == NULL){
        handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
        return NULL;
    }
    new_cell->number = old_cell->number;
    new_cell->ladder_to = old_cell->ladder_to;
    new_cell->snake_to = old_cell->snake_to;
    return new_cell;
}

static bool check_args(const int argc)
{
    if (argc != 3)
    {
        return false;
    } else
    {
        return true;
    }

}

static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        } else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell *cells[BOARD_SIZE];
    if (create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        if(!add_to_database(markov_chain, cells[i])){
            handle_error(ALLOCATION_ERROR_MASSAGE, &markov_chain);
            return EXIT_FAILURE;
        }
    }
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain, cells[i])->data;
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        } else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell *) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                Node* to = get_node_from_database(markov_chain, cells[index_to]);
                if(to == NULL){
                    handle_error(ALLOCATION_ERROR_MASSAGE, &markov_chain);
                    return EXIT_FAILURE;
                }
                to_node = to->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

static bool init_database(MarkovChain **walk_paths)
{
    (*walk_paths)->database = malloc(sizeof(LinkedList));
    if ((*walk_paths)->database == NULL){
        handle_error(ALLOCATION_ERROR_MASSAGE, walk_paths);
        return false;
    }
    (*walk_paths)->database->size = 0;
    (*walk_paths)->database->first = NULL;
    (*walk_paths)->database->last = NULL;
    return true;
}

static MarkovChain *init_walk_paths()
{
    MarkovChain *walk_paths = malloc(sizeof(MarkovChain));
    if (walk_paths == NULL){
        handle_error(ALLOCATION_ERROR_MASSAGE, NULL);
        return NULL;
    }
    if (!init_database(&walk_paths)){
        return false;
    }
    walk_paths->print_func = print_cell;
    walk_paths->comp_func = cell_comp;
    walk_paths->free_data = free_cell;
    walk_paths->copy_func = cell_copier;
    walk_paths->is_last = is_last_cell;
    return walk_paths;
}

static int generate_game(MarkovChain *walk_paths, const int num_of_walks)
{
    for (int i = 0; i < num_of_walks; i++)
    {
        MarkovNode *first_word = walk_paths->database->first->data;
        fprintf(stdout, "Random Walk %d: ", i + 1);
        generate_random_sequence(walk_paths, first_word,
                                 MAX_GENERATION_LENGTH);
        fprintf(stdout, "\n");
    }
    return EXIT_SUCCESS;
}

static int run_command(const int num_of_walks)
{
    MarkovChain *walk_paths = init_walk_paths();

    int fill = fill_database(walk_paths);
    if (fill == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    generate_game(walk_paths, num_of_walks);
    free_markov_chain(&walk_paths);
    return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    bool args_validity = check_args(argc);
    if (!args_validity)
    {
        handle_error(USAGE_MESSAGE, NULL);
        return EXIT_FAILURE;
    }
    unsigned int seed = strtol(argv[1], NULL, DECIMAL);
    srand(seed);
    unsigned int num_of_walks = strtol(argv[2], NULL, DECIMAL);
    int ret = run_command(num_of_walks);
    return ret;
}
