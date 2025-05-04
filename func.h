/*
** File: func.h
** Project: QuantSoc Mock Real Time Mock Trading
*/

/******************************************************************************
 ***                               H E A D E R                              ***
 ******************************************************************************
 *                                                                            *
 *                 Project Name :                                             *
 *                                                                            *
 *                       Author::                                             *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Functions:                                                                 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - -*/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


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
#define DEFAULT_ORDERBOOK_SIZE  1000    // default orderbook size
#define DEFAULT_PRICE_MAX       1000    // default max orders
#define DEFAULT_PRICE_MIN      1    // Minimum price

// Strings when handling game creation
#define GAME_ROUNDS         "How many rounds would you like to have?: "

// Strings when handling question writing
#define STRING_WRITE_Q      "Write Question: "

#define STRING_WRITE_ANS    "Whats the answer: "

#define STRING_WRITE_HINT   "Whats your hint: "
#define STRING_ADD_HINT     "Would you like to add a hint: "
#define STRING_MORE_HINT    "Would you like to add another hint: "
#define STRING_MAX_HINTS    "Max hints reached"
#define YES_NO              "(\"y\" or \"n\")"

// Error strings for reading strings
#define STRING_ERROR_CHECK  "Are there any errors"
#define STRING_READ_ERROR   "Error reading input. Please try again"

// Error strings for reading integers
#define INT_READ_ERROR      "Invalid input. Please enter a valid integer.\n"
#define INT_RANGE_ERROR     "Input out of range"

// Answer with 'y' or 'n'
#define STRING_ANSWER_ERROR "Please answer with \"y\" or \"n\": "

// Strings for adding players
#define ADD_PLAYER_QUERY    "Add another player: "
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
    gameData    *data;                  // array of questions and count
    gameLobby   *lobby;                 // game lobby
    orderHead   *orderbook;             // orderbook
    orderHead   *que;                   // queue of orders
    int     current_question;           // Current question in play
} game;

// Array of questions and total number of questions
typedef struct gameData {
    gameQuestion    **questions;        // Array of questions
    int     count;                      // Total number of questions
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


game *initialise_game();

gameData *initialise_game_data();

gameQuestion *initialise_question();

gameLobby *initialise_lobby();

player *initialise_player();


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

bool check_response() {
    char temp;

    while ((temp = tolower(getchar())) != 'y') {
        if (temp == 'n') {
            flush_input();
            return false;
        } else if (temp != 'y') {
            printf("%s", STRING_ANSWER_ERROR);
            flush_input();
            continue;
        }
    }

    return true;
}

void game_log(
    game *g,                            // game ptr
    char mode                           // mode of logging   
);                                  // logs errors to file

void print_orderbook(
    orderHead *orderbook,               // orderbook ptr
    char mode                           // mode of printing      
);                                  // prints orderbook to terminal or file


///////////////////////////////////// FREE /////////////////////////////////////


void free_game(
    game *g                             // game ptr
);                                  // frees game

void free_game_data(
    gameData *data                      // game data ptr
);                                  // frees game data

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
