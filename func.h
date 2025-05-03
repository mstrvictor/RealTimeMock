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
#define STRING_ADD_HINT     "Would you like to add a hint"
#define STRING_MORE_HINT    "Would you like to add another hint"
#define STRING_MAX_HINTS    "Max hints reached"
#define YES_NO              "(\"y\" or \"n\")"

// Error strings for reading strings
#define STRING_ERROR_CHECK  "Are there any errors"
#define STRING_READ_ERROR   "Error reading input. Please try again"

// Error strings for reading integers
#define INT_READ_ERROR      "Invalid input. Please enter a valid integer."
#define INT_RANGE_ERROR     "Input out of range"

// Strings for adding players
#define ADD_PLAYER_QUERY    "Add another player"
#define REGO_PLAYER_NAME    "Player name: "


//////////////////////////////////// STRUCTS ///////////////////////////////////


typedef struct game             game;
typedef struct gameData         gameData;
typedef struct gameQuestion     gameQuestion;
typedef struct gameLobby        gameLobby;
typedef struct player           player;
typedef struct orderHead        orderHead;
typedef struct order            order;

// Information for individual specifics games
typedef struct game {
    gameData    *game_data;             // array of questions and count
    gameLobby   *game_lobby;            // game lobby
    orderHead   *orderbook;             // orderbook
    orderHead   *que;                   // queue of orders
    int     current_question;           // Current question in play
} game;

// Array of questions and total number of questions
typedef struct gameData {
    gameQuestion    **questions;        // Array of questions
    int     question_count;             // Total number of questions
} gameData;

// Struct for individual questions
typedef struct gameQuestion {
    char    *question;                  // String of question
    int     answer;                     // Answer
    char    **hints;                    // Array of strings that are hints
    int     hint_count;                 // Number of hints
} gameQuestion;

// Struct for game lobby
typedef struct gameLobby {
    player  **players;                  // array of pointers to players
    int     player_count;               // count of active players
} gameLobby;

// Struct for individual players
typedef struct player {
    char    *player_name;               // player name
    int     player_number;              // player identifier
    int     balance;                    // current balance
    int     position;                   // position
    orderHead   *outstanding_bids;      // outstanding bids
    orderHead   *outstanding_asks;      // outstanding asks
} player;

// Struct for orderbook
typedef struct orderHead {
    order   *next;                      // points to first order
    int     volume;                     // + for asks, - for bids
    int     price;                      // use in orderbook not in player orders
} orderHead;

// Struct for order
typedef struct order {
    player  *owner;                     // Player that owns it
    int     hashkey;                    // Which order it is
    int     volume;                     // + for asks, - for bids
    order   *next;                      // Next in line
    order   *prev;                      // Prev, can point to head, NULL if 1st
    orderHead   *head;                  // Head of the struct
} order;


///////////////////////////////// SEQUENCE 1a) /////////////////////////////////


gameQuestion *create_questions();   // Initialise questiions

gameData *initialise_game();        // Initialise game

gameQuestion *initialise_question();    // initialise questions

gameLobby *initialise_lobby();      // Initialise lobby

player *initialise_player();        // initialise players


///////////////////////////////// SEQUENCE 2a) /////////////////////////////////


void game_loop(
    gameData *g                         // game data
);                                  // Start game

void dequeue_orders(
    orderHead *que,                     // que of orders
    orderHead *orderbook                // actual orderbook.
);                                  // dequeue orders


///////////////////////////////// SEQUENCE 3a) /////////////////////////////////





//////////////////////////////////// HELPER ////////////////////////////////////


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

void error_log(
    game *g                             // game ptr
);                                 // logs errors to file


///////////////////////////////////// FREE /////////////////////////////////////


void free_game(
    gameData *g                          // game ptr
);                                  // frees game

void free_question(
    gameQuestion *q                     // question ptr
);                                  // frees question

void free_lobby(
    gameLobby *lob                      // lobby ptr
);                                  // free lobby

void free_player(   
    player *p                           // player ptr
);                                  // free player

void free_orderHead(
    orderHead *ordH                     // orderHead ptr
);                                  // free orderHead
