// Constants
#define MAX_STRING_LENGTH   200     // Max length of any string
#define MAX_NAME_LENGTH     20      // Max player name length
#define MAX_HINTS           10      // Max hints for any question

#define ANSWER_UPPER        1000    // upper/lower bound for integer answers
#define ANSWER_LOWER        1

#define GAME_ROUND_UPPER    10      // upper/lower bound for number of rounds
#define GAME_ROUND_LOWER    1

#define DEFAULT_POSITION_LIMIT  30  // default position limit
#define DEFAULT_PLAYER_MAX      30  // deafult max players

// Strings when handling game creation
#define GAME_ROUNDS         "How many rounds would you like to have?: "

// Strings when handling question writing
#define STRING_WRITE_Q      "Write Question: "
#define STRING_WRITE_ANS    "Whats the answer: "
#define STRING_WRITE_HINT   "Whats your hint: "
#define STRING_ADD_HINT     "Would you like to add a hint (\"y\" or \"n\")?: "
#define STRING_MORE_HINT    "Would you like to add another hint (\"y\" or \"n\")?: "
#define STRING_MAX_HINTS    "Max hints reached"

// Error strings for reading strings
#define STRING_ERROR_CHECK  "Are there any errors (\"y\" or \"n\")?"
#define STRING_READ_ERROR   "Error reading input. Please try again"

// Error strings for reading integers
#define INT_READ_ERROR      "Invalid input. Please enter a valid integer."
#define INT_RANGE_ERROR     "Input out of range "

// Strings for adding players
#define ADD_PLAYER_QUERY    "Add another player (\"y\" or \"n\")?"
#define REGO_PLAYER_NAME    "Player name: "

typedef struct question question;
typedef struct gameData gameData;
typedef struct player player;
typedef struct orderHead orderHead;
typedef struct order order;
typedef struct gameLobby gameLobby;

// Struct for individual questions
typedef struct question {
    char *question;                     // String of question
    int answer;                         // Answer
    char **hints;                       // Array of strings that are hints
    int hint_count;                     // Number of hints
} question;

// Struct for games as a whole
typedef struct gameData {
    question **questions;               // Array of questions
    int current_question;               // Current question in play
    int question_count;                 // Total number of questions
} gameData;

// Struct for individual players
typedef struct player {
    char *player_name;                  // player name
    int player_number;                  // player identifier
    int balance;                        // current balance
    int position;                       // position
    orderHead *outstanding_bids;        // outstanding bids
    orderHead *outstanding_asks;        // outstanding asks
} player;

// Struct for orderbook
typedef struct orderHead {
    order *next;                       // points to first order
    int volume;                         // + for asks, - for bids
    int price;                          // use in orderbook not in player orders
} orderHead;

// Struct for order
typedef struct order {
    player *owner;                      // Player that owns it
    int hashkey;                        // Which order it is
    int volume;                         // + for asks, - for bids
    order *next;                        // Next in line
    order *prev;                        // Prev, can point to head, NULL if 1st
    orderHead *head;                    // Head of the struct
} order;

// Struct for game lobby
typedef struct gameLobby {
    player **players;                   // array of pointers to players
    int player_count;                   // count of active players
} gameLobby;

// Helpers
void flush_input();                 // Flushes inputs after reading from stdin
void read_string(
    char printString[],                 // Error string
    char *destination,                  // Pointer to string to write into
    int strLen                          // Size of destination string
);                                  // reads strings
void read_int(
    char printString[],                 // Error String
    int *destination,                   // Pointer to integer to write into
    int lower,                          // Upper bound for int
    int upper                           // Lower bound
);                                  // Reads an integer into an address.

// Free
void free_game(gameData *game);     // frees game
void free_question(question *q);    // frees question
void free_lobby(gameLobby *lobby);  // free lobby
void free_player(player *p);        // free player
void free_orderHead(orderHead *h);   // free orderHead

// Game sequence 1. a)
gameData *initialise_game();        // Initialise game
question *initialise_question();    // initialise questions
gameLobby *initialise_lobby();      // Initialise lobby
void *initialise_player(player *p); // initialise players

// Game sequence 2.
void game_loop(gameData *g);        // Start game
void dequeue_orders(
    orderHead *que,                 // que of orders
    orderHead *orderbook            // actual orderbook.
);