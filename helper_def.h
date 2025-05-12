/*
** File: helper_def.h
** Project: QuantSoc Mock Real Time Mock Trading
*/

/******************************************************************************
 ***                   H E L P E R    D E C L R A T I O N                   ***
 ******************************************************************************
 *                                                                            *
 *                 Project Name : QuantSoc Real Time                          *
 *                                Mock Trading Game                           *                      *
 *                                                                            *
 *                       Author:: Victor Tang                                 *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *		Defines constants, strings and forward declrations of structs, funct- *
 * -ions and helpers.														  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/////////////////////////////////// LIBRARIES //////////////////////////////////


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/////////////////////////////////// CONSTANTS //////////////////////////////////

/*--------------------------------  INTEGERS  --------------------------------*/


#define MAX_STRING_LENGTH   200     // Max length of any string
#define MAX_NAME_LENGTH     20      // Max player name length
#define MAX_HINTS           10      // Max hints for any question
#define ORDER_STR_LENGTH    10      // Max length of order string

#define ANSWER_UPPER        1000    // upper/lower bound for integer answers
#define ANSWER_LOWER        1

#define GAME_ROUND_UPPER    10      // upper/lower bound for number of rounds
#define GAME_ROUND_LOWER    1

#define DEFAULT_POSITION_LIMIT  30  // default position limit
#define DEFAULT_PLAYER_MAX      30  // deafult max players
#define DEFAULT_ORDERBOOK_SIZE  1000    // default orderbook size
#define DEFAULT_PRICE_MAX       1000    // default max orders
#define DEFAULT_PRICE_MIN       0   // Minimum price

#define DEFAULT_ORDEHEAD_LEN    20  // Default orderhead length
#define DEFAULT_ORDERHEAD_INC   10  // Default orderhead increment


/*--------------------------------  STRINGS  ---------------------------------*/


#define GAME_ROUNDS         "How many rounds would you like to have?: "

#define STRING_WRITE_Q      "Write Question: "

#define STRING_WRITE_ANS    "Whats the answer: "

#define STRING_WRITE_HINT   "Whats your hint: "
#define STRING_ADD_HINT     "Would you like to add a hint: "
#define STRING_MORE_HINT    "Would you like to add another hint: "
#define STRING_MAX_HINTS    "Max hints reached"
#define YES_NO              "(\"y\" or \"n\")"

#define STRING_ERROR_CHECK  "Are there any errors"
#define STRING_READ_ERROR   "Error reading input. Please try again"

#define INT_READ_ERROR      "Invalid input. Please enter a valid integer.\n"
#define INT_RANGE_ERROR     "Input out of range"

#define STRING_ANSWER_ERROR "Please answer with \"y\" or \"n\": "

#define ADD_PLAYER_QUERY    "Add another player: "
#define REGO_PLAYER_NAME    "Player name: "

#define PLAYER_NOT_FOUND   "Player not found in lobby.\n"

#define ORDER_WRITE        "Input order (PRICE/VOLUME/(BID (B) / ASK (A))): "

#define ORDER_STR_ERROR    "Invalid order string.\n"
#define ORDER_STR_ERROR_2  "Position limit reached.\n"


/*----------------------------------------------------------------------------*/

//////////////////////////////////// STRUCTS ///////////////////////////////////

/*------------------------ Forward Def and Comments  -------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct game             game;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct gameData         gameData;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct gameQuestion     gameQuestion;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct gameLobby        gameLobby;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct player           player;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct orderbook        orderbook;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct orderHead        orderHead;

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
typedef struct order            order;


/*----------------------------------------------------------------------------*/

////////////////////////////////// FUNCTIONS ///////////////////////////////////

/*-------------------------------  Sequence 1  -------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
game *initialise_game();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
gameData *initialise_game_data();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
gameQuestion *initialise_question();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
gameLobby *initialise_lobby();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
orderbook *initialise_orderbook();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
orderHead *initialise_orderHead();

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
player *initialise_player();


/*-------------------------------  Sequence 2  -------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void game_loop(
    game *g                             // game data
);                             		// Start game

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
order *parse_order(
	game *g                           	// game data
);								 	// Parse order from string

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
order *create_order(
    char *order_str,                 	// string to create order
	player *p                          	// player to create order for
);                                  // Create order from string       

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void match_order(
	order *ord, 						// order to match
	orderbook *book						// orderbook to match against
);									// match order with orderbook

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void match_bid(
    orderHead *node,               		// orderbook  
    order *ord                          // order
);                                  // match bid orders  

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void match_ask(
    orderHead *node,               		// orderbook
    order *ord                          // order
);                                  // match ask orders 


/*-------------------------------  Sequence 3  -------------------------------*/





/*----------------------------------------------------------------------------*/

//////////////////////////////////// HELPER ////////////////////////////////////

/*---------------------------------  INPUT  ----------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void flush_input();                 // Flushes inputs after reading from stdin

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void read_string(
    char printString[],                 // Error string
    char *destination,                  // Pointer to string to write into
    int strLen                          // Size of destination string
);                                  // reads strings

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void read_int(
    char printString[],                 // Error String
    int *destination,                   // Pointer to integer to write into
    int lower,                          // Upper bound for int
    int upper                           // Lower bound
); 									// Reads an integer into an address.


/*---------------------------------  OUTPUT  ---------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void game_log(
    game *g,                            // game ptr
    char mode                           // mode of logging   
);                                  // logs game to file or terminal.

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void print_orderbook(
    orderbook *orderbook                // orderbook ptr    
);                                  // prints orderbook to terminal or file

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void print_help();                  // Prints help menu


/*------------------------------  CONDITIONAL  -------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
bool check_response() ;             // Checks if the response is 'y' or 'n'

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
player *find_player(
    gameLobby *lob,                     // game lobby
    char *name                          // name of player
);                                  // finds player in lobby


/*------------------------------  LINKED LIST  -------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void append_order(
    orderHead *head,                    // orderhead to appened to
    order *ord                          // order to append
);                                  // appends order to orderhead


/*---------------------------------  FREE  -----------------------------------*/


/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_game(
    game *g                             // game ptr
);                                  // frees game

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_game_data(
    gameData *data                      // game data ptr
);                                  // frees game data

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_question(
    gameQuestion *q                     // question ptr
);                                  // frees question

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_lobby(
    gameLobby *lob                      // lobby ptr
);                                  // free lobby

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_player(   
    player *p                           // player ptr
);                                  // free player

/*
*	Full Name 
*
*	Description of what it does
*
*	Input
*
*  	Output
*
*/
void free_orderHead(
    orderHead *h                        // orderHead ptr
);                                  // free orderHead


/*----------------------------------------------------------------------------*/