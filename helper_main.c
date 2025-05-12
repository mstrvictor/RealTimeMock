/*
** File: helper_main.h
** Project: QuantSoc Mock Real Time Mock Trading
*/

/******************************************************************************
 ***                         H E L P E R    M A I N                         ***
 ******************************************************************************
 *                                                                            *
 *                 Project Name : QuantSoc Real Time                          *
 *                                Mock Trading Game                           *                      *
 *                                                                            *
 *                       Author:: Victor Tang                                 *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *		Function body of helper structs functions and helpers.			      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "helper_def.h"

//////////////////////////////////// STRUCTS ///////////////////////////////////


typedef struct game {
    gameData    *data;                  // array of questions and count
    gameLobby   *lobby;                 // game lobby
    orderbook   *orderbook;             // orderbook
    orderHead   *que;                   // queue of orders
    int     currQ;           			// Current question in play
} game;

typedef struct gameData {
    gameQuestion    **questions;        // Array of questions
    int     count;                      // Total number of questions
} gameData;

typedef struct gameQuestion {
    char    *question;                  // String of question
    int     answer;                     // Answer
    char    **hints;                    // Array of strings that are hints
    int     hint_count;                 // Number of hints
} gameQuestion;

typedef struct gameLobby {
    player  **players;                  // array of pointers to players
    int     player_count;               // count of active players
} gameLobby;

typedef struct player {
    char    *player_name;               // player name
    int     player_number;              // player identifier
    int     balance;                    // current balance
    int     position;                   // position
	order   *orders;                   	// orders player has
	int 	size;						// size of orders
	int 	count;						// number of orders placed by player
} player;

typedef struct orderbook {
    orderHead   *bid_high;              // highest bid
    orderHead   *ask_low;               // lowest ask
    orderHead   *ask_high;              // highest ask. for printing orderbook
} orderbook;

typedef struct orderHead {
    order   **list;                     // list of orders
    int     count;                      // number of orders
    int     size;                  		// size of list
    int     volume;                     // + for asks, - for bids
    int     price;                      // Price of orders
    orderHead  *next;                   // next orderhead
    orderHead  *prev;                   // prev orderhead
} orderHead;

typedef struct order {
    player  *owner;                     // Player that owns it
	int    order_number;                // Order number (player specific)
    int     volume;                     // 
    int     price;                      // 
    char    type;                       // 'A' for ask, 'B' for bid
	order   *next;                      // Next order in orderbook
    order   *prev;                      // Previous order
} order;


/////////////////////////////////  SEQUENCE 1  /////////////////////////////////


int global_player_counter = 100;

game *initialise_game() {
    game *g = malloc(sizeof(game));
    
    // gameData *data
    g->data = initialise_game_data();

    // gameLobby *lobby
    g->lobby = initialise_lobby();

    // orderbook *orderbook
    g->orderbook = initialise_orderbook();
    
    // orderHead *que
    g->que = initialise_orderHead();

    // int current_question
    g->currQ = 0;

    return g;
}

gameData *initialise_game_data() {
    gameData *data = malloc(sizeof(gameData));

    // How many rounds of questions in this game?
    read_int(
        GAME_ROUNDS,
        &data->count,
        GAME_ROUND_LOWER,
        GAME_ROUND_UPPER
    );
 
   // Malloc for array of pointers to question_count
    data->questions = malloc(data->count * sizeof(gameQuestion *));
    
    // Initialise each question
    for (int i = 0;i < data->count;i++) {
        data->questions[i] = initialise_question();
    }

    return data;
}

gameQuestion *initialise_question() {
    gameQuestion *q = malloc(sizeof(gameQuestion));
    q->question = malloc(MAX_STRING_LENGTH * sizeof(char));
    q->hints = malloc(MAX_HINTS * sizeof(char *));
    q->hint_count = 0;

    // Read question
    read_string(
        STRING_WRITE_Q, 
        q->question, 
        MAX_STRING_LENGTH
    );

    // Read answer
    read_int(
        STRING_WRITE_ANS,
        &q->answer,
        ANSWER_LOWER,
        ANSWER_UPPER
    );

    // Add hint
    printf("%s", STRING_ADD_HINT);
    char user_input = getchar();

    while (user_input == 'y') {
        flush_input();
        q->hints[q->hint_count] = malloc(MAX_STRING_LENGTH * sizeof(char));

        // Read hint
        read_string(
            STRING_WRITE_HINT, 
            q->hints[q->hint_count], 
            MAX_STRING_LENGTH
        );

        q->hint_count++;

        if (q->hint_count < MAX_HINTS) {
            printf("%s", STRING_MORE_HINT);
            user_input = getchar();
        } else {
            break;
        }
    }

    flush_input();

    return q;
}

gameLobby *initialise_lobby() {
    gameLobby *lobby = malloc(sizeof(gameLobby));
    
    lobby->players = malloc(DEFAULT_PLAYER_MAX * sizeof(player *));

    lobby->player_count = 0;

    while (lobby->player_count < DEFAULT_PLAYER_MAX) {
        lobby->players[lobby->player_count] = initialise_player();
        lobby->player_count++;

        printf("%s", ADD_PLAYER_QUERY);
        if (check_response()) {
            continue;
        } else {
            break;
        }
    }

    return lobby;
}

orderbook *initialise_orderbook() {
    orderbook *ob = malloc(sizeof(orderbook));
    ob->ask_low = NULL;
    ob->bid_high = NULL;
    ob->ask_high = NULL;

    return ob;
}

orderHead *initialise_orderHead() {
    orderHead *h = malloc(sizeof(orderHead));
    h->list = malloc(DEFAULT_ORDEHEAD_LEN * sizeof(order *));
    h->count = 0;
    h->size = DEFAULT_ORDEHEAD_LEN;
    h->volume = 0;
    h->price = -1;
    h->next = NULL;
    h->prev = NULL;

    for (int i = 0;i < h->size;i++) h->list[i] = NULL;

    return h;
}

player *initialise_player() {
    player *p = malloc(sizeof(player));

    p->player_name = malloc(MAX_NAME_LENGTH * sizeof(char));

    read_string(
        REGO_PLAYER_NAME,
        p->player_name,
        MAX_NAME_LENGTH
    );

    p->player_number = global_player_counter++;

    p->balance = 0;
    p->position = 0;

    p->outstanding_bids = malloc(sizeof(orderHead));
    p->outstanding_bids->next = NULL;
    p->outstanding_bids->volume = 0;
    p->outstanding_bids->price = 0;

    p->outstanding_asks = malloc(sizeof(orderHead));
    p->outstanding_asks->next = NULL;
    p->outstanding_asks->volume = 0;
    p->outstanding_asks->price = 0;

    return p;
}


/////////////////////////////////  SEQUENCE 2  /////////////////////////////////


void game_loop(game *g) {
    char input;
    while ((input = tolower(getchar())) != EOF) {
        flush_input();

        // segments for progresing games?
        if (input == 'q') {\
            // end game
            printf("Game ended\n");
            return;

        } else if (input == 'o') {
            order *ord = parse_order(g);

            match_order(ord, g->orderbook);
        } else if (input == 'h'){
            // print help
            print_help();

        } else if (input == 's') {
            // Print orderbook
            print_orderbook(g->orderbook);

        } else {
            printf("%s", STRING_READ_ERROR);

        }
    }
}

// reading input to order
order *parse_order(game *g) {
    // create order for player
    // Find player
    char player_name[MAX_NAME_LENGTH];
    char order_str[ORDER_STR_LENGTH];

    // read from stdin player name
    read_string(
        REGO_PLAYER_NAME,
        player_name, 
        MAX_NAME_LENGTH
    );

    // read order string
    read_string(
        ORDER_WRITE,
        order_str,
        ORDER_STR_LENGTH
    );

    // call write order
    return create_order(
        order_str,
        find_player(g->lobby, player_name)
    );
}

// Create an order from an order string. 
order *create_order(char *order_str, player *p) {
    order *ord = malloc(sizeof(order));
    char *token = NULL;

    token = strtok(order_str, "/");
    if (strcmp(token, "B") == 0) {
        ord->type = 'B';
    } else if (strcmp(token, "A") == 0) {
        ord->type = 'A';
    } else {
        printf("%s", ORDER_STR_ERROR);
        free(ord);
        return NULL;
    }

    token = strtok(NULL, "/");
    ord->price = atoi(token);
    if (
        ord->price <= DEFAULT_PRICE_MIN ||
        ord->price >= DEFAULT_PRICE_MAX
    ) {
        printf("%s", ORDER_STR_ERROR);
        free(ord);
        return NULL;
    }

    token = strtok(NULL, "/");
    ord->volume = atoi(token);
    int standing = p->position;

    if (ord->type == 'B') {
        standing += p->outstanding_bids->volume;
    } else {
        standing += p->outstanding_asks->volume;
    }

    if (standing + ord->volume > DEFAULT_POSITION_LIMIT) {
        printf("%s", ORDER_STR_ERROR_2);
        free(ord);
        return NULL;
    }

    ord->owner = p;
    ord->next = NULL;
    ord->prev = NULL;
    ord->head = NULL;

    return ord;
}

// Appends order to orderhead type
void append_order(orderHead *head, order *ord) {
    // check if head is NULL
    if (!head) {
        head = initialise_orderHead();
    }

    // check if head is full
    if (head->count == head->size) {
        head->size += DEFAULT_ORDERHEAD_INC;
        head->list = realloc(
            head->list, 
            head->size * sizeof(order *)
        );
    }

    // add order to head
    head->list[head->count] = ord;
    head->count++;
}

// Tries to match order with orderbook, if fail then appends to orderbook.
void match_order(order *ord, orderbook *book) {
    if (ord->type == 'B') {
        if (book->ask_low->price <= ord->price) {
            match_bid(book->ask_low, ord);
        } 

    } else if (ord->type == 'A') {
        if (book->bid_high->price >= ord->price) {
            match_ask(book->bid_high, ord);
        }


    }
}

void match_bid(orderHead *node, order *ord) {
    int i = 0;
    order *temp = NULL;

    for (
        i = 0;
        (ord->volume > 0 && i < node->count);
        i++
    ) {
        temp = node->list[i];
        
        if (temp->volume > ord->volume) {
            // if temp volume is greater then order volume
            // add money to owner
            temp->owner->balance += ord->volume * temp->price;
            temp->owner->position -= ord->volume;
            // subtract volume from order
            temp->volume -= ord->volume;

            // subtract money from buyer
            ord->owner->balance -= ord->volume * temp->price;
            ord->owner->position += ord->volume;

            // free order
        } else {

        }
    }

    // if order isn't fully filled, continue matching, else append
    if (
        ord->volume > 0 &&
        node->next &&
        node->next->price <= ord->price
    ) {
        match_bid(node->next, ord);
    } else {
        // append bid to bid list
    }
 
    // free orderhead and redo orderbook, 
    if (node->prev) node->prev = node->next;
    if (node->next) node->next->prev = node->prev;
    free_orderHead(node);

}

void match_ask(orderHead *node, order *ord) {

}


/////////////////////////////////  SEQUENCE 3  /////////////////////////////////



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

        if (check_response()) {
            continue;
        } else {
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

        if (check_response()) {
            continue;
        } else {
            break;
        }
    }
}

void game_log(game *g, char mode) {
    if (mode == 't') {
        // // Print to terminal
        // printf("Game data:\n");
        // printf("Number of rounds: %d\n", g->data->count);
        // printf("Current question: %d\n", g->current_question);

        // // priknts info about questions
        // for (int i = 0;i < g->data->count;i++) {
        //     printf("Question %d: %s\n", i + 1, g->data->questions[i]->question);
        //     printf("Answer: %d\n", g->data->questions[i]->answer);
        //     printf("Hints:\n");
        //     for (int j = 0;j < g->data->questions[i]->hint_count;j++) {
        //         printf("\t%d: %s\n", j, g->data->questions[i]->hints[j]);
        //     }
        // }

        // // prints out orderbook
        // printf("Orderbook data:\n");
        // printf("||----||-----||----||\n");
        // printf("||Bids||Price||Asks||\n");
        // printf("||----||-----||----||\n");
        // for (int i = 999;i >= 0;i--) {
        //     orderHead *temp = g->orderbook[i];
        //     if (temp) {
        //         if (temp->volume > 0) {
        //             printf("||----||-%03d-||%04d||\n", (i + 1), temp->volume);
        //         } else if (temp->volume < 0){
        //             printf("||%04d||-%03d-||----||\n", temp->volume, (i + i));
        //         }
        //     }
        // }
        // printf("||----||-----||----||\n");

    } else if (mode == 'f') {
        // Print to file

    } else {
        printf("Invalid mode");

    }
}

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

player *find_player(gameLobby *lob, char *name) {
    // finds player in lobby
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) {
        if (
            lob->players[i] && 
            strcmp(lob->players[i]->player_name, name) == 0
        ) {
            return lob->players[i];
        }
    }

    return NULL;
}


///////////////////////////////////// FREE /////////////////////////////////////


// Frees contents of game struct then game
void free_game(game *g) {
    free_game_data(g->data);

    free_lobby(g->lobby);

    free(g->orderbook);

    free_orderHead(g->que);

    free(g);
}

// Frees array of questions then gameData
void free_game_data(gameData *data) {
    for (int i = 0;i < data->count;i++) {
        free_question(data->questions[i]);
    }

    free(data->questions);
    free(data);
}

// Frees hints, then answer string, then question
void free_question(gameQuestion *q) {
    for (int i = 0;i < q->hint_count;i++) free(q->hints[i]);
    free(q->hints);

    free(q->question);

    free(q);
}

// Frees every active player in lobby
void free_lobby(gameLobby *lob) {
    for (int i = 0;i < DEFAULT_PLAYER_MAX;i++) {
        // Listed as such since players can leave game, so an entry between
        // active players can be NULL.
        if (!lob->players[i]) free_player(lob->players[i]);
    } 

    free(lob->players);
    free(lob);
}

// Frees contents then player.
void free_player(player *p) {
    free(p->player_name);

    free_orderHead(p->outstanding_bids);
    free_orderHead(p->outstanding_asks);

    free(p);
}

// Order is a linked list, so frees as such. 
void free_orderHead(orderHead *h) {
    if (!h) return;

    for (int i = 0;i < h->count;i++) {
        if (h->list[i]) {
            free(h->list[i]);
        }
    }

    free(h);
}