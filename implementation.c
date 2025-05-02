/* Real time Mock Trading system implementation.
Taking liberal inspiration from:
  SQT Mock Trading
  Optiver Delta Dash

Game sequence:
    1. initialisation
        a. Host sets questions and answers (initialise_question)
        b. Lobby Starts (dw for offline implementation)
        c. Players connect (dw for offline implementation)
    2. Gameloop
        a. Question is shown to all players 
        b. Players can choose to make bid/ask offers
        c. Bid/Ask offers are recorded into orderbook (Pointers at highest bid
        vs lowest ask)
        d. matched trades are notified, unmatched remain until canceled
        e. new information is dropped, loop to b, else progress
    3. Settlement
        a. hosts answer is used to settle market.
        b. scores/current positions are settled.
        c. Players are ranked based on their final position and score.
        
    4. Looping between 2. and 3.
    
    5. Final
        a. Players are scored based off their peformance for every single round
        either:
            A) Players are scored based on standard distribution of scores
            B) Players are scored off position
            C) Players are scored off Money made
        b. Players are ranked accordingly. 
  
  Discussion
    1. Datastructure of orderbook?
    2. Main function points to "mini-main" functions, such as main for seq 1,
    seq 2 etc. Make a graph/network to make everything more interpretable. 
    3. Running multiple games simultaneously? Honestly thats going to be a hard
    ask, probably only 1 game at a time. Give someone backdoor handling in the 
    server, maybe operate everything via terminal. Or just make an admin panel
    UNSW quantsoc email logins to/controls. 
    4. The more I work with the project the better I understand web dev. This is
    the easy part lol
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Global variable tracking player_count
int global_player_counter = 100;

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

// Struct for orderbook
typedef struct orderHead {
    orders *next;                       // points to first order
    int volume;                         // + for asks, - for bids. 
    int price;                          // use in orderbook not in player orders
} orderHead

// Struct for order
typedef struct order {
    player *owner;                      // Player that owns it
    int hashkey;                        // Which order it is
    int volume;                         // + for asks, - for bids. 
    order *next;                        // Next in line
    order *prev;                        // Prev, can point to head, NULL if 1st
    orderHead *head;                    // Head of the struct
} order

// Struct for individual players
// check current position using outstanding bids and asks to ensure player
// doesn't exceed position limit 
typedef struct player {
    char *player_name;                  // player name
    int player_number;                  // player identifier
    int balance;                        // current balance
    int position;                       // position, 
    orderHead *outstanding_bids;        // outstanding bids
    orderHead *outstanding_asks;        // outstanding aks
} player

typedef struct gameLobby {
    player **players;                   // array of pointers to players
    int player_count;                   // count of active players. 
} gameLobby

// Helpers
void flush_input();                 // Flushes inputs after reading from stdin
void read_string(
    char printString[],                 // Error string
    char *destination,                  // Pointer to string to write into
    int strLen,                         // Size of destination string
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
void free_orderHead(orderHead h);   // free orderHead

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
)


///////////////////////////////////// MAIN /////////////////////////////////////

int main(void) {
    gameData *game = initialise_game();
    gameLobby *lobby = initialise_lobby();

    // loop here until game start? I guess a mutex should be sufficient. 
    // 2 threads, one to pull game data and to start looby initialisation
    // and 1 thread to focus on game progress. 
    // If a player leaves free all their orders but keep their spot in the game

    game_loop(game, lobby);

    free_lobby(lobby);
    free_game(game);

    return 0;
}

//////////////////////////////////// HELPER ////////////////////////////////////

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void read_string(char printString[], char *destination, int strLen) {
    while (1) {
        printf("%s", printString);
        if (fgets(destination, MAX_STRING_LENGTH, stdin) == NULL) {
            printf("%s", STRING_READ_ERROR);
            flush_input();
            continue;
        }

        size_t len = strlen(destination);
        if (len > 0 && (destination[len - 1] == '\n')) {
            destination[len - 1] = '\0';
        }

        printf("%s \"%s\": ", STRING_ERROR_CHECK, destination);

        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }
}

void read_int(char printString[], int *destination, int lower, int upper) {
    while (1) {
        printf("%s", printString);
        if (scanf("%d", destination) != 1) {
            printf("%s", INT_READ_ERROR);
            flush_input();
            continue;
        }

        flush_input();

        if (*destination < lower || *destination > upper) {
            printf("%s [%d - %d]", INT_RANGE_ERROR, lower, upper);
            continue;
        }

        printf("%s \"%d\": ", STRING_ERROR_CHECK, *destination);

        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }
}

///////////////////////////////////// FREE /////////////////////////////////////

// Frees every questions in game, then frees game.
void free_game(gameData *game) {
    for (int i = 0;i < game->question_count;i++) {
        free_question(game->questions[i]);
    }
    free(game->questions);
    free(game);
}

// Frees hints, then question
void free_question(question *q) {
    for (int i = 0;i < q->hint_count;i++) free(q->hints[i]);
    free(q->question);
    free(q->hints);
    free(q);
}

// Frees every player in lobby
void free_lobby(gameLobby *lobby) {
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) {
        if (lobby->players[i] != NULL) free_player(lobby->players[i]);
    } 
    free(lobby);
}

// Frees contents then player.
void free_player(player *p) {
    free(p->player_name);
    free_orderHead(p->outstanding_bids);
    free_orderHead(p->outstanding_asks);
    free(p);
}

// Order is a linked list, so frres as such. 
void free_orderHead(orderHead h) {
    order temp = h->next, prev = h->next;

    while (temp != NULL) {
        prev = temp;
        temp = temp->next;
        free(prev);
    }
}

///////////////////////////////// SEQUENCE 1a) /////////////////////////////////


gameData *initialise_game() {
    gameData *game = malloc(sizeof(gameData));
    game->question_count = 0;

    printf("%s", GAME_ROUNDS);

    // Reading what intended number of game rounds should be. 
    read_int(
        GAME_ROUNDS, 
        &game->question_count, 
        GAME_ROUND_LOWER, 
        GAME_ROUND_UPPER
    );

    game->questions = malloc(game->question_count * sizeof(question *));

    for (
        game->current_question = 0; 
        game->current_question < game->question_count;
        game->current_question++
    ) game->questions[game->current_question] = initialise_question();

    return game;
}

question *initialise_question() {
    question *q = malloc(sizeof(question));
    q->question = malloc(MAX_STRING_LENGTH * sizeof(char));
    q->hints = malloc(MAX_HINTS * sizeof(char *));
    q->hint_count = 0;

    for (int i = 0; i < MAX_HINTS; i++) q->hints[i] = NULL;

    // Read question
    read_string(STRING_WRITE_Q, q->question, MAX_STRING_LENGTH);

    // Read answer
    read_int(STRING_WRITE_ANS, &q->answer, ANSWER_LOWER, ANSWER_UPPER);

    // Add hint
    printf("%s", STRING_ADD_HINT);
    char user_input = getchar();
    flush_input();

    while (user_input == 'y') {
        flush_input();
        q->hints[q->hint_count] = malloc(MAX_STRING_LENGTH * sizeof(char));
        read_string(
            STRING_WRITE_HINT, 
            q->hints[q->hint_count++], 
            MAX_STRING_LENGTH
        );

        if (q->hint_count < MAX_HINTS) {
            printf("%s", STRING_MORE_HINT);
            user_input = getchar();
        } else {
            break;
        }
    }

    return q;
}

gameLobby *initialise_lobby() {
    gameLobby *lobby = malloc(sizeof(gameLobby));
    lobby->players = malloc(DEFAULT_PLAYER_MAX * sizeof(player));
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) lobby->players[i] = NULL;
    lobby->player_count = 0;

    while (lobby->player_count < DEFAULT_PLAYER_MAX) {
        initialise_player(lobby->players[lobby->player_count++]);

        printf("%s", ADD_PLAYER_QUERY);
        if (getchar() == 'n') {
            flush_input();
            break;
        }
    }

    return lobby;
}

void *initialise_player(player *p) {
    p->player_name = malloc(MAX_NAME_LENGTH * sizeof(char));
    read_string(REGO_PLAYER_NAME, p->player_name, MAX_NAME_LENGTH);
    p->player_number = global_player_counter;
    global_player_counter++;
    p->balance = 0;
    p->position = 0;
    p->asks = NULL;
    p->bids = NULL;
}

///////////////////////////////// SEQUENCE 2a) /////////////////////////////////

// Start game, players can make orders. Format will be:
// player (bid / ask) (price) (volume)
void game_loop(gameData *g) {

}

// player orders get appended to a que, orders are executed when this is called.
void dequeue_orders(
    orderHead *que,
    orderHead *orderbook
) {

}

// prints order book to a csv file. something that can be processed easily for
// the visual component
void output_orderbook(orderHead, *orderbook) {

}

///////////////////////////////// SEQUENCE 3a) /////////////////////////////////
// setteling market
// Take true value, cleanup orderbook and settle the position of everyone in the
// orderbook. 
void settle_game(gameData *g, gameLobby *lobby, orderHead *orderbook) {

}